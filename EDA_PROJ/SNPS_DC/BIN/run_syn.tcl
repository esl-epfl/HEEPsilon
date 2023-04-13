set power_preserve_rtl_hier_names true


# First synthesis
read_file -format systemverilog { HDL/RTL/cgra_top.sv }

current_design adder

# Constraints
create_clock -period 1 -name desing_clk clk 
set_max_area 0 

# set port constraints
# set_logic_zero {in_a[0] in_a[1]}
# set_logic_one
# set_logic_dc

#compile_ultra
compile -map_effort medium

ungroup -all -flatten 
write -format vhdl -output RPT/cgra_top_compiled.vhd 
write -format ddc -output RPT/cgra_top_compiled.ddc

read_file -format ddc RPT/cgra_top_compiled.ddc

current_design cgra_top_compiled.ddc:cgra_top


report_area > RPT/area_compiled.rpt
report_timing > RPT/timing_compiled.rpt
report_power > RPT/power_compiled.rpt

# optimised design
read_saif -input SA/dump.saif -instance_name cgra_top_wrapper/cgra_top_i

#set_max_leakage_power 0 mw
#set_max_dynamic_power 0 mw
#set_max_total_power 0 mw

set_leakage_optimization true
set_dynamic_optimization true

create_clock -period 1 -name desing_clk clk
set_max_area 0

compile -incremental

write -format ddc -output RPT/cgra_top_optimized.ddc
write -format vhdl -output RPT/cgra_top_optimized.sv

report_power > RPT/power_optimized.rpt
report_area > RPT/area_optimized.rpt
report_timing > RPT/timing_optimized.rpt


quit
