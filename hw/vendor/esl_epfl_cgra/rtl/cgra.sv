////////////////////////////////////////////////////////////////////////////////
// Author:         Benoît Denkinger - benoit.denkinger@epfl.ch                //
//                                                                            //
// Additional contributions by:                                               //
//                                                                            //
//                                                                            //
// Design Name:    CGRA                                                       //
// Project Name:   CGRA                                                       //
// Language:       SystemVerilog                                              //
//                                                                            //
// Description:    CGRA RCs mesh.                                             //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

module cgra
  import cgra_pkg::*;
(
  input  logic [            N_COL-1:0] clk_i,
  input  logic [            N_COL-1:0] rst_col_i,
  input  logic [            N_COL-1:0] rcs_col_e_i,
  input  logic [            N_COL-1:0] rcs_conf_we_i,
  input  logic [            N_COL-1:0] rcs_conf_re_i,
  input  logic [RCS_NUM_CREG_LOG2-1:0] rcs_col_pc_i [0:N_COL-1],
  input  logic [      INSTR_WIDTH-1:0] rcs_conf_words_i [0:N_ROW-1],
  input  logic [            N_COL-1:0] rcs_pc_e_i,
  input  logic [         DP_WIDTH-1:0] data_rdata_i [0:N_COL-1],
  input  logic [            N_COL-1:0] data_gnt_i,
  input  logic [            N_COL-1:0] data_rvalid_i,
  input  logic [            N_COL-1:0] col_acc_map_i [0:N_COL-1],
  output logic [            N_COL-1:0] data_req_o,
  output logic [            N_COL-1:0] data_wen_o,
  output logic [            N_COL-1:0] data_ind_o,
  output logic [         DP_WIDTH-1:0] data_add_o [0:N_COL-1],
  output logic [         DP_WIDTH-1:0] data_wdata_o [0:N_COL-1],
  output logic [            N_COL-1:0] rcs_br_req_o,
  output logic [RCS_NUM_CREG_LOG2-1:0] rcs_br_add_o [0:N_COL-1],
  output logic [            N_COL-1:0] rcs_stall_o,
  output logic [            N_COL-1:0] exec_end_o
);

  logic [   N_COL-1:0] rcs_ex_end [0:N_ROW-1];
  logic [   N_COL-1:0] rcs_stall_s [0:N_ROW-1];
  logic [   N_COL-1:0] rcs_nop_s [0:N_ROW-1];
  logic [   N_COL-1:0] rc_stall_col;
  logic [   N_ROW-1:0] rcs_br_req_row_s [0:N_COL-1];
  logic [   N_COL-1:0] rcs_br_req_row_merged_s;
  logic [   N_COL-1:0] rcs_br_req_col_merged_s [0:N_COL-1];
  logic [   N_COL-1:0] rcs_exec_end_col_merged;
  logic [   N_COL-1:0] rc_stall_comb;
  logic [   N_COL-1:0] rcs_br_req [0:N_ROW-1];
  logic [   N_COL-1:0] exec_end_s;
  logic [   N_COL-1:0] data_req_s [0:N_ROW-1];
  logic [   N_COL-1:0] data_wen_s [0:N_ROW-1];
  logic [   N_COL-1:0] data_ind_s [0:N_ROW-1];
  logic [DP_WIDTH-1:0] data_add_s [0:N_ROW-1][0:N_COL-1];
  logic [DP_WIDTH-1:0] data_wdata_s [0:N_ROW-1][0:N_COL-1];
  logic [DP_WIDTH-1:0] rcs_wdata_s [0:N_COL-1];

  logic [  DP_WIDTH-1:0] rcs_res [0:N_ROW-1][0:N_COL-1];
  logic [  DP_WIDTH-1:0] rcs_res_reg [0:N_ROW-1][0:N_COL-1];
  logic [ALU_N_FLAG-1:0] rcs_flag [0:N_ROW-1][0:N_COL-1];
  logic [ALU_N_FLAG-1:0] rcs_flag_reg [0:N_ROW-1][0:N_COL-1];

  logic [RCS_NUM_CREG_LOG2-1:0] rcs_br_add [0:N_ROW-1][0:N_COL-1];

  logic [   N_ROW-1:0] data_req_gnt_mask [0:N_COL-1];
  logic [   N_ROW-1:0] gnt_demux [0:N_COL-1];
  logic [   N_ROW-1:0] data_req_rvalid_mask [0:N_COL-1];
  logic [   N_ROW-1:0] rvalid_demux [0:N_COL-1];
  logic [   N_ROW-1:0] gnt_mask [0:N_COL-1];
  logic [   N_ROW-1:0] rvalid_mask [0:N_COL-1];

  logic [-1:N_COL][  DP_WIDTH-1:0] rcs_mesh_res [-1:N_ROW];
  logic [-1:N_COL][ALU_N_FLAG-1:0] rcs_mesh_flag [-1:N_ROW];

  // i,k are usually for the rows and j,l for the columns
  genvar i,j;

  assign rcs_stall_o = rc_stall_comb;
  assign exec_end_o = exec_end_s;

  assign data_wdata_o = rcs_wdata_s;

  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      // Merge rows branch request
      rcs_br_req_row_s[l] = {rcs_br_req[3][l], rcs_br_req[2][l], rcs_br_req[1][l], rcs_br_req[0][l]};
      rcs_br_req_row_merged_s[l] = rcs_br_req[3][l] | rcs_br_req[2][l] | rcs_br_req[1][l] | rcs_br_req[0][l];
      // branch cols branch request for multi-cols kernels
      rcs_br_req_col_merged_s[l] = rcs_br_req_row_merged_s & col_acc_map_i[l];
      // Capture execution end signal
      rcs_exec_end_col_merged[l] = (rcs_ex_end[3][l] | rcs_ex_end[2][l] | rcs_ex_end[1][l] | rcs_ex_end[0][l]) & ~rcs_br_req_o[l];
      // RCs stall capture
      rc_stall_col[l]  = rcs_stall_s[3][l] | rcs_stall_s[2][l] | rcs_stall_s[1][l] | rcs_stall_s[0][l];
    end
  end

  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      // Combine the stall for multi-columns kernel
      rc_stall_comb[l] = |(rc_stall_col & col_acc_map_i[l]);
    end
  end

  // For some unclear reason this has to be separated from rcs_exec_end_col_merged block otherwise the logic produced is not purely combinational and there is a delay
  // for exec_end signal for multi-column kernels.
  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      exec_end_s[l]      = |(rcs_exec_end_col_merged & col_acc_map_i[l]);
    end
  end

  // Maintain request high as long as one RC is not served
  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      data_req_o[l] = data_req_gnt_mask[l][0] | data_req_gnt_mask[l][1] | data_req_gnt_mask[l][2] | data_req_gnt_mask[l][3];
    end
  end

  // Select which request to grant
  generate // generate is used to create separated statement blocks
    for (j=0; j<N_COL; j++) begin : gnt_demux_gen
      always_comb
      begin
        gnt_demux[j] = '0;
        // for each row
        if (data_req_gnt_mask[j][0] == 1'b1 && data_gnt_i[j] == 1'b1) begin
          gnt_demux[j][0] = 1'b1;
        end else if (data_req_gnt_mask[j][1] == 1'b1 && data_gnt_i[j] == 1'b1) begin
          gnt_demux[j][1] = 1'b1;
        end else if (data_req_gnt_mask[j][2] == 1'b1 && data_gnt_i[j] == 1'b1) begin
          gnt_demux[j][2] = 1'b1;
        end else if (data_req_gnt_mask[j][3] == 1'b1 && data_gnt_i[j] == 1'b1) begin
          gnt_demux[j][3] = 1'b1;
        end
      end
    end : gnt_demux_gen
  endgenerate

  // Select which request to forward rvalid (only for read wen=1)
  generate // generate is used to create separated statement blocks
    for (j=0; j<N_COL; j++) begin : rvalid_demux_gen
      always_comb
      begin
        rvalid_demux[j] = '0;
        // for each row
        if (data_req_rvalid_mask[j][0] == 1'b1 && data_wen_s[0][j] == 1'b1 && data_rvalid_i[j] == 1'b1) begin
          rvalid_demux[j][0] = 1'b1;
        end else if (data_req_rvalid_mask[j][1] == 1'b1 && data_wen_s[1][j] == 1'b1 && data_rvalid_i[j] == 1'b1) begin
          rvalid_demux[j][1] = 1'b1;
        end else if (data_req_rvalid_mask[j][2] == 1'b1 && data_wen_s[2][j] == 1'b1 && data_rvalid_i[j] == 1'b1) begin
          rvalid_demux[j][2] = 1'b1;
        end else if (data_req_rvalid_mask[j][3] == 1'b1 && data_wen_s[3][j] == 1'b1 && data_rvalid_i[j] == 1'b1) begin
          rvalid_demux[j][3] = 1'b1;
        end
      end
    end : rvalid_demux_gen
  endgenerate

  // Mask data request of an RC once it is granted
  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      for (int m=0; m<N_ROW; m++) begin
        // mask the request once it is granted
        data_req_gnt_mask[l][m] = data_req_s[m][l] & gnt_mask[l][m];
      end
    end
  end

  // Mask data request of an RC once data is read
  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      for (int m=0; m<N_ROW; m++) begin
        // mask the request once data is read
        data_req_rvalid_mask[l][m] = data_req_s[m][l] & rvalid_mask[l][m];
      end
    end
  end

  // Generate gnt mask
  generate // generate is used to create separated statement blocks
    for (j=0; j<N_COL; j++) begin : gnt_mask_gen
      always_ff @(posedge clk_i[j])
      begin
        if (rst_col_i[j] == 1'b1 || rcs_pc_e_i[j] == 1'b1) begin // reset at start and every new instruction
          for (int k=0; k<N_ROW; k++) begin
            gnt_mask[j][k] <= 1'b1;
          end
        end else begin
          for (int k=0; k<N_ROW; k++) begin
            if (gnt_demux[j][k] == 1'b1) begin
              gnt_mask[j][k] <= 1'b0;
            end
          end
        end
      end
    end : gnt_mask_gen
  endgenerate

  // Generate rvalid mask
  generate // generate is used to create separated statement blocks
    for (j=0; j<N_COL; j++) begin : rvalid_mask_gen
      always_ff @(posedge clk_i[j])
      begin
        if (rst_col_i[j] == 1'b1 || rcs_pc_e_i[j] == 1'b1) begin // reset at start and every new instruction
          for (int k=0; k<N_ROW; k++) begin
            rvalid_mask[j][k] <= 1'b1;
          end
        end else begin
          for (int k=0; k<N_ROW; k++) begin
            if (rvalid_demux[j][k] == 1'b1) begin
              rvalid_mask[j][k] <= 1'b0;
            end
          end
        end
      end
    end : rvalid_mask_gen
  endgenerate

  // Register between RCs
  generate // generate is used to create separated statement blocks
    for (j=0; j<N_COL; j++) begin : reg_gen
      always_ff @(posedge clk_i[j])
      begin
        if (rst_col_i[j] == 1'b1) begin
          for (int k=0; k<N_ROW; k++) begin
            rcs_res_reg[k][j]  <= '0;
            rcs_flag_reg[k][j] <= '0;
          end
        end else begin
          for (int k=0; k<N_ROW; k++) begin
            // If execution is resumed and this RC was not requesting data, update its register
            if (rcs_pc_e_i[j] == 1'b1 && data_req_s[k][j] == 1'b0) begin
              // Update output registers only if rc is active (i.e., not executing a nop)
              if (rcs_nop_s[k][j] == 1'b0) begin
                rcs_res_reg[k][j]  <= rcs_res[k][j];
                rcs_flag_reg[k][j] <= rcs_flag[k][j];
              end
            // If data is requested, update its register as soon as data is valid (otherwise only last data read is passed)
            end else if (rvalid_demux[j][k] == 1'b1) begin
              // Bypass the RC otherwise rdata goes through the ALU
              rcs_res_reg[k][j]  <= data_rdata_i[j];
              rcs_flag_reg[k][j] <= {data_rdata_i[j][DP_WIDTH-1], ~(|data_rdata_i[j])};
            end
          end
        end
      end
    end
  endgenerate

  generate // generate is used to create separated statement blocks
    for (j=0; j<N_COL; j++) begin : data_req_gen
      always_comb
      begin
        // for each row
        if (data_req_gnt_mask[j][0] == 1'b1 && data_ind_s[0][j] == 1'b1) begin
          data_add_o[j] = data_add_s[0][j];
        end else if (data_req_gnt_mask[j][1] == 1'b1 && data_ind_s[1][j] == 1'b1) begin
          data_add_o[j] = data_add_s[1][j];
        end else if (data_req_gnt_mask[j][2] == 1'b1 && data_ind_s[2][j] == 1'b1) begin
          data_add_o[j] = data_add_s[2][j];
        end else if (data_req_gnt_mask[j][3] == 1'b1 && data_ind_s[3][j] == 1'b1) begin
          data_add_o[j] = data_add_s[3][j];
        end else begin
          data_add_o[j] = '0;
        end
      end

      always_comb
      begin
        // for each row ...
        if (data_req_gnt_mask[j][0] == 1'b1 && data_wen_s[0][j] == 1'b0) begin
          rcs_wdata_s[j] = data_wdata_s[0][j];
        end else if (data_req_gnt_mask[j][1] == 1'b1 && data_wen_s[1][j] == 1'b0) begin
          rcs_wdata_s[j] = data_wdata_s[1][j];
        end else if (data_req_gnt_mask[j][2] == 1'b1 && data_wen_s[2][j] == 1'b0) begin
          rcs_wdata_s[j] = data_wdata_s[2][j];
        end else if (data_req_gnt_mask[j][3] == 1'b1 && data_wen_s[3][j] == 1'b0) begin
          rcs_wdata_s[j] = data_wdata_s[3][j];
        end else begin
          rcs_wdata_s[j] = '0;
        end
      end

      always_comb
      begin
        // for each row ...
        if (data_req_gnt_mask[j][0] == 1'b1) begin
          data_wen_o[j] = data_wen_s[0][j];
          data_ind_o[j] = data_ind_s[0][j];
        end else if (data_req_gnt_mask[j][1] == 1'b1) begin
          data_wen_o[j] = data_wen_s[1][j];
          data_ind_o[j] = data_ind_s[1][j];
        end else if (data_req_gnt_mask[j][2] == 1'b1) begin
          data_wen_o[j] = data_wen_s[2][j];
          data_ind_o[j] = data_ind_s[2][j];
        end else if (data_req_gnt_mask[j][3] == 1'b1) begin
          data_wen_o[j] = data_wen_s[3][j];
          data_ind_o[j] = data_ind_s[3][j];
        end else begin
          data_wen_o[j] = '0;
          data_ind_o[j] = '0;
        end
      end
    end
  endgenerate

  // Branch request
  always_comb
  begin
    for (int l=0; l<N_COL; l++) begin
      
      rcs_br_req_o[l]  = 1'b0;
      rcs_br_add_o[l] = '0;

      if (rcs_br_req_col_merged_s[l] == 4'b0001) begin
        if (rcs_br_req_row_s[0] == 4'b0001) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[0][0];
        end else if (rcs_br_req_row_s[0] == 4'b0010) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[1][0];
        end else if (rcs_br_req_row_s[0] == 4'b0100) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[2][0];
        end else if (rcs_br_req_row_s[0] == 4'b1000) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[3][0];
        end

      end else if (rcs_br_req_col_merged_s[l] == 4'b0010) begin
        if (rcs_br_req_row_s[1] == 4'b0001) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[0][1];
        end else if (rcs_br_req_row_s[1] == 4'b0010) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[1][1];
        end else if (rcs_br_req_row_s[1] == 4'b0100) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[2][1];
        end else if (rcs_br_req_row_s[1] == 4'b1000) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[3][1];
        end

      end else if (rcs_br_req_col_merged_s[l] == 4'b0100) begin
        if (rcs_br_req_row_s[2] == 4'b0001) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[0][2];
        end else if (rcs_br_req_row_s[2] == 4'b0010) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[1][2];
        end else if (rcs_br_req_row_s[2] == 4'b0100) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[2][2];
        end else if (rcs_br_req_row_s[2] == 4'b1000) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[3][2];
        end

      end else if (rcs_br_req_col_merged_s[l] == 4'b1000) begin
        if (rcs_br_req_row_s[3] == 4'b0001) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[0][3];
        end else if (rcs_br_req_row_s[3] == 4'b0010) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[1][3];
        end else if (rcs_br_req_row_s[3] == 4'b0100) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[2][3];
        end else if (rcs_br_req_row_s[3] == 4'b1000) begin
          rcs_br_req_o[l]  = 1'b1;
          rcs_br_add_o[l] = rcs_br_add[3][3];
        end
      end
    end
  end

  //---------------------------------------------------------------------
  //
  // CGRA torus connection array use to easily connect all cells
  //
  //---------------------------------------------------------------------

  always_comb
  begin
    // RCs data result connections
    for (int k=0; k<N_ROW; k++) begin
      for (int l=0; l<N_COL; l++) begin
        rcs_mesh_res[k][l] = rcs_res_reg[k][l];
      end
    end
    for (int k=0; k<N_ROW; k++) begin
      rcs_mesh_res[k][-1] = rcs_res_reg[k][N_COL-1];
      rcs_mesh_res[k][N_COL] = rcs_res_reg[k][0];
    end
    for (int l=0; l<N_COL; l++) begin
      rcs_mesh_res[-1][l] = rcs_res_reg[N_ROW-1][l];
      rcs_mesh_res[N_ROW][l] = rcs_res_reg[0][l];
    end

    // RCs flag result connections
    for (int k=0; k<N_ROW; k++) begin
      for (int l=0; l<N_COL; l++) begin
        rcs_mesh_flag[k][l] = rcs_flag_reg[k][l];
      end
    end
    for (int k=0; k<N_ROW; k++) begin
      rcs_mesh_flag[k][-1] = rcs_flag_reg[k][N_COL-1];
      rcs_mesh_flag[k][N_COL] = rcs_flag_reg[k][0];
    end
    for (int l=0; l<N_COL; l++) begin
      rcs_mesh_flag[-1][l] = rcs_flag_reg[N_ROW-1][l];
      rcs_mesh_flag[N_ROW][l] = rcs_flag_reg[0][l];
    end
  end

  //---------------------------------------------------------------------
  //
  // Components mapping
  //
  //---------------------------------------------------------------------

  //  N_ROW x N_COL        col_0          col_1         col_2           col_3
  //
  //  RC1 / row_0      LTRC(0,0) ---- TRC (0,0) ---- TRC (0,0) ---- RTRC(0,0)
  //                       |              |              |              |
  //  RC2 / row_1      LRC (0,0) ---- CRC (0,0) ---- CRC (0,0) ---- RRC (0,0)
  //                       |              |              |              | 
  //  RC3 / row_2      LRC (0,0) ---- CRC (0,0) ---- CRC (0,0) ---- RRC (0,0)
  //                       |              |              |              | 
  //  RC4 / row_3      LBRC(0,0) ---- BRC (0,0) ---- BRC (0,0) ---- RBRC(0,0)

  generate
    for (i=0; i<N_ROW; i++) begin : rc_row_gen
      for (j=0; j<N_COL; j++) begin : rc_col_gen
        reconfigurable_cell rc_i (
          //                               [ROW][COL]
          .clk_i         (            clk_i     [j  ] ),
          .rst_rc_i      (       rst_col_i      [j  ] ),
          .ce_i          (     rcs_col_e_i      [j  ] ),
          .conf_rdata_i  ( rcs_conf_words_i[i  ]      ),
          .data_rdata_i  (     data_rdata_i     [j  ] ),
          .data_rvalid_i (     rvalid_demux[j  ][i  ] ),
          .conf_we_i     (    rcs_conf_we_i     [j  ] ),
          .conf_re_i     (    rcs_conf_re_i     [j  ] ),
          .global_pc_i   (     rcs_col_pc_i     [j  ] ),
          .pc_en_i       (       rcs_pc_e_i     [j  ] ),
          .own_res_i     (     rcs_mesh_res[i  ][j  ] ),
          .left_res_i    (     rcs_mesh_res[i  ][j-1] ),
          .right_res_i   (     rcs_mesh_res[i  ][j+1] ),
          .top_res_i     (     rcs_mesh_res[i-1][j  ] ),
          .bottom_res_i  (     rcs_mesh_res[i+1][j  ] ),
          .own_flag_i    (    rcs_mesh_flag[i  ][j  ] ),
          .left_flag_i   (    rcs_mesh_flag[i  ][j-1] ),
          .right_flag_i  (    rcs_mesh_flag[i  ][j+1] ),
          .top_flag_i    (    rcs_mesh_flag[i-1][j  ] ),
          .bottom_flag_i (    rcs_mesh_flag[i+1][j  ] ),
          .result_o      (          rcs_res[i  ][j  ] ),
          .flag_o        (         rcs_flag[i  ][j  ] ),
          .br_req_o      (       rcs_br_req[i  ][j  ] ),
          .br_add_o      (       rcs_br_add[i  ][j  ] ),
          .data_req_o    (       data_req_s[i  ][j  ] ),
          .data_wen_o    (       data_wen_s[i  ][j  ] ),
          .data_ind_o    (       data_ind_s[i  ][j  ] ),
          .data_add_o    (       data_add_s[i  ][j  ] ),
          .data_wdata_o  (     data_wdata_s[i  ][j  ] ),
          .rc_stall_o    (      rcs_stall_s[i  ][j  ] ),
          .rc_nop_o      (        rcs_nop_s[i  ][j  ] ),
          .exec_end_o    (       rcs_ex_end[i  ][j  ] )
        );
      end
    end
  endgenerate

endmodule
