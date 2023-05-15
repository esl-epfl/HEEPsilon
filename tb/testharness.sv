// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module testharness #(
    parameter PULP_XPULP    = 0,
    parameter FPU           = 0,
    parameter PULP_ZFINX    = 0,
    parameter JTAG_DPI      = 0,
    parameter CLK_FREQUENCY = 'd100_000  //KHz
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
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_powergate_switch;
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_powergate_switch_ack;
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_powergate_iso;

  cgra_x_heep_top #(
      .PULP_XPULP(PULP_XPULP),
      .FPU(FPU),
      .PULP_ZFINX(PULP_ZFINX)
  ) cgra_x_heep_top_i (
      .clk_i,
      .rst_ni,

      .boot_select_i,
      .execute_from_flash_i,

      .jtag_tck_i  (sim_jtag_tck),
      .jtag_tms_i  (sim_jtag_tms),
      .jtag_trst_ni(sim_jtag_trstn),
      .jtag_tdi_i  (sim_jtag_tdi),
      .jtag_tdo_o  (sim_jtag_tdo),

      .gpio_io(gpio),

      .uart_rx_i(uart_rx),
      .uart_tx_o(uart_tx),

      .spi_flash_sd_io (spi_flash_sd_io),
      .spi_flash_csb_io(spi_flash_csb),
      .spi_flash_sck_io(spi_flash_sck),

      .spi_sd_io (spi_sd_io),
      .spi_csb_io(spi_csb),
      .spi_sck_io(spi_sck),

      .external_subsystem_powergate_switch_o(external_subsystem_powergate_switch),
      .external_subsystem_powergate_switch_ack_i(external_subsystem_powergate_switch_ack),
      .external_subsystem_powergate_iso_o(external_subsystem_powergate_iso),

      .exit_value_o,
      .exit_valid_o
  );

  //pretending to be SWITCH CELLs that delay by SWITCH_ACK_LATENCY cycles the ACK signal
  logic
      tb_cpu_subsystem_powergate_switch_ack[SWITCH_ACK_LATENCY+1],
      tb_peripheral_subsystem_powergate_switch_ack[SWITCH_ACK_LATENCY+1];
  logic [core_v_mini_mcu_pkg::NUM_BANKS-1:0] tb_memory_subsystem_banks_powergate_switch_ack[SWITCH_ACK_LATENCY+1];
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] tb_external_subsystem_powergate_switch_ack[SWITCH_ACK_LATENCY+1];
  logic delayed_tb_cpu_subsystem_powergate_switch_ack;
  logic delayed_tb_peripheral_subsystem_powergate_switch_ack;
  logic [core_v_mini_mcu_pkg::NUM_BANKS-1:0] delayed_tb_memory_subsystem_banks_powergate_switch_ack;
  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] delayed_tb_external_subsystem_powergate_switch_ack;

  always_ff @(negedge clk_i) begin
    tb_cpu_subsystem_powergate_switch_ack[0] <= cgra_x_heep_top_i.x_heep_system_i.cpu_subsystem_powergate_switch;
    tb_peripheral_subsystem_powergate_switch_ack[0] <= cgra_x_heep_top_i.x_heep_system_i.peripheral_subsystem_powergate_switch;
    tb_memory_subsystem_banks_powergate_switch_ack[0] <= cgra_x_heep_top_i.x_heep_system_i.memory_subsystem_banks_powergate_switch;
    tb_external_subsystem_powergate_switch_ack[0] <= external_subsystem_powergate_switch;
    for (int i = 0; i < SWITCH_ACK_LATENCY; i++) begin
      tb_memory_subsystem_banks_powergate_switch_ack[i+1] <= tb_memory_subsystem_banks_powergate_switch_ack[i];
      tb_cpu_subsystem_powergate_switch_ack[i+1] <= tb_cpu_subsystem_powergate_switch_ack[i];
      tb_peripheral_subsystem_powergate_switch_ack[i+1] <= tb_peripheral_subsystem_powergate_switch_ack[i];
      tb_external_subsystem_powergate_switch_ack[i+1] <= tb_external_subsystem_powergate_switch_ack[i];
    end
  end

  assign delayed_tb_cpu_subsystem_powergate_switch_ack = tb_cpu_subsystem_powergate_switch_ack[SWITCH_ACK_LATENCY];
  assign delayed_tb_peripheral_subsystem_powergate_switch_ack = tb_peripheral_subsystem_powergate_switch_ack[SWITCH_ACK_LATENCY];
  assign delayed_tb_memory_subsystem_banks_powergate_switch_ack = tb_memory_subsystem_banks_powergate_switch_ack[SWITCH_ACK_LATENCY];
  assign delayed_tb_external_subsystem_powergate_switch_ack = tb_external_subsystem_powergate_switch_ack[SWITCH_ACK_LATENCY];

  always_comb begin
`ifndef VERILATOR
    force cgra_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.cpu_subsystem_powergate_switch_ack_i = delayed_tb_cpu_subsystem_powergate_switch_ack;
    force cgra_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.peripheral_subsystem_powergate_switch_ack_i = delayed_tb_peripheral_subsystem_powergate_switch_ack;
    force cgra_x_heep_top_i.x_heep_system_i.core_v_mini_mcu_i.memory_subsystem_banks_powergate_switch_ack_i = delayed_tb_memory_subsystem_banks_powergate_switch_ack;
    force external_subsystem_powergate_switch_ack = delayed_tb_external_subsystem_powergate_switch_ack;
`else
    cgra_x_heep_top_i.x_heep_system_i.cpu_subsystem_powergate_switch_ack = delayed_tb_cpu_subsystem_powergate_switch_ack;
    cgra_x_heep_top_i.x_heep_system_i.peripheral_subsystem_powergate_switch_ack = delayed_tb_peripheral_subsystem_powergate_switch_ack;
    cgra_x_heep_top_i.x_heep_system_i.memory_subsystem_banks_powergate_switch_ack = delayed_tb_memory_subsystem_banks_powergate_switch_ack;
    external_subsystem_powergate_switch_ack = delayed_tb_external_subsystem_powergate_switch_ack;
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
