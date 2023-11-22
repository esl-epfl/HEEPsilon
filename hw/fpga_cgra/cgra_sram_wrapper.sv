// Copyright 2023 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module cgra_sram_wrapper #(
    parameter int unsigned NumWords = 32'd1024,  // Number of Words in data array
    parameter int unsigned DataWidth = 32'd32,  // Data signal width
    // DEPENDENT PARAMETERS, DO NOT OVERWRITE!
    parameter int unsigned AddrWidth = (NumWords > 32'd1) ? $clog2(NumWords) : 32'd1
) (
    input  logic                 clk_i,    // Clock
    input  logic                 rst_ni,   // Asynchronous reset active low
    // input ports
    input  logic                 req_i,    // request
    input  logic                 we_i,     // write enable
    input  logic [AddrWidth-1:0] addr_i,   // request address
    input  logic [         31:0] wdata_i,  // write data
    input  logic [          3:0] be_i,     // write byte enable
    input  logic                 set_retentive_i,
    // output ports
    output logic [         31:0] rdata_o   // read data
);
  sram_wrapper#(
    .NumWords (NumWords),
    .DataWidth(DataWidth),
    .AddrWidth(AddrWidth)
  ) sram_wrapper_i(
    .clk_i          (clk_i),
    .rst_ni         (rst_ni),
    .req_i          (req_i),
    .we_i           (we_i),
    .addr_i         (addr_i),
    .wdata_i        (wdata_i),
    .be_i           (be_i),
    .set_retentive_ni(set_retentive_i),
    // output ports
    .rdata_o        (rdata_o)
  );

endmodule