////////////////////////////////////////////////////////////////////////////////
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                                                                            //
//                                                                            //
// Design Name:    CONFUGRATION MEMORY                                        //
// Project Name:   CGRA                                                       //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    Configuration memory holding the kernels using SCM.        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module context_memory
  import cgra_pkg::*;
(
  input  logic                           clk_i,
  input  logic                           rst_ni,
  input  logic                           cm_cg_i,
  input  logic                           cm_set_retentive_i,
  input  logic [              N_ROW-1:0] cm_row_req_i,
  input  logic                           cm_we_i,
  input  logic [  IMEM_N_LINES_LOG2-1:0] cm_addr_i,
  input  logic [DATA_BUS_DATA_WIDTH-1:0] cm_wdata_i,
  output logic [        INSTR_WIDTH-1:0] rcs_cmem_rdata_o [0:N_ROW-1]
);


  // CGRA context memories (one per row)
  cgra_sram_wrapper #(
    .NumWords (IMEM_N_LINES),
    .DataWidth(32'd32)
  ) cgra_ram0_i (
    .clk_i  (cm_cg_i),
    .rst_ni (rst_ni),
    .req_i  (cm_row_req_i[0]),
    .we_i   (cm_we_i),
    .addr_i (cm_addr_i),
    .wdata_i(cm_wdata_i),
    .be_i   (4'b1111),
    .set_retentive_i (cm_set_retentive_i),
    // output ports
    .rdata_o(rcs_cmem_rdata_o[0])
  );

  cgra_sram_wrapper #(
    .NumWords (IMEM_N_LINES),
    .DataWidth(32'd32)
  ) cgra_ram1_i (
    .clk_i  (cm_cg_i),
    .rst_ni (rst_ni),
    .req_i  (cm_row_req_i[1]),
    .we_i   (cm_we_i),
    .addr_i (cm_addr_i),
    .wdata_i(cm_wdata_i),
    .be_i   (4'b1111),
    .set_retentive_i (cm_set_retentive_i),
    // output ports
    .rdata_o(rcs_cmem_rdata_o[1])
  );

  cgra_sram_wrapper #(
    .NumWords (IMEM_N_LINES),
    .DataWidth(32'd32)
  ) cgra_ram2_i (
    .clk_i  (cm_cg_i),
    .rst_ni (rst_ni),
    .req_i  (cm_row_req_i[2]),
    .we_i   (cm_we_i),
    .addr_i (cm_addr_i),
    .wdata_i(cm_wdata_i),
    .be_i   (4'b1111),
    .set_retentive_i (cm_set_retentive_i),
    // output ports
    .rdata_o(rcs_cmem_rdata_o[2])
  );

  cgra_sram_wrapper #(
    .NumWords (IMEM_N_LINES),
    .DataWidth(32'd32)
  ) cgra_ram3_i (
    .clk_i  (cm_cg_i),
    .rst_ni (rst_ni),
    .req_i  (cm_row_req_i[3]),
    .we_i   (cm_we_i),
    .addr_i (cm_addr_i),
    .wdata_i(cm_wdata_i),
    .be_i   (4'b1111),
    .set_retentive_i (cm_set_retentive_i),
    // output ports
    .rdata_o(rcs_cmem_rdata_o[3])
  );
    
endmodule
