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
    input  logic                 set_retentive_ni,
    // output ports
    output logic [         31:0] rdata_o   // read data
);

  localparam int unsigned NUM_WORDS = NumWords;
  
  generate
    if (NUM_WORDS == 128) begin
      xilinx_emem_gen xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else if (NUM_WORDS == 512) begin
      xilinx_mem_gen_2k xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else if (NUM_WORDS == 1024) begin
      xilinx_mem_gen_4k xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else if (NUM_WORDS == 2048) begin
      xilinx_mem_gen_8k xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else if (NUM_WORDS == 4096) begin
      xilinx_mem_gen_16k xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else if (NUM_WORDS == 8192) begin
      xilinx_mem_gen_32k xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else if (NUM_WORDS == 16384) begin
      xilinx_mem_gen_64k xilinx_sram_i (
        .clka (clk_i),
        .ena  (req_i),
        .wea  ({4{req_i & we_i}} & be_i),
        .addra(addr_i),
        .dina (wdata_i),
        // output ports
        .douta(rdata_o)
      );
    end else begin
      $error("Bank size not generated for NumWords = %0d.", NumWords);
    end
  endgenerate

endmodule