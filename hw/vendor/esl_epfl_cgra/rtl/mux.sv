////////////////////////////////////////////////////////////////////////////////
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                                                                            //
//                                                                            //
// Design Name:    MUX                                                        //
// Project Name:   CGRA                                                       //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    Configurable multiplexer.                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module mux
  import cgra_pkg::*;
#(
  parameter MUX_NUM_INPUTS   = 8,
  parameter MUX_INPUTS_WIDTH = 32,
  parameter MUX_NSEL         = $clog2(MUX_NUM_INPUTS)
)
(
  input  logic [MUX_INPUTS_WIDTH-1:0] mux_inputs [0:MUX_NUM_INPUTS-1],
  input  logic [        MUX_NSEL-1:0] mux_sel,
  output logic [MUX_INPUTS_WIDTH-1:0] mux_o
);

  logic [MUX_INPUTS_WIDTH-1:0] mux_res;

  assign mux_o = mux_res;

  always_comb
  begin
    mux_res = '0;

    for (int i=0; i<MUX_NUM_INPUTS; i++) begin
      if (i[MUX_NSEL-1:0] == mux_sel) begin
        mux_res = mux_res | mux_inputs[i];
      end
    end
  end

endmodule
