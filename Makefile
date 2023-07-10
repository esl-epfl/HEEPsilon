# Copyright EPFL contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0


# Makefile to generates cgra-x-heep files and build the design with fusesoc

.PHONY: clean help

TARGET 		?= sim
FPGA_BOARD 	?= pynq-z2


# 1 external domain for the CGRA
EXTERNAL_DOMAINS = 1


export HEEP_DIR = hw/vendor/esl_epfl_x_heep/
include $(HEEP_DIR)Makefile.venv


# Generates mcu files. First the mcu-gen from X-HEEP is called.
# This is needed to be done after the X-HEEP mcu-gen because the test-bench to be used is the one from CGRA-X-HEEP, not the one from X-HEEP.
mcu-gen:
	$(MAKE) -f $(XHEEP_MAKE) EXTERNAL_DOMAINS=${EXTERNAL_DOMAINS} $(MAKECMDGOALS)
	cd hw/vendor/esl_epfl_x_heep &&\
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson  --outdir ../../../tb/ --memorybanks $(MEMORY_BANKS) --tpl-sv ../../../tb/tb_util.svh.tpl

## Builds (synthesis and implementation) the bitstream for the FPGA version using Vivado
## @param FPGA_BOARD=nexys-a7-100t,pynq-z2
## @param FUSESOC_FLAGS=--flag=<flagname>
vivado-fpga: |venv
	fusesoc --cores-root . run --no-export --target=$(FPGA_BOARD) $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildvivado.log


# Runs verible formating
verible:
	util/format-verible;

# Simulation
verilator-sim: mcu-gen
	fusesoc --cores-root . run --no-export --target=sim --tool=verilator $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildsim.log

questasim-sim: mcu-gen
	fusesoc --cores-root . run --no-export --target=sim --tool=modelsim $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildsim.log

questasim-sim-opt: questasim-sim
	$(MAKE) -C build/eslepfl_systems_cgra-x-heep_0/sim-modelsim opt

vcs-sim:
	fusesoc --cores-root . run --no-export --target=sim --tool=vcs $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildsim.log


## Generates the build output for a given application
## Uses verilator to simulate the HW model and run the FW
## UART Dumping in uart0.log to show recollected results
run-verilator:
	$(MAKE) app PROJECT=$(PROJECT)
	cd ./build/eslepfl_systems_cgra-x-heep_0/sim-verilator; \
	./Vtestharness +firmware=../../../sw/build/main.hex; \
	cat uart0.log; \
	cd ../../..;

# Builds the program and uses flash-load to run on the FPGA
run-fpga:
	$(MAKE) app PROJECT=$(PROJECT) LINKER=flash_load TARGET=pynq-z2
	( cd hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog && make clean && make all ) ;\
	sudo $(MAKE) flash-prog ;\

# Builds the program and uses flash-load to run on the FPGA.
# Additionally opens picocom (if available) to see the output.
run-fpga-com:
	$(MAKE) app PROJECT=$(PROJECT) LINKER=flash_load TARGET=pynq-z2
	( cd hw/vendor/esl_epfl_x_heep/sw/vendor/yosyshq_icestorm/iceprog && make clean && make all ) ;\
	sudo $(MAKE) flash-prog ;\
	sudo picocom -b 115200 -r -l --imap lfcrlf /dev/ttyUSB2

XHEEP_MAKE = $(HEEP_DIR)/external.mk
include $(XHEEP_MAKE)
