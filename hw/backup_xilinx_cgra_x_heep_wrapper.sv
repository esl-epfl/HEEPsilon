// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module xilinx_cgra_x_heep_wrapper
  import obi_pkg::*;
  import reg_pkg::*;
#(
    parameter PULP_XPULP    = 0,
    parameter FPU           = 0,
    parameter PULP_ZFINX    = 0,
    parameter CLK_LED_COUNT_LENGTH = 27
) (
    inout   logic clk_i,
    inout   logic rst_i,

    //visibility signals
    //output  logic rst_led,
    //output  logic clk_led,
    //output  logic clk_out,

    //output  logic exit_value_o, // was a [31:0] signal
    //inout   logic exit_valid_o,
    inout   logic execute_from_flash_i,
    inout   logic boot_select_i,

    // Review the assignment of SPI flash
    inout  logic [ 3:0] spi_flash_sd_io,
    inout  logic        spi_flash_csb_o, // Pin 0 is the output
    //inout  logic        spi_flash_csb_i, // Input is not used
    inout  logic        spi_flash_sck_o,


    inout               uart_tx_o,
    inout               uart_rx_i,

    
    // Not use JTAG
    //inout  logic        jtag_tck_i,
    //inout  logic        jtag_tms_i,
    //inout  logic        jtag_trst_ni,
    //inout  logic        jtag_tdi_i,
    //inout  logic        jtag_tdo_o,

    // Will only use the Tri-color LED
    //inout  logic [22:0] gpio_io,
    inout   logic [22:20] gpio_io

    // Rename SPI2 in constraints!     
    //inout   logic [1:0] spi2_csb_io,
    //inout   logic       spi2_sck_o,
    //inout   logic       spi2_sd_0_io,
    //inout   logic       spi2_sd_1_io,
    //inout   logic       spi2_sd_2_io,
    //inout   logic       spi2_sd_3_io,

    // Will not use I2C
    //inout logic i2c_scl_io,
    //inout logic i2c_sda_io,


    //inout  logic [ 3:0] spi_sd_io,
    //inout  logic [ 1:0] spi_csb_io,
    //inout  logic        spi_sck_io,

    //output logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_powergate_switch_o,
    //input  logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_powergate_switch_ack_i,
    //output logic [core_v_mini_mcu_pkg::EXTERNAL_DOMAINS-1:0] external_subsystem_powergate_iso_o

);

  wire                               clk_gen;
  logic [                      31:0] exit_value;
  wire                               rst_n;
  logic [CLK_LED_COUNT_LENGTH - 1:0] clk_count;

  // Use only a reduced number of GPIOs
  wire [31:0] gpio;
  assign gpio [22:20] = gpio_io;
  assign gpio [19:0] = 20'h0;

  // Split the flash_csb and only use the output
  wire [1:0] spi_flash_csb;
  assign spi_flash_csb[0] = spi_flash_csb_o;


  //assign exit_value_o = exit_value[0];


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

  xilinx_clk_wizard_wrapper xilinx_clk_wizard_wrapper_i (
      .clk_125MHz(clk_i),
      .clk_out1_0(clk_gen)
  );

  cgra_x_heep_top #(
        .PULP_XPULP (0),
        .FPU        (0),
        .PULP_ZFINX (0)
      ) cgra_x_heep_top_i (

        .clk_i(clk_gen),
        .rst_ni(rst_n),

        .boot_select_i(boot_select_i),
        .execute_from_flash_i(execute_from_flash_i),

        // Not use JTAG
        .jtag_tck_i(),
        .jtag_tms_i(),
        .jtag_trst_ni(),
        .jtag_tdi_i(),
        .jtag_tdo_o(),

        // Lets not use the gpios
        .gpio_io(),

        //.spi2_cs_0_io(spi2_csb_io[0]),
        //.spi2_cs_1_io(spi2_csb_io[1]),
        //.spi2_sck_io(spi2_sck_o),
        //.spi2_sd_0_io(spi2_sd_0_io),
        //.spi2_sd_1_io(spi2_sd_1_io),
        //.spi2_sd_2_io(spi2_sd_2_io),
        //.spi2_sd_3_io(spi2_sd_3_io),
        .spi2_cs_0_io(),
        .spi2_cs_1_io(),
        .spi2_sck_io(),
        .spi2_sd_0_io(),
        .spi2_sd_1_io(),
        .spi2_sd_2_io(),
        .spi2_sd_3_io(),
        
        // Will not use I2C
        .i2c_scl_io(),
        .i2c_sda_io(),

        .uart_rx_i(uart_rx_i),
        .uart_tx_o(uart_tx_o),

        .spi_flash_sd_io(spi_flash_sd_io),
        .spi_flash_csb_io(spi_flash_csb),
        .spi_flash_sck_io(spi_flash_sck_o),

        // Will not use SPI
        .spi_sd_io(),
        .spi_csb_io(),
        .spi_sck_io(),

        //.external_subsystem_powergate_switch_o(external_subsystem_powergate_switch_o),
        //.external_subsystem_powergate_switch_ack_i(external_subsystem_powergate_switch_ack_i),
        //.external_subsystem_powergate_iso_o(external_subsystem_powergate_iso_o),
        .external_subsystem_powergate_switch_o(),
        .external_subsystem_powergate_switch_ack_i(),
        .external_subsystem_powergate_iso_o(),

        // Will not use exit values
        .exit_value_o(), // Was exit_value_o
        .exit_valid_o()
  );


endmodule
