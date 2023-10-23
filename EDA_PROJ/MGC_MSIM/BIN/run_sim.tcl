exec rm -fr DLIB/WORK
vlib DLIB/WORK
vmap work DLIB/WORK


#vlog  -work work \
#	+libext+.sv \
#	-y ../../hw/rtl \
#	-y ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/include \
#	-y ../../hw/vendor/esl_epfl_x_heep/hw/system/ \
#	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/lowrisc_opentitan/hw/ip/prim/rtl/ \
#	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/pulp_platform_tech_cells_generic/src/rtl/ \
#	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/pulp_platform_common_cells/src/ \
#	-y ../../hw/vendor/esl_epfl_cgra/wrapper/ \
#	-y ../../hw/vendor/esl_epfl_cgra/rtl \
#	-y ../../hw/vendor/esl_epfl_cgra/eda/HDL/tech_lib/clkgate/rtl/ \
#	-y ../../hw/vendor/esl_epfl_cgra/sim/ \
#	+incdir+../../hw/vendor/esl_epfl_x_heep/hw/vendor/pulp_platform_common_cells/include/ \
#	HDL/RTL/cgra_x_heep_top.sv \

 
vlog  -work work \
	+libext+.sv \
	-y ../../hw/rtl \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/ \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/include \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/lowrisc_opentitan/hw/ip/prim/rtl/ \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/pulp_platform_tech_cells_generic/src/rtl/ \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/openhwgroup_cv32e40p/rtl/include/ \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/pulp_platform_common_cells/src/ \
	-y ../../hw/vendor/esl_epfl_cgra/wrapper/ \
	-y ../../hw/vendor/esl_epfl_cgra/rtl \
	-y ../../hw/vendor/esl_epfl_cgra/eda/HDL/tech_lib/clkgate/rtl/ \
	-y ../../hw/vendor/esl_epfl_cgra/sim/ \
	-v ../../hw/vendor/esl_epfl_x_heep/hw/system/x_heep_system.sv \
	-v ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/core_v_mini_mcu.sv \
	-v ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/cpu_subsystem.sv \
	-v ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/debug_subsystem.sv \
	-v ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/system_bus.sv \
	-v ../../hw/vendor/esl_epfl_x_heep/hw/core-v-mini-mcu/peripheral_subsystem.sv \
	-y ../../hw/vendor/esl_epfl_x_heep/hw/vendor/openhwgroup_cv32e40p/rtl/include/ \
	+incdir+../../hw/vendor/esl_epfl_x_heep/hw/vendor/pulp_platform_common_cells/include/ \
	HDL/TBENCH/testharness.sv

vsim -lib work testharness

vcd file SA/dump.vcd

vcd add cgra_top_wrapper/cgra_x_heep_top_i/*

vcd off SA/dump.vcd

run 2ns

vcd on SA/dump.vcd

run 5ms

quit

