
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                 Name Surname - email (affiliation if not ESL)              //
//                                                                            //
// Design Name:    clkgate                                                    //
//                                                                            //
// Project Name:   CGRA_ESL_EPFL                                              //
//                                                                            //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    Wrapper clock-gating unit.                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module cgra_clock_gate
(
  input  logic clk_i,
  input  logic test_en_i,
  input  logic en_i,
  output logic clk_o
);

  // For synthesis replace with library clock-gating cells
  logic clk_en;

  always_latch
  begin
    if (clk_i == 1'b0)
      clk_en <= en_i | test_en_i;
  end

  assign clk_o = clk_i & clk_en;

endmodule
