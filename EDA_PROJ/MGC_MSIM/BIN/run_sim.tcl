exec rm -fr DLIB/WORK
vlib DLIB/WORK
vmap work DLIB/WORK


vlog  -work work HDL/RTL/cgra_top.sv +libext+.sv -y HDL/RTL -v HDL/RTL/cgra_reg_pkg.sv 
vlog  -work work HDL/TBENCH/cgra_top_wrapper.sv 

vsim cgra_top

vcd file SA/dump.vcd

vcd add cgra_top_wrapper/cgra_top_i/*

vcd off SA/dump.vcd

run 2ns

vcd on SA/dump.vcd

run 5ms

quit

