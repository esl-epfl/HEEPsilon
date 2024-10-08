// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module xilinx_heepsilon_wrapper
  import obi_pkg::*;
  import reg_pkg::*;
#(
    parameter COREV_PULP    = 0,
    parameter FPU           = 0,
    parameter ZFINX    = 0,
    parameter CLK_LED_COUNT_LENGTH = 27
) (
`ifdef FPGA_ZCU104
    inout logic clk_300mhz_n,
    inout logic clk_300mhz_p,
`else
    inout logic clk_i,
`endif
    inout logic rst_i,

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

    output logic        exit_value_o,
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

  wire                               clk_gen;
  logic [                      31:0] exit_value;
  wire                               rst_n;
  logic [CLK_LED_COUNT_LENGTH - 1:0] clk_count;

  // low active reset
  assign rst_n   = !rst_i;

  // reset LED for debugging
  assign rst_led = rst_n;
  
  // counter to blink an LED
  assign clk_led = clk_count[CLK_LED_COUNT_LENGTH-1];

  always_ff @(posedge clk_gen or negedge rst_n) begin : clk_count_process
    if (!rst_n) begin
      clk_count <= '0;
    end else begin
      clk_count <= clk_count + 1;
    end
  end

  // eXtension Interface
  if_xif #() ext_if ();

  // clock output for debugging
  assign clk_out = clk_gen;

`ifdef FPGA_ZCU104
  xilinx_clk_wizard_wrapper xilinx_clk_wizard_wrapper_i (
      .CLK_IN1_D_0_clk_n(clk_300mhz_n),
      .CLK_IN1_D_0_clk_p(clk_300mhz_p),
      .clk_out1_0(clk_gen)
  );
`else  // FPGA PYNQ-Z2
  xilinx_clk_wizard_wrapper xilinx_clk_wizard_wrapper_i (
      .clk_125MHz(clk_i),
      .clk_out1_0(clk_gen)
  );
`endif

  heepsilon_top #(
        .COREV_PULP (0),
        .FPU        (0),
        .ZFINX (0)
      ) heepsilon_top_i (
        .clk_i(clk_gen),
        .rst_ni(rst_n),
        .boot_select_i,
        .execute_from_flash_i,
        .jtag_tck_i,
        .jtag_tms_i,
        .jtag_trst_ni,
        .jtag_tdi_i,
        .jtag_tdo_o,
        .uart_rx_i,
        .uart_tx_o,
        .gpio_io,
        .exit_value_o(exit_value),
        .exit_valid_o,
        .spi_flash_sd_io,
        .spi_flash_csb_o,
        .spi_flash_sck_o,
        .spi_sd_io,
        .spi_csb_o,
        .spi_sck_o,
        .spi2_sd_0_io,
        .spi2_sd_1_io,
        .spi2_sd_2_io,
        .spi2_sd_3_io,
        .spi2_csb_io,
        .spi2_sck_o,
        .i2c_scl_io,
        .i2c_sda_io
  );

  assign exit_value_o = exit_value[0];

endmodule
