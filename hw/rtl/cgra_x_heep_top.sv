// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module cgra_x_heep_top #(
    parameter PULP_XPULP    = 0,
    parameter FPU           = 0,
    parameter PULP_ZFINX    = 0
) (
    inout logic         clk_i,
    inout logic         rst_ni,

    //visibility signals
    output logic        rst_led,
    output logic        clk_led,
    output logic        clk_out,

    inout logic         boot_select_i,
    inout logic         execute_from_flash_i,

    inout  logic        jtag_tck_i,
    inout  logic        jtag_tms_i,
    inout  logic        jtag_trst_ni,
    inout  logic        jtag_tdi_i,
    inout  logic        jtag_tdo_o,

    inout               uart_rx_i,
    inout               uart_tx_o,

    inout  logic [22:0] gpio_io,

    output logic [31:0] exit_value_o,
    inout  logic        exit_valid_o,

    inout  logic [ 3:0] spi_flash_sd_io,
    inout  logic        spi_flash_csb_o,
    inout  logic        spi_flash_sck_o,

    inout  logic [ 3:0] spi_sd_io,
    inout  logic        spi_csb_o,
    inout  logic        spi_sck_o,

    inout logic         spi2_sd_0_io,
    inout logic         spi2_sd_1_io,
    inout logic         spi2_sd_2_io,
    inout logic         spi2_sd_3_io,
    inout logic [1:0]   spi2_csb_io,
    inout logic         spi2_sck_o,


    inout logic         i2c_scl_io,
    inout logic         i2c_sda_io
);

  import obi_pkg::*;
  import reg_pkg::*;
  import cgra_x_heep_pkg::*;

  // External xbar master/slave and peripheral ports
  obi_req_t [cgra_x_heep_pkg::EXT_XBAR_NMASTER-1:0] ext_xbar_master_req;
  obi_resp_t [cgra_x_heep_pkg::EXT_XBAR_NMASTER-1:0] ext_xbar_master_resp;
  obi_req_t ext_xbar_slave_req;
  obi_resp_t ext_xbar_slave_resp;
  reg_req_t ext_periph_slave_req;
  reg_rsp_t ext_periph_slave_resp;

  // External interrupts
  logic [core_v_mini_mcu_pkg::NEXT_INT-1:0] ext_intr_vector;

  // External subsystems
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_rst_n;
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_ram_banks_set_retentive;

  logic cgra_int;
  logic cgra_enable;
  logic cgra_logic_rst_n;
  logic cgra_ram_banks_set_retentive;

  always_comb begin
    // All interrupt lines set to zero by default
    for (int i = 0; i < core_v_mini_mcu_pkg::NEXT_INT; i++) begin
      ext_intr_vector[i] = 1'b0;
    end
    // Re-assign the interrupt lines used here
    ext_intr_vector[0] = cgra_int;
  end

  // CGRA logic clock gating unit enable (always-on in this case)
  assign cgra_enable = 1'b1;
  assign cgra_logic_rst_n = external_subsystem_rst_n[0];
  assign cgra_ram_banks_set_retentive = external_ram_banks_set_retentive[0];

  cgra_top_wrapper cgra_top_wrapper_i (
      .clk_i,
      .rst_ni,
      .cgra_enable_i(cgra_enable),
      .rst_logic_ni(cgra_logic_rst_n),
      .masters_req_o(ext_xbar_master_req),
      .masters_resp_i(ext_xbar_master_resp),
      .reg_req_i(ext_periph_slave_req),
      .reg_rsp_o(ext_periph_slave_resp),
      .slave_req_i(ext_xbar_slave_req),
      .slave_resp_o(ext_xbar_slave_resp),
      .cmem_set_retentive_i(cgra_ram_banks_set_retentive),
      .cgra_int_o(cgra_int)
  );

  // eXtension Interface
  if_xif #() ext_if ();

  x_heep_system #(
      .PULP_XPULP(PULP_XPULP),
      .FPU(FPU),
      .PULP_ZFINX(PULP_ZFINX),
      .EXT_XBAR_NMASTER(cgra_x_heep_pkg::EXT_XBAR_NMASTER)
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
      .gpio_0_io(gpio_io[0]),
      .gpio_1_io(gpio_io[1]),
      .gpio_2_io(gpio_io[2]),
      .gpio_3_io(gpio_io[3]),
      .gpio_4_io(gpio_io[4]),
      .gpio_5_io(gpio_io[5]),
      .gpio_6_io(gpio_io[6]),
      .gpio_7_io(gpio_io[7]),
      .gpio_8_io(gpio_io[8]),
      .gpio_9_io(gpio_io[9]),
      .gpio_10_io(gpio_io[10]),
      .gpio_11_io(gpio_io[11]),
      .gpio_12_io(gpio_io[12]),
      .gpio_13_io(gpio_io[13]),
      .gpio_14_io(gpio_io[14]),
      .gpio_15_io(gpio_io[15]),
      .gpio_16_io(gpio_io[16]),
      .gpio_17_io(gpio_io[17]),
      .gpio_18_io(gpio_io[18]),
      .gpio_19_io(gpio_io[19]),
      .gpio_20_io(gpio_io[20]),
      .gpio_21_io(gpio_io[21]),
      .gpio_22_io(gpio_io[22]),

      .spi2_cs_0_io(spi2_csb_io[0]),
      .spi2_cs_1_io(spi2_csb_io[1]),
      .spi2_sck_io(spi2_sck_o),
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
      .ext_xbar_master_req_i(ext_xbar_master_req),
      .ext_xbar_master_resp_o(ext_xbar_master_resp),
      .ext_xbar_slave_req_o(ext_xbar_slave_req),
      .ext_xbar_slave_resp_i(ext_xbar_slave_resp),
      .ext_peripheral_slave_req_o(ext_periph_slave_req),
      .ext_peripheral_slave_resp_i(ext_periph_slave_resp),
      .external_subsystem_powergate_switch_o(),
      .external_subsystem_powergate_switch_ack_i(),
      .external_subsystem_powergate_iso_o(),
      .external_subsystem_rst_no(external_subsystem_rst_n),
      .external_ram_banks_set_retentive_o(external_ram_banks_set_retentive)
  );

endmodule  // cgra_x_heep_pkg
