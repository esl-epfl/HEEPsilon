// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module testharness #(
    parameter COREV_PULP                  = 0,
    parameter FPU                         = 0,
    parameter ZFINX                       = 0,
    parameter X_EXT                       = 0,
    parameter USE_EXTERNAL_DEVICE_EXAMPLE = 0,
    parameter JTAG_DPI                    = 0,
    parameter CLK_FREQUENCY               = 'd100_000  //KHz
) (
    inout logic clk_i,
    inout logic rst_ni,

    inout logic boot_select_i,
    inout logic execute_from_flash_i,

    inout  logic        jtag_tck_i,
    inout  logic        jtag_tms_i,
    inout  logic        jtag_trst_ni,
    inout  logic        jtag_tdi_i,
    inout  logic        jtag_tdo_o,
    output logic [31:0] exit_value_o,
    inout  logic        exit_valid_o
);

  `include "tb_util.svh"

  import obi_pkg::*;
  import reg_pkg::*;

  localparam SWITCH_ACK_LATENCY = 15;

  localparam EXT_DOMAINS_RND = core_v_mini_mcu_pkg::EXTERNAL_DOMAINS == 0 ? 1 : core_v_mini_mcu_pkg::EXTERNAL_DOMAINS;

  wire uart_rx;
  wire uart_tx;
  logic sim_jtag_enable = (JTAG_DPI == 1);
  wire sim_jtag_tck;
  wire sim_jtag_tms;
  wire sim_jtag_trst;
  wire sim_jtag_tdi;
  wire sim_jtag_tdo;
  wire sim_jtag_trstn;
  wire [31:0] gpio;

  wire [3:0] spi_flash_sd_io;
  wire [1:0] spi_flash_csb;
  wire spi_flash_sck;

  wire [3:0] spi_sd_io;
  wire [1:0] spi_csb;
  wire spi_sck;

  // External subsystems
  // logic [EXT_DOMAINS_RND-1:0] external_subsystem_powergate_switch_n;
  logic [EXT_DOMAINS_RND-1:0] external_subsystem_powergate_switch_ack_n;
  //   logic [EXT_DOMAINS_RND-1:0] external_subsystem_powergate_iso_n;
  //   logic [EXT_DOMAINS_RND-1:0] external_subsystem_rst_n;
  //   logic [EXT_DOMAINS_RND-1:0] external_ram_banks_set_retentive_n;
  //   logic [EXT_DOMAINS_RND-1:0] external_subsystem_clkgate_en_n;

  heepsilon_top #(
      .COREV_PULP(COREV_PULP),
      .FPU(FPU),
      .ZFINX(ZFINX),
      .X_EXT(X_EXT)
  ) heepsilon_top_i (
      .clk_i,
      .rst_ni,
      .boot_select_i,
      .execute_from_flash_i,
      .jtag_tck_i(sim_jtag_tck),
      .jtag_tms_i(sim_jtag_tms),
      .jtag_trst_ni(sim_jtag_trstn),
      .jtag_tdi_i(sim_jtag_tdi),
      .jtag_tdo_o(sim_jtag_tdo),
      .gpio_io(gpio[22:0]),
      .uart_rx_i(uart_rx),
      .uart_tx_o(uart_tx),
      .spi_flash_sd_io(spi_flash_sd_io),
      .spi_flash_csb_o(spi_flash_csb[0]),
      .spi_flash_sck_o(spi_flash_sck),
      .spi_sd_io(spi_sd_io),
      .spi_csb_o(spi_csb[0]),
      .spi_sck_o(spi_sck),
      .spi2_csb_io(gpio[24:23]),
      .spi2_sck_o(gpio[25]),
      .spi2_sd_0_io(gpio[26]),
      .spi2_sd_1_io(gpio[27]),
      .spi2_sd_2_io(gpio[28]),
      .spi2_sd_3_io(gpio[29]),
      .i2c_scl_io(gpio[31]),
      .i2c_sda_io(gpio[30]),
      .exit_value_o,
      .exit_valid_o
  );

  //pretending to be SWITCH CELLs that delay by SWITCH_ACK_LATENCY cycles the ACK signal
  logic tb_cpu_subsystem_powergate_switch_ack_n[SWITCH_ACK_LATENCY+1];
  logic tb_peripheral_subsystem_powergate_switch_ack_n[SWITCH_ACK_LATENCY+1];
  logic [core_v_mini_mcu_pkg::NUM_BANKS-1:0] tb_memory_subsystem_banks_powergate_switch_ack_n[SWITCH_ACK_LATENCY+1];
  logic [EXT_DOMAINS_RND-1:0] tb_external_subsystem_powergate_switch_ack_n[SWITCH_ACK_LATENCY+1];

  logic delayed_tb_cpu_subsystem_powergate_switch_ack_n;
  logic delayed_tb_peripheral_subsystem_powergate_switch_ack_n;
  logic [core_v_mini_mcu_pkg::NUM_BANKS-1:0] delayed_tb_memory_subsystem_banks_powergate_switch_ack_n;
  logic [EXT_DOMAINS_RND-1:0] delayed_tb_external_subsystem_powergate_switch_ack_n;

  always_ff @(negedge clk_i) begin
    tb_cpu_subsystem_powergate_switch_ack_n[0] <= heepsilon_top_i.x_heep_system_i.cpu_subsystem_powergate_switch_n;
    tb_peripheral_subsystem_powergate_switch_ack_n[0] <= heepsilon_top_i.x_heep_system_i.peripheral_subsystem_powergate_switch_n;
    tb_memory_subsystem_banks_powergate_switch_ack_n[0] <= heepsilon_top_i.x_heep_system_i.memory_subsystem_banks_powergate_switch_n;
    tb_external_subsystem_powergate_switch_ack_n[0] <= heepsilon_top_i.external_subsystem_powergate_switch_n;
    for (int i = 0; i < SWITCH_ACK_LATENCY; i++) begin
      tb_memory_subsystem_banks_powergate_switch_ack_n[i+1] <= tb_memory_subsystem_banks_powergate_switch_ack_n[i];
      tb_cpu_subsystem_powergate_switch_ack_n[i+1] <= tb_cpu_subsystem_powergate_switch_ack_n[i];
      tb_peripheral_subsystem_powergate_switch_ack_n[i+1] <= tb_peripheral_subsystem_powergate_switch_ack_n[i];
      tb_external_subsystem_powergate_switch_ack_n[i+1] <= tb_external_subsystem_powergate_switch_ack_n[i];
    end
  end

  assign delayed_tb_cpu_subsystem_powergate_switch_ack_n = tb_cpu_subsystem_powergate_switch_ack_n[SWITCH_ACK_LATENCY];
  assign delayed_tb_peripheral_subsystem_powergate_switch_ack_n = tb_peripheral_subsystem_powergate_switch_ack_n[SWITCH_ACK_LATENCY];
  assign delayed_tb_memory_subsystem_banks_powergate_switch_ack_n = tb_memory_subsystem_banks_powergate_switch_ack_n[SWITCH_ACK_LATENCY];
  assign delayed_tb_external_subsystem_powergate_switch_ack_n = tb_external_subsystem_powergate_switch_ack_n[SWITCH_ACK_LATENCY];

  always_comb begin
`ifndef VERILATOR
    force heepsilon_top_i.x_heep_system_i.core_v_mini_mcu_i.cpu_subsystem_powergate_switch_ack_ni = delayed_tb_cpu_subsystem_powergate_switch_ack_n;
    force heepsilon_top_i.x_heep_system_i.core_v_mini_mcu_i.peripheral_subsystem_powergate_switch_ack_ni = delayed_tb_peripheral_subsystem_powergate_switch_ack_n;
    force heepsilon_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_banks_powergate_switch_ack_ni = delayed_tb_memory_subsystem_banks_powergate_switch_ack_n;
    force external_subsystem_powergate_switch_ack_n = delayed_tb_external_subsystem_powergate_switch_ack_n;
`else
    heepsilon_top_i.x_heep_system_i.cpu_subsystem_powergate_switch_ack_n = delayed_tb_cpu_subsystem_powergate_switch_ack_n;
    heepsilon_top_i.x_heep_system_i.peripheral_subsystem_powergate_switch_ack_n = delayed_tb_peripheral_subsystem_powergate_switch_ack_n;
    heepsilon_top_i.x_heep_system_i.memory_subsystem_banks_powergate_switch_ack_n = delayed_tb_memory_subsystem_banks_powergate_switch_ack_n;
    heepsilon_top_i.external_subsystem_powergate_switch_ack_n = delayed_tb_external_subsystem_powergate_switch_ack_n;
`endif
  end

  uartdpi #(
      .BAUD('d256000),
      .FREQ(CLK_FREQUENCY * 1000),  //Hz
      .NAME("uart0")
  ) i_uart0 (
      .clk_i,
      .rst_ni,
      .tx_o(uart_rx),
      .rx_i(uart_tx)
  );

  // jtag calls from dpi
  SimJTAG #(
      .TICK_DELAY(1),
      .PORT      (4567)
  ) i_sim_jtag (
      .clock(clk_i),
      .reset(~rst_ni),
      .enable(sim_jtag_enable),
      .init_done(rst_ni),
      .jtag_TCK(sim_jtag_tck),
      .jtag_TMS(sim_jtag_tms),
      .jtag_TDI(sim_jtag_tdi),
      .jtag_TRSTn(sim_jtag_trstn),
      .jtag_TDO_data(sim_jtag_tdo),
      .jtag_TDO_driven(1'b1),
      .exit()
  );

`ifndef VERILATOR
  // Flash used for booting (execute from flash or copy from flash)
  spiflash flash_boot_i (
      .csb(spi_flash_csb[0]),
      .clk(spi_flash_sck),
      .io0(spi_flash_sd_io[0]),  // MOSI
      .io1(spi_flash_sd_io[1]),  // MISO
      .io2(spi_flash_sd_io[2]),
      .io3(spi_flash_sd_io[3])
  );
`endif

`ifndef VERILATOR
  // Flash used as an example device with an SPI interface
  spiflash flash_device_i (
      .csb(spi_csb[0]),
      .clk(spi_sck),
      .io0(spi_sd_io[0]),  // MOSI
      .io1(spi_sd_io[1]),  // MISO
      .io2(spi_sd_io[2]),
      .io3(spi_sd_io[3])
  );
`endif

endmodule  // testharness
