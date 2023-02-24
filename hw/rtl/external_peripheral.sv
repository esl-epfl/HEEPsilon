// Copyright 2022 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module external_peripheral
  import obi_pkg::*;
  import reg_pkg::*;
#(
    parameter EXT_XBAR_NMASTER = 0
) (
    input logic ref_clk_i,
    input logic rst_ni,

    output logic system_clk_o,

    input logic bypass_fll_i,

    // Masters
    output obi_req_t  [EXT_XBAR_NMASTER-1:0] ext_xbar_master_req_o,
    input  obi_resp_t [EXT_XBAR_NMASTER-1:0] ext_xbar_master_resp_i,

    // Slaves
    input  obi_req_t  cgra_req_i,
    output obi_resp_t cgra_resp_o,

    input  reg_req_t cgra_periph_slave_req_i,
    output reg_rsp_t cgra_periph_slave_resp_o,

    input logic cgra_ram_banks_set_retentive_i,

    input  reg_req_t fll_slave_req_i,
    output reg_rsp_t fll_slave_resp_o,

    input  reg_req_t coubertin_slave_req_i,
    output reg_rsp_t coubertin_slave_resp_o,

    input  reg_req_t       heepocrates_ctrl_slave_req_i,
    output reg_rsp_t       heepocrates_ctrl_slave_resp_o,
    input  logic     [3:0] heepocrates_ctrl_mem_sw_fb_i      [core_v_mini_mcu_pkg::NUM_BANKS],
    input  logic     [3:0] heepocrates_ctrl_cgra_mem_sw_fb_i,
    input  logic     [1:0] heepocrates_ctrl_coubertin_sw_fb_i,

    // CGRA logic reset
    input logic cgra_logic_rst_n,

    // Interrupts
    output [core_v_mini_mcu_pkg::NEXT_INT-1:0] intr_vector_ext_o
);

  obi_req_t [EXT_XBAR_NMASTER-1:0] cgra_masters_req;
  obi_resp_t [EXT_XBAR_NMASTER-1:0] cgra_masters_resp;

  logic [core_v_mini_mcu_pkg::NEXT_INT-1:0] intr_vector;
  logic cgra_int;
  logic cgra_enable;

  assign ext_xbar_master_req_o = cgra_masters_req;
  assign cgra_masters_resp = ext_xbar_master_resp_i;

  assign intr_vector_ext_o = intr_vector;

  always_comb begin
    // All interrupt lines set to zero by default
    for (int i = 0; i < core_v_mini_mcu_pkg::NEXT_INT; i++) begin
      intr_vector[i] = 1'b0;
    end
    // Re-assign the interrupt lines used here
    intr_vector[0] = cgra_int;
  end

  cgra_top_wrapper cgra_top_wrapper_i (
      .clk_i(system_clk_o),
      .rst_ni,
      .cgra_enable_i(cgra_enable),
      .rst_logic_ni(cgra_logic_rst_n),
      .masters_req_o(cgra_masters_req),
      .masters_resp_i(cgra_masters_resp),
      .reg_req_i(cgra_periph_slave_req_i),
      .reg_rsp_o(cgra_periph_slave_resp_o),
      .slave_req_i(cgra_req_i),
      .slave_resp_o(cgra_resp_o),
      .cmem_set_retentive_i(cgra_ram_banks_set_retentive_i),
      .cgra_int_o(cgra_int)
  );

  external_ctrl #(
      .reg_req_t(reg_req_t),
      .reg_rsp_t(reg_rsp_t)
  ) external_ctrl_i (
      .clk_i(system_clk_o),
      .rst_ni,
      .mem_sw_fb_i(heepocrates_ctrl_mem_sw_fb_i),
      .cgra_mem_sw_fb_i(heepocrates_ctrl_cgra_mem_sw_fb_i),
      .coubertin_sw_fb_i(heepocrates_ctrl_coubertin_sw_fb_i),
      .reg_req_i(heepocrates_ctrl_slave_req_i),
      .reg_rsp_o(heepocrates_ctrl_slave_resp_o),
      .cgra_enable_o(cgra_enable)
  );

endmodule : external_peripheral
