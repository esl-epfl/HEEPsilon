log -r tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/* -nofilter Memory

onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/clk_i
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/rst_ni
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/core_instr_req_o
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/core_instr_resp_i
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/core_data_req_o
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/core_data_resp_i
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/irq_i
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/irq_ack_o
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/irq_id_o
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/debug_req_i
add wave -noupdate -group CPU /tb_top/testharness_i/cgra_x_heep_top_i/x_heep_system_i/core_v_mini_mcu_i/cpu_subsystem_i/core_sleep_o
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/clk_i
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/rst_ni
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/rst_logic_ni
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_enable_i
add wave -noupdate -group CGRA_TOP_WRAPPER -subitemconfig {{/tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/masters_req_o[1]} -expand {/tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/masters_req_o[0]} -expand} /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/masters_req_o
add wave -noupdate -group CGRA_TOP_WRAPPER -subitemconfig {{/tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/masters_resp_i[1]} -expand {/tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/masters_resp_i[0]} -expand} /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/masters_resp_i
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/reg_req_i
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/reg_rsp_o
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/slave_req_i
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/slave_resp_o
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cmem_set_retentive_i
add wave -noupdate -group CGRA_TOP_WRAPPER /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_int_o
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/clk_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/rst_ni
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/cm_cg_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/cm_set_retentive_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/cm_row_req_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/cm_we_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/cm_addr_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/cm_wdata_i
add wave -noupdate -group CGRA_CONTEXT_MEM /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_context_memory_i/rcs_cmem_rdata_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/clk_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rst_ni
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/acc_req_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/imem_gnt_ctrl_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/imem_rvalid_ctrl_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/kmem_rdata_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/ker_id_req_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/data_stall_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_br_req_i
add wave -noupdate -group CGRA_CTRL -expand /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_br_add_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_stall_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_exec_end_i
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_conf_we_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_conf_re_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_pc_e_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_pc_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/col_e_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_rst_col_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_conf_ack_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/imem_radd_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/rcs_conf_req_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/col_start_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/acc_ack_o
add wave -noupdate -group CGRA_CTRL /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_ctrl_i/acc_end_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/clk_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rst_col_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_conf_we_i
add wave -noupdate -group CGRA_RCS -expand /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_conf_re_i
add wave -noupdate -group CGRA_RCS -expand /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_col_pc_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_conf_words_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_pc_e_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_rdata_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_gnt_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_rvalid_i
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_req_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_wen_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_ind_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_add_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_wdata_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_br_add_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_stall_o
add wave -noupdate -group CGRA_RCS -expand /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/exec_end_o
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_stall_s
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_nop_s
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rc_stall_col
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_req_gnt_mask
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/data_req_rvalid_mask
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rvalid_demux
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/gnt_mask
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rvalid_mask
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_res
add wave -noupdate -group CGRA_RCS /tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rcs_res_reg
add wave -noupdate -group RC_2_1 -expand {/tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rc_row_gen[2]/rc_col_gen[1]/rc_i/rc_datapath/reg_file_i/regs_o}
add wave -noupdate -group RC_3_1 -expand {/tb_top/testharness_i/cgra_x_heep_top_i/cgra_top_wrapper_i/cgra_top_i/cgra_rcs_i/rc_row_gen[3]/rc_col_gen[1]/rc_i/rc_datapath/reg_file_i/regs_o}
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {4943608936 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 296
configure wave -valuecolwidth 100
configure wave -justifyvalue left
configure wave -signalnamewidth 1
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {0 ps} {10091781 ns}
