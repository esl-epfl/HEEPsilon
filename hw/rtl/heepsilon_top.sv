// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module heepsilon_top #(
    parameter COREV_PULP = 0,
    parameter FPU        = 0,
    parameter ZFINX      = 0,
    parameter X_EXT      = 0
) (
    inout logic clk_i,
    inout logic rst_ni,

    inout logic boot_select_i,
    inout logic execute_from_flash_i,

    inout logic jtag_tck_i,
    inout logic jtag_tms_i,
    inout logic jtag_trst_ni,
    inout logic jtag_tdi_i,
    inout logic jtag_tdo_o,

    inout uart_rx_i,
    inout uart_tx_o,

    inout logic [22:0] gpio_io,

    output logic [31:0] exit_value_o,
    inout  logic        exit_valid_o,

    inout logic [3:0] spi_flash_sd_io,
    inout logic       spi_flash_csb_o,
    inout logic       spi_flash_sck_o,

    inout logic [3:0] spi_sd_io,
    inout logic       spi_csb_o,
    inout logic       spi_sck_o,

    inout logic       spi2_sd_0_io,
    inout logic       spi2_sd_1_io,
    inout logic       spi2_sd_2_io,
    inout logic       spi2_sd_3_io,
    inout logic [1:0] spi2_csb_io,
    inout logic       spi2_sck_o,

    inout logic i2c_scl_io,
    inout logic i2c_sda_io
);

  import obi_pkg::*;
  import reg_pkg::*;
  import heepsilon_pkg::*;

  // External xbar master/slave and peripheral ports
  obi_req_t ext_xbar_slave_req;
  obi_resp_t ext_xbar_slave_resp;
  reg_req_t ext_periph_slave_req;
  reg_rsp_t ext_periph_slave_resp;
  obi_req_t [heepsilon_pkg::CGRA_XBAR_NMASTER-1:0] ext_master_req;
  obi_req_t [heepsilon_pkg::CGRA_XBAR_NMASTER-1:0] heep_slave_req;
  obi_resp_t [heepsilon_pkg::CGRA_XBAR_NMASTER-1:0] ext_master_resp;
  obi_resp_t [heepsilon_pkg::CGRA_XBAR_NMASTER-1:0] heep_slave_resp;
  obi_req_t heep_core_instr_req;
  obi_resp_t heep_core_instr_resp;
  obi_req_t heep_core_data_req;
  obi_resp_t heep_core_data_resp;
  obi_req_t heep_debug_master_req;
  obi_resp_t heep_debug_master_resp;
  obi_req_t heep_dma_read_ch0_req;
  obi_resp_t heep_dma_read_ch0_resp;
  obi_req_t heep_dma_write_ch0_req;
  obi_resp_t heep_dma_write_ch0_resp;
  obi_req_t heep_dma_addr_ch0_req;
  obi_resp_t heep_dma_addr_ch0_resp;



  // External interrupts
  logic [core_v_mini_mcu_pkg::NEXT_INT-1:0] ext_intr_vector;

  logic cgra_int;
  logic cgra_enable;
  logic cgra_logic_rst_n;
  logic cgra_ram_banks_set_retentive_n;

  // External subsystems
  logic external_subsystem_rst_n;
  logic external_ram_banks_set_retentive_n;
  /* verilator lint_off unused */
  logic external_subsystem_clkgate_en_n;
  logic external_subsystem_powergate_switch_n;
  logic external_subsystem_powergate_switch_ack_n;
  logic external_subsystem_powergate_iso_n;

  // CGRA logic clock gating unit enable (always-on in this case)
  assign cgra_enable                    = 1'b1;
  assign cgra_logic_rst_n               = external_subsystem_rst_n;
  assign cgra_ram_banks_set_retentive_n = external_ram_banks_set_retentive_n;

  always_comb begin
    // All interrupt lines set to zero by default
    for (int i = 0; i < core_v_mini_mcu_pkg::NEXT_INT; i++) begin
      ext_intr_vector[i] = 1'b0;
    end
    // Re-assign the interrupt lines used here
    ext_intr_vector[0] = cgra_int;
  end


  // External bus
  // ----------------------
  // The external bus connects the external peripherals among them and to
  // the corresponding X-HEEP slave port (to the internal system bus).
  ext_bus #(
      .EXT_XBAR_NMASTER(CGRA_XBAR_NMASTER),
      .EXT_XBAR_NSLAVE (1)
  ) ext_bus_i (
      .clk_i        (clk_i),
      .rst_ni       (rst_ni),
      .addr_map_i   (EXT_XBAR_ADDR_RULES),
      .default_idx_i('0),

      .heep_core_instr_req_i    (heep_core_instr_req),
      .heep_core_instr_resp_o   (heep_core_instr_resp),
      .heep_core_data_req_i     (heep_core_data_req),
      .heep_core_data_resp_o    (heep_core_data_resp),
      .heep_debug_master_req_i  (heep_debug_master_req),
      .heep_debug_master_resp_o (heep_debug_master_resp),
      .heep_dma_read_req_i  (heep_dma_read_ch0_req),
      .heep_dma_read_resp_o (heep_dma_read_ch0_resp),
      .heep_dma_write_req_i (heep_dma_write_ch0_req),
      .heep_dma_write_resp_o(heep_dma_write_ch0_resp),
      .heep_dma_addr_req_i  (heep_dma_addr_ch0_req),
      .heep_dma_addr_resp_o (heep_dma_addr_ch0_resp),

      .ext_master_req_i (ext_master_req),
      .ext_master_resp_o(ext_master_resp),
      .heep_slave_req_o (heep_slave_req),
      .heep_slave_resp_i(heep_slave_resp),
      .ext_slave_req_o  (ext_xbar_slave_req),
      .ext_slave_resp_i (ext_xbar_slave_resp)
  );

  cgra_top_wrapper cgra_top_wrapper_i (
      .clk_i,
      .rst_ni,
      .cgra_enable_i(cgra_enable),
      .rst_logic_ni(cgra_logic_rst_n),
      .masters_req_o(ext_master_req),
      .masters_resp_i(ext_master_resp),
      .reg_req_i(ext_periph_slave_req),
      .reg_rsp_o(ext_periph_slave_resp),
      .slave_req_i(ext_xbar_slave_req),
      .slave_resp_o(ext_xbar_slave_resp),
      .cmem_set_retentive_ni(cgra_ram_banks_set_retentive_n),
      .cgra_int_o(cgra_int)
  );

  // eXtension Interface
  if_xif #() ext_if ();

  x_heep_system #(
      .COREV_PULP(COREV_PULP),
      .FPU(FPU),
      .ZFINX(ZFINX),
      .X_EXT(X_EXT),
      .EXT_XBAR_NMASTER(CGRA_XBAR_NMASTER)
  ) x_heep_system_i (
      .clk_i,
      .rst_ni,
      .jtag_tck_i,
      .jtag_tms_i,
      .jtag_trst_ni,
      .jtag_tdi_i,
      .jtag_tdo_o,
      .boot_select_i,
      .execute_from_flash_i,
      .exit_valid_o,
      .uart_rx_i,
      .uart_tx_o,
      .gpio_0_io     (gpio_io[0]),
      .gpio_1_io     (gpio_io[1]),
      .gpio_2_io     (gpio_io[2]),
      .gpio_3_io     (gpio_io[3]),
      .gpio_4_io     (gpio_io[4]),
      .gpio_5_io     (gpio_io[5]),
      .gpio_6_io     (gpio_io[6]),
      .gpio_7_io     (gpio_io[7]),
      .gpio_8_io     (gpio_io[8]),
      .gpio_9_io     (gpio_io[9]),
      .gpio_10_io    (gpio_io[10]),
      .gpio_11_io    (gpio_io[11]),
      .gpio_12_io    (gpio_io[12]),
      .gpio_13_io    (gpio_io[13]),
      .gpio_14_io    (gpio_io[14]),
      .gpio_15_io    (gpio_io[15]),
      .gpio_16_io    (gpio_io[16]),
      .gpio_17_io    (gpio_io[17]),
      .pdm2pcm_pdm_io(gpio_io[18]),
      .pdm2pcm_clk_io(gpio_io[19]),
      .i2s_sck_io    (gpio_io[20]),
      .i2s_ws_io     (gpio_io[21]),
      .i2s_sd_io     (gpio_io[22]),

      .spi2_cs_0_io(spi2_csb_io[0]),
      .spi2_cs_1_io(spi2_csb_io[1]),
      .spi2_sck_io (spi2_sck_o),
      .spi2_sd_0_io(spi2_sd_0_io),
      .spi2_sd_1_io(spi2_sd_1_io),
      .spi2_sd_2_io(spi2_sd_2_io),
      .spi2_sd_3_io(spi2_sd_3_io),

      .i2c_sda_io(i2c_sda_io),
      .i2c_scl_io(i2c_scl_io),

      .spi_flash_sck_io(spi_flash_sck_o),
      .spi_flash_cs_0_io(spi_flash_csb_o),
      .spi_flash_cs_1_io(),
      .spi_flash_sd_0_io(spi_flash_sd_io[0]),
      .spi_flash_sd_1_io(spi_flash_sd_io[1]),
      .spi_flash_sd_2_io(spi_flash_sd_io[2]),
      .spi_flash_sd_3_io(spi_flash_sd_io[3]),
      .spi_sck_io(spi_sck_o),
      .spi_cs_0_io(spi_csb_o),
      .spi_cs_1_io(),
      .spi_sd_0_io(spi_sd_io[0]),
      .spi_sd_1_io(spi_sd_io[1]),
      .spi_sd_2_io(spi_sd_io[2]),
      .spi_sd_3_io(spi_sd_io[3]),

      .exit_value_o,
      .intr_vector_ext_i(ext_intr_vector),
      .xif_compressed_if(ext_if),
      .xif_issue_if(ext_if),
      .xif_commit_if(ext_if),
      .xif_mem_if(ext_if),
      .xif_mem_result_if(ext_if),
      .xif_result_if(ext_if),

      .ext_xbar_master_req_i(heep_slave_req),
      .ext_xbar_master_resp_o(heep_slave_resp),
      .ext_core_instr_req_o(heep_core_instr_req),
      .ext_core_instr_resp_i(heep_core_instr_resp),
      .ext_core_data_req_o(heep_core_data_req),
      .ext_core_data_resp_i(heep_core_data_resp),
      .ext_debug_master_req_o(heep_debug_master_req),
      .ext_debug_master_resp_i(heep_debug_master_resp),
      .ext_dma_read_req_o(heep_dma_read_ch0_req),
      .ext_dma_read_resp_i(heep_dma_read_ch0_resp),
      .ext_dma_write_req_o(heep_dma_write_ch0_req),
      .ext_dma_write_resp_i(heep_dma_write_ch0_resp),
      .ext_dma_addr_req_o(heep_dma_addr_ch0_req),
      .ext_dma_addr_resp_i(heep_dma_addr_ch0_resp),
      .external_subsystem_clkgate_en_no(external_subsystem_clkgate_en_n),
      .ext_peripheral_slave_req_o(ext_periph_slave_req),
      .ext_peripheral_slave_resp_i(ext_periph_slave_resp),
      .external_subsystem_powergate_switch_no(external_subsystem_powergate_switch_n),
      .external_subsystem_powergate_switch_ack_ni(external_subsystem_powergate_switch_ack_n),
      .external_subsystem_powergate_iso_no(external_subsystem_powergate_iso_n),

      .external_subsystem_rst_no(external_subsystem_rst_n),
      .external_ram_banks_set_retentive_no(external_ram_banks_set_retentive_n)
  );

endmodule  // heepsilon_pkg
