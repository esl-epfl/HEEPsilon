// Copyright 2023 EPFL
// Solderpad Hardware License, Version 2.1, see LICENSE.md for details.
// SPDX-License-Identifier: Apache-2.0 WITH SHL-2.1

module cgra_clock_gate
(
  input  logic clk_i,
  input  logic test_en_i,
  input  logic en_i,
  output logic clk_o
);

    xilinx_clk_gating clk_gate_i (
        .clk_i,
        .en_i,
        .test_en_i,
        .clk_o
    );

endmodule