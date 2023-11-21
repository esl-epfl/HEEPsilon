// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// Register Top module auto-generated by `reggen`


`include "common_cells/assertions.svh"

module cgra_reg_top #(
    parameter type reg_req_t = logic,
    parameter type reg_rsp_t = logic,
    parameter int AW = 7
) (
  input clk_i,
  input rst_ni,
  input  reg_req_t reg_req_i,
  output reg_rsp_t reg_rsp_o,
  // To HW
  output cgra_reg_pkg::cgra_reg2hw_t reg2hw, // Write
  input  cgra_reg_pkg::cgra_hw2reg_t hw2reg, // Read


  // Config
  input devmode_i // If 1, explicit error return for unmapped register access
);

  import cgra_reg_pkg::* ;

  localparam int DW = 32;
  localparam int DBW = DW/8;                    // Byte Width

  // register signals
  logic           reg_we;
  logic           reg_re;
  logic [AW-1:0]  reg_addr;
  logic [DW-1:0]  reg_wdata;
  logic [DBW-1:0] reg_be;
  logic [DW-1:0]  reg_rdata;
  logic           reg_error;

  logic          addrmiss, wr_err;

  logic [DW-1:0] reg_rdata_next;

  // Below register interface can be changed
  reg_req_t  reg_intf_req;
  reg_rsp_t  reg_intf_rsp;


  assign reg_intf_req = reg_req_i;
  assign reg_rsp_o = reg_intf_rsp;


  assign reg_we = reg_intf_req.valid & reg_intf_req.write;
  assign reg_re = reg_intf_req.valid & ~reg_intf_req.write;
  assign reg_addr = reg_intf_req.addr;
  assign reg_wdata = reg_intf_req.wdata;
  assign reg_be = reg_intf_req.wstrb;
  assign reg_intf_rsp.rdata = reg_rdata;
  assign reg_intf_rsp.error = reg_error;
  assign reg_intf_rsp.ready = 1'b1;

  assign reg_rdata = reg_rdata_next ;
  assign reg_error = (devmode_i & addrmiss) | wr_err;


  // Define SW related signals
  // Format: <reg>_<field>_{wd|we|qs}
  //        or <reg>_{wd|we|qs} if field == 1 or 0
  logic [3:0] col_status_qs;
  logic [31:0] kernel_id_qs;
  logic [31:0] kernel_id_wd;
  logic kernel_id_we;
  logic [31:0] ptr_in_col_0_qs;
  logic [31:0] ptr_in_col_0_wd;
  logic ptr_in_col_0_we;
  logic [31:0] ptr_out_col_0_qs;
  logic [31:0] ptr_out_col_0_wd;
  logic ptr_out_col_0_we;
  logic [31:0] ptr_in_col_1_qs;
  logic [31:0] ptr_in_col_1_wd;
  logic ptr_in_col_1_we;
  logic [31:0] ptr_out_col_1_qs;
  logic [31:0] ptr_out_col_1_wd;
  logic ptr_out_col_1_we;
  logic [31:0] ptr_in_col_2_qs;
  logic [31:0] ptr_in_col_2_wd;
  logic ptr_in_col_2_we;
  logic [31:0] ptr_out_col_2_qs;
  logic [31:0] ptr_out_col_2_wd;
  logic ptr_out_col_2_we;
  logic [31:0] ptr_in_col_3_qs;
  logic [31:0] ptr_in_col_3_wd;
  logic ptr_in_col_3_we;
  logic [31:0] ptr_out_col_3_qs;
  logic [31:0] ptr_out_col_3_wd;
  logic ptr_out_col_3_we;
  logic perf_cnt_enable_qs;
  logic perf_cnt_enable_wd;
  logic perf_cnt_enable_we;
  logic perf_cnt_reset_qs;
  logic perf_cnt_reset_wd;
  logic perf_cnt_reset_we;
  logic [31:0] perf_cnt_total_kernels_qs;
  logic [31:0] perf_cnt_total_kernels_wd;
  logic perf_cnt_total_kernels_we;
  logic [31:0] perf_cnt_col_0_active_cycles_qs;
  logic [31:0] perf_cnt_col_0_active_cycles_wd;
  logic perf_cnt_col_0_active_cycles_we;
  logic [31:0] perf_cnt_col_0_stall_cycles_qs;
  logic [31:0] perf_cnt_col_0_stall_cycles_wd;
  logic perf_cnt_col_0_stall_cycles_we;
  logic [31:0] perf_cnt_col_1_active_cycles_qs;
  logic [31:0] perf_cnt_col_1_active_cycles_wd;
  logic perf_cnt_col_1_active_cycles_we;
  logic [31:0] perf_cnt_col_1_stall_cycles_qs;
  logic [31:0] perf_cnt_col_1_stall_cycles_wd;
  logic perf_cnt_col_1_stall_cycles_we;
  logic [31:0] perf_cnt_col_2_active_cycles_qs;
  logic [31:0] perf_cnt_col_2_active_cycles_wd;
  logic perf_cnt_col_2_active_cycles_we;
  logic [31:0] perf_cnt_col_2_stall_cycles_qs;
  logic [31:0] perf_cnt_col_2_stall_cycles_wd;
  logic perf_cnt_col_2_stall_cycles_we;
  logic [31:0] perf_cnt_col_3_active_cycles_qs;
  logic [31:0] perf_cnt_col_3_active_cycles_wd;
  logic perf_cnt_col_3_active_cycles_we;
  logic [31:0] perf_cnt_col_3_stall_cycles_qs;
  logic [31:0] perf_cnt_col_3_stall_cycles_wd;
  logic perf_cnt_col_3_stall_cycles_we;

  // Register instances
  // R[col_status]: V(False)

  prim_subreg #(
    .DW      (4),
    .SWACCESS("RO"),
    .RESVAL  (4'h0)
  ) u_col_status (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    .we     (1'b0),
    .wd     ('0  ),

    // from internal hardware
    .de     (hw2reg.col_status.de),
    .d      (hw2reg.col_status.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.col_status.q ),

    // to register interface (read)
    .qs     (col_status_qs)
  );


  // R[kernel_id]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_kernel_id (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (kernel_id_we),
    .wd     (kernel_id_wd),

    // from internal hardware
    .de     (hw2reg.kernel_id.de),
    .d      (hw2reg.kernel_id.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.kernel_id.q ),

    // to register interface (read)
    .qs     (kernel_id_qs)
  );


  // R[ptr_in_col_0]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_in_col_0 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_in_col_0_we),
    .wd     (ptr_in_col_0_wd),

    // from internal hardware
    .de     (hw2reg.ptr_in_col_0.de),
    .d      (hw2reg.ptr_in_col_0.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_in_col_0.q ),

    // to register interface (read)
    .qs     (ptr_in_col_0_qs)
  );


  // R[ptr_out_col_0]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_out_col_0 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_out_col_0_we),
    .wd     (ptr_out_col_0_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_out_col_0.q ),

    // to register interface (read)
    .qs     (ptr_out_col_0_qs)
  );


  // R[ptr_in_col_1]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_in_col_1 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_in_col_1_we),
    .wd     (ptr_in_col_1_wd),

    // from internal hardware
    .de     (hw2reg.ptr_in_col_1.de),
    .d      (hw2reg.ptr_in_col_1.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_in_col_1.q ),

    // to register interface (read)
    .qs     (ptr_in_col_1_qs)
  );


  // R[ptr_out_col_1]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_out_col_1 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_out_col_1_we),
    .wd     (ptr_out_col_1_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_out_col_1.q ),

    // to register interface (read)
    .qs     (ptr_out_col_1_qs)
  );


  // R[ptr_in_col_2]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_in_col_2 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_in_col_2_we),
    .wd     (ptr_in_col_2_wd),

    // from internal hardware
    .de     (hw2reg.ptr_in_col_2.de),
    .d      (hw2reg.ptr_in_col_2.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_in_col_2.q ),

    // to register interface (read)
    .qs     (ptr_in_col_2_qs)
  );


  // R[ptr_out_col_2]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_out_col_2 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_out_col_2_we),
    .wd     (ptr_out_col_2_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_out_col_2.q ),

    // to register interface (read)
    .qs     (ptr_out_col_2_qs)
  );


  // R[ptr_in_col_3]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_in_col_3 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_in_col_3_we),
    .wd     (ptr_in_col_3_wd),

    // from internal hardware
    .de     (hw2reg.ptr_in_col_3.de),
    .d      (hw2reg.ptr_in_col_3.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_in_col_3.q ),

    // to register interface (read)
    .qs     (ptr_in_col_3_qs)
  );


  // R[ptr_out_col_3]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_ptr_out_col_3 (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (ptr_out_col_3_we),
    .wd     (ptr_out_col_3_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.ptr_out_col_3.q ),

    // to register interface (read)
    .qs     (ptr_out_col_3_qs)
  );


  // R[perf_cnt_enable]: V(False)

  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h0)
  ) u_perf_cnt_enable (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_enable_we),
    .wd     (perf_cnt_enable_wd),

    // from internal hardware
    .de     (1'b0),
    .d      ('0  ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_enable.q ),

    // to register interface (read)
    .qs     (perf_cnt_enable_qs)
  );


  // R[perf_cnt_reset]: V(False)

  prim_subreg #(
    .DW      (1),
    .SWACCESS("RW"),
    .RESVAL  (1'h0)
  ) u_perf_cnt_reset (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_reset_we),
    .wd     (perf_cnt_reset_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_reset.de),
    .d      (hw2reg.perf_cnt_reset.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_reset.q ),

    // to register interface (read)
    .qs     (perf_cnt_reset_qs)
  );


  // R[perf_cnt_total_kernels]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_total_kernels (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_total_kernels_we),
    .wd     (perf_cnt_total_kernels_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_total_kernels.de),
    .d      (hw2reg.perf_cnt_total_kernels.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_total_kernels.q ),

    // to register interface (read)
    .qs     (perf_cnt_total_kernels_qs)
  );


  // R[perf_cnt_col_0_active_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_0_active_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_0_active_cycles_we),
    .wd     (perf_cnt_col_0_active_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_0_active_cycles.de),
    .d      (hw2reg.perf_cnt_col_0_active_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_0_active_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_0_active_cycles_qs)
  );


  // R[perf_cnt_col_0_stall_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_0_stall_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_0_stall_cycles_we),
    .wd     (perf_cnt_col_0_stall_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_0_stall_cycles.de),
    .d      (hw2reg.perf_cnt_col_0_stall_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_0_stall_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_0_stall_cycles_qs)
  );


  // R[perf_cnt_col_1_active_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_1_active_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_1_active_cycles_we),
    .wd     (perf_cnt_col_1_active_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_1_active_cycles.de),
    .d      (hw2reg.perf_cnt_col_1_active_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_1_active_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_1_active_cycles_qs)
  );


  // R[perf_cnt_col_1_stall_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_1_stall_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_1_stall_cycles_we),
    .wd     (perf_cnt_col_1_stall_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_1_stall_cycles.de),
    .d      (hw2reg.perf_cnt_col_1_stall_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_1_stall_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_1_stall_cycles_qs)
  );


  // R[perf_cnt_col_2_active_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_2_active_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_2_active_cycles_we),
    .wd     (perf_cnt_col_2_active_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_2_active_cycles.de),
    .d      (hw2reg.perf_cnt_col_2_active_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_2_active_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_2_active_cycles_qs)
  );


  // R[perf_cnt_col_2_stall_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_2_stall_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_2_stall_cycles_we),
    .wd     (perf_cnt_col_2_stall_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_2_stall_cycles.de),
    .d      (hw2reg.perf_cnt_col_2_stall_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_2_stall_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_2_stall_cycles_qs)
  );


  // R[perf_cnt_col_3_active_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_3_active_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_3_active_cycles_we),
    .wd     (perf_cnt_col_3_active_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_3_active_cycles.de),
    .d      (hw2reg.perf_cnt_col_3_active_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_3_active_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_3_active_cycles_qs)
  );


  // R[perf_cnt_col_3_stall_cycles]: V(False)

  prim_subreg #(
    .DW      (32),
    .SWACCESS("RW"),
    .RESVAL  (32'h0)
  ) u_perf_cnt_col_3_stall_cycles (
    .clk_i   (clk_i    ),
    .rst_ni  (rst_ni  ),

    // from register interface
    .we     (perf_cnt_col_3_stall_cycles_we),
    .wd     (perf_cnt_col_3_stall_cycles_wd),

    // from internal hardware
    .de     (hw2reg.perf_cnt_col_3_stall_cycles.de),
    .d      (hw2reg.perf_cnt_col_3_stall_cycles.d ),

    // to internal hardware
    .qe     (),
    .q      (reg2hw.perf_cnt_col_3_stall_cycles.q ),

    // to register interface (read)
    .qs     (perf_cnt_col_3_stall_cycles_qs)
  );




  logic [20:0] addr_hit;
  always_comb begin
    addr_hit = '0;
    addr_hit[ 0] = (reg_addr == CGRA_COL_STATUS_OFFSET);
    addr_hit[ 1] = (reg_addr == CGRA_KERNEL_ID_OFFSET);
    addr_hit[ 2] = (reg_addr == CGRA_PTR_IN_COL_0_OFFSET);
    addr_hit[ 3] = (reg_addr == CGRA_PTR_OUT_COL_0_OFFSET);
    addr_hit[ 4] = (reg_addr == CGRA_PTR_IN_COL_1_OFFSET);
    addr_hit[ 5] = (reg_addr == CGRA_PTR_OUT_COL_1_OFFSET);
    addr_hit[ 6] = (reg_addr == CGRA_PTR_IN_COL_2_OFFSET);
    addr_hit[ 7] = (reg_addr == CGRA_PTR_OUT_COL_2_OFFSET);
    addr_hit[ 8] = (reg_addr == CGRA_PTR_IN_COL_3_OFFSET);
    addr_hit[ 9] = (reg_addr == CGRA_PTR_OUT_COL_3_OFFSET);
    addr_hit[10] = (reg_addr == CGRA_PERF_CNT_ENABLE_OFFSET);
    addr_hit[11] = (reg_addr == CGRA_PERF_CNT_RESET_OFFSET);
    addr_hit[12] = (reg_addr == CGRA_PERF_CNT_TOTAL_KERNELS_OFFSET);
    addr_hit[13] = (reg_addr == CGRA_PERF_CNT_COL_0_ACTIVE_CYCLES_OFFSET);
    addr_hit[14] = (reg_addr == CGRA_PERF_CNT_COL_0_STALL_CYCLES_OFFSET);
    addr_hit[15] = (reg_addr == CGRA_PERF_CNT_COL_1_ACTIVE_CYCLES_OFFSET);
    addr_hit[16] = (reg_addr == CGRA_PERF_CNT_COL_1_STALL_CYCLES_OFFSET);
    addr_hit[17] = (reg_addr == CGRA_PERF_CNT_COL_2_ACTIVE_CYCLES_OFFSET);
    addr_hit[18] = (reg_addr == CGRA_PERF_CNT_COL_2_STALL_CYCLES_OFFSET);
    addr_hit[19] = (reg_addr == CGRA_PERF_CNT_COL_3_ACTIVE_CYCLES_OFFSET);
    addr_hit[20] = (reg_addr == CGRA_PERF_CNT_COL_3_STALL_CYCLES_OFFSET);
  end

  assign addrmiss = (reg_re || reg_we) ? ~|addr_hit : 1'b0 ;

  // Check sub-word write is permitted
  always_comb begin
    wr_err = (reg_we &
              ((addr_hit[ 0] & (|(CGRA_PERMIT[ 0] & ~reg_be))) |
               (addr_hit[ 1] & (|(CGRA_PERMIT[ 1] & ~reg_be))) |
               (addr_hit[ 2] & (|(CGRA_PERMIT[ 2] & ~reg_be))) |
               (addr_hit[ 3] & (|(CGRA_PERMIT[ 3] & ~reg_be))) |
               (addr_hit[ 4] & (|(CGRA_PERMIT[ 4] & ~reg_be))) |
               (addr_hit[ 5] & (|(CGRA_PERMIT[ 5] & ~reg_be))) |
               (addr_hit[ 6] & (|(CGRA_PERMIT[ 6] & ~reg_be))) |
               (addr_hit[ 7] & (|(CGRA_PERMIT[ 7] & ~reg_be))) |
               (addr_hit[ 8] & (|(CGRA_PERMIT[ 8] & ~reg_be))) |
               (addr_hit[ 9] & (|(CGRA_PERMIT[ 9] & ~reg_be))) |
               (addr_hit[10] & (|(CGRA_PERMIT[10] & ~reg_be))) |
               (addr_hit[11] & (|(CGRA_PERMIT[11] & ~reg_be))) |
               (addr_hit[12] & (|(CGRA_PERMIT[12] & ~reg_be))) |
               (addr_hit[13] & (|(CGRA_PERMIT[13] & ~reg_be))) |
               (addr_hit[14] & (|(CGRA_PERMIT[14] & ~reg_be))) |
               (addr_hit[15] & (|(CGRA_PERMIT[15] & ~reg_be))) |
               (addr_hit[16] & (|(CGRA_PERMIT[16] & ~reg_be))) |
               (addr_hit[17] & (|(CGRA_PERMIT[17] & ~reg_be))) |
               (addr_hit[18] & (|(CGRA_PERMIT[18] & ~reg_be))) |
               (addr_hit[19] & (|(CGRA_PERMIT[19] & ~reg_be))) |
               (addr_hit[20] & (|(CGRA_PERMIT[20] & ~reg_be)))));
  end

  assign kernel_id_we = addr_hit[1] & reg_we & !reg_error;
  assign kernel_id_wd = reg_wdata[31:0];

  assign ptr_in_col_0_we = addr_hit[2] & reg_we & !reg_error;
  assign ptr_in_col_0_wd = reg_wdata[31:0];

  assign ptr_out_col_0_we = addr_hit[3] & reg_we & !reg_error;
  assign ptr_out_col_0_wd = reg_wdata[31:0];

  assign ptr_in_col_1_we = addr_hit[4] & reg_we & !reg_error;
  assign ptr_in_col_1_wd = reg_wdata[31:0];

  assign ptr_out_col_1_we = addr_hit[5] & reg_we & !reg_error;
  assign ptr_out_col_1_wd = reg_wdata[31:0];

  assign ptr_in_col_2_we = addr_hit[6] & reg_we & !reg_error;
  assign ptr_in_col_2_wd = reg_wdata[31:0];

  assign ptr_out_col_2_we = addr_hit[7] & reg_we & !reg_error;
  assign ptr_out_col_2_wd = reg_wdata[31:0];

  assign ptr_in_col_3_we = addr_hit[8] & reg_we & !reg_error;
  assign ptr_in_col_3_wd = reg_wdata[31:0];

  assign ptr_out_col_3_we = addr_hit[9] & reg_we & !reg_error;
  assign ptr_out_col_3_wd = reg_wdata[31:0];

  assign perf_cnt_enable_we = addr_hit[10] & reg_we & !reg_error;
  assign perf_cnt_enable_wd = reg_wdata[0];

  assign perf_cnt_reset_we = addr_hit[11] & reg_we & !reg_error;
  assign perf_cnt_reset_wd = reg_wdata[0];

  assign perf_cnt_total_kernels_we = addr_hit[12] & reg_we & !reg_error;
  assign perf_cnt_total_kernels_wd = reg_wdata[31:0];

  assign perf_cnt_col_0_active_cycles_we = addr_hit[13] & reg_we & !reg_error;
  assign perf_cnt_col_0_active_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_0_stall_cycles_we = addr_hit[14] & reg_we & !reg_error;
  assign perf_cnt_col_0_stall_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_1_active_cycles_we = addr_hit[15] & reg_we & !reg_error;
  assign perf_cnt_col_1_active_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_1_stall_cycles_we = addr_hit[16] & reg_we & !reg_error;
  assign perf_cnt_col_1_stall_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_2_active_cycles_we = addr_hit[17] & reg_we & !reg_error;
  assign perf_cnt_col_2_active_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_2_stall_cycles_we = addr_hit[18] & reg_we & !reg_error;
  assign perf_cnt_col_2_stall_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_3_active_cycles_we = addr_hit[19] & reg_we & !reg_error;
  assign perf_cnt_col_3_active_cycles_wd = reg_wdata[31:0];

  assign perf_cnt_col_3_stall_cycles_we = addr_hit[20] & reg_we & !reg_error;
  assign perf_cnt_col_3_stall_cycles_wd = reg_wdata[31:0];

  // Read data return
  always_comb begin
    reg_rdata_next = '0;
    unique case (1'b1)
      addr_hit[0]: begin
        reg_rdata_next[3:0] = col_status_qs;
      end

      addr_hit[1]: begin
        reg_rdata_next[31:0] = kernel_id_qs;
      end

      addr_hit[2]: begin
        reg_rdata_next[31:0] = ptr_in_col_0_qs;
      end

      addr_hit[3]: begin
        reg_rdata_next[31:0] = ptr_out_col_0_qs;
      end

      addr_hit[4]: begin
        reg_rdata_next[31:0] = ptr_in_col_1_qs;
      end

      addr_hit[5]: begin
        reg_rdata_next[31:0] = ptr_out_col_1_qs;
      end

      addr_hit[6]: begin
        reg_rdata_next[31:0] = ptr_in_col_2_qs;
      end

      addr_hit[7]: begin
        reg_rdata_next[31:0] = ptr_out_col_2_qs;
      end

      addr_hit[8]: begin
        reg_rdata_next[31:0] = ptr_in_col_3_qs;
      end

      addr_hit[9]: begin
        reg_rdata_next[31:0] = ptr_out_col_3_qs;
      end

      addr_hit[10]: begin
        reg_rdata_next[0] = perf_cnt_enable_qs;
      end

      addr_hit[11]: begin
        reg_rdata_next[0] = perf_cnt_reset_qs;
      end

      addr_hit[12]: begin
        reg_rdata_next[31:0] = perf_cnt_total_kernels_qs;
      end

      addr_hit[13]: begin
        reg_rdata_next[31:0] = perf_cnt_col_0_active_cycles_qs;
      end

      addr_hit[14]: begin
        reg_rdata_next[31:0] = perf_cnt_col_0_stall_cycles_qs;
      end

      addr_hit[15]: begin
        reg_rdata_next[31:0] = perf_cnt_col_1_active_cycles_qs;
      end

      addr_hit[16]: begin
        reg_rdata_next[31:0] = perf_cnt_col_1_stall_cycles_qs;
      end

      addr_hit[17]: begin
        reg_rdata_next[31:0] = perf_cnt_col_2_active_cycles_qs;
      end

      addr_hit[18]: begin
        reg_rdata_next[31:0] = perf_cnt_col_2_stall_cycles_qs;
      end

      addr_hit[19]: begin
        reg_rdata_next[31:0] = perf_cnt_col_3_active_cycles_qs;
      end

      addr_hit[20]: begin
        reg_rdata_next[31:0] = perf_cnt_col_3_stall_cycles_qs;
      end

      default: begin
        reg_rdata_next = '1;
      end
    endcase
  end

  // Unused signal tieoff

  // wdata / byte enable are not always fully used
  // add a blanket unused statement to handle lint waivers
  logic unused_wdata;
  logic unused_be;
  assign unused_wdata = ^reg_wdata;
  assign unused_be = ^reg_be;

  // Assertions for Register Interface
  `ASSERT(en2addrHit, (reg_we || reg_re) |-> $onehot0(addr_hit))

endmodule
