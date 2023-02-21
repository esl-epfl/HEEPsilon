////////////////////////////////////////////////////////////////////////////////
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                                                                            //
//                                                                            //
// Design Name:    PROGRAM_COUNTER                                            //
// Project Name:   CGRA                                                       //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    CGRA controller program counter.                           //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module program_counter
  import cgra_pkg::*;
#(
  parameter CNT_N_BITS = 4
)
(
  input  logic                  clk_i,
  input  logic                  rst_ni,
  input  logic                  restart_i,
  input  logic                  pc_e_i,
  input  logic                  br_req_i,
  input  logic [CNT_N_BITS-1:0] br_add_i,
  output logic [CNT_N_BITS-1:0] pc_o
);

  logic [CNT_N_BITS-1:0] pc_cnt;

  assign pc_o = br_req_i == 1'b0 ? pc_cnt : br_add_i;

  always_ff @(posedge clk_i, negedge rst_ni)
  begin
    if (rst_ni == 1'b0) begin
      pc_cnt <= '0;
    end else begin
      if (restart_i == 1'b1) begin
        pc_cnt <= '0;
      end else if (pc_e_i == 1'b1) begin
        if (br_req_i == 1'b0) begin
          pc_cnt <= pc_cnt + 1;
        end else begin
          pc_cnt <= br_add_i + 1;
        end
      end
  	end
  end

endmodule
