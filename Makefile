# Copyright EPFL contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0


TARGET ?= sim

# 1 external domain for the CGRA
EXTERNAL_DOMAINS = 1

export HEEP_DIR = hw/vendor/esl_epfl_x_heep/
include $(HEEP_DIR)Makefile.venv

# Generates mcu files
mcu-gen:
	@echo This is mcu-gen from topmost Makefile
	cd hw/vendor/esl_epfl_x_heep && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/core-v-mini-mcu/include --cpu $(CPU) --bus $(BUS) --memorybanks $(MEMORY_BANKS) --external_domains $(EXTERNAL_DOMAINS) --pkg-sv hw/core-v-mini-mcu/include/core_v_mini_mcu_pkg.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/core-v-mini-mcu/ --memorybanks $(MEMORY_BANKS) --tpl-sv hw/core-v-mini-mcu/system_bus.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir ../../../tb/ --memorybanks $(MEMORY_BANKS) --tpl-sv ../../../tb/tb_util.svh.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/system/ --tpl-sv hw/system/pad_ring.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/core-v-mini-mcu/ --tpl-sv hw/core-v-mini-mcu/core_v_mini_mcu.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/system/ --tpl-sv hw/system/x_heep_system.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir sw/device/lib/runtime --cpu $(CPU) --memorybanks $(MEMORY_BANKS) --header-c sw/device/lib/runtime/core_v_mini_mcu.h.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir sw/linker --memorybanks $(MEMORY_BANKS) --linker_script sw/linker/link.ld.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir . --memorybanks $(MEMORY_BANKS) --pkg-sv ./core-v-mini-mcu.upf.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/ip/power_manager/rtl --memorybanks $(MEMORY_BANKS) --external_domains $(EXTERNAL_DOMAINS) --pkg-sv hw/ip/power_manager/data/power_manager.sv.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/ip/power_manager/data --memorybanks $(MEMORY_BANKS) --external_domains $(EXTERNAL_DOMAINS) --pkg-sv hw/ip/power_manager/data/power_manager.hjson.tpl  && \
	bash -c "cd hw/ip/power_manager; source power_manager_gen.sh; cd ../../../"  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir sw/device/lib/drivers/power_manager --memorybanks $(MEMORY_BANKS) --external_domains $(EXTERNAL_DOMAINS) --pkg-sv sw/device/lib/drivers/power_manager/data/power_manager.h.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/system/pad_control/data --pkg-sv hw/system/pad_control/data/pad_control.hjson.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir hw/system/pad_control/rtl --pkg-sv hw/system/pad_control/rtl/pad_control.sv.tpl  && \
	bash -c "cd hw/system/pad_control; source pad_control_gen.sh; cd ../../../"  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir sw/linker --memorybanks $(MEMORY_BANKS) --linker_script sw/linker/link_flash_exec.ld.tpl  && \
	python util/mcu_gen.py --cfg mcu_cfg.hjson --pads_cfg pad_cfg.hjson --outdir sw/linker --memorybanks $(MEMORY_BANKS) --linker_script sw/linker/link_flash_load.ld.tpl &&\
	cd ../../

# Runs verible formating
verible:
	util/format-verible;

app-cgra-test:
	$(MAKE) -C sw applications/cgra_func_test/main.hex  TARGET=$(TARGET)

app-cgra-dbl-search:
	$(MAKE) -C sw applications/cgra_dbl_search/main.hex  TARGET=$(TARGET)

# Tools specific fusesoc call

# Simulation
verilator-sim: mcu-gen
	fusesoc --cores-root . run --no-export --target=sim --tool=verilator $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildsim.log

questasim-sim: mcu-gen
	fusesoc --cores-root . run --no-export --target=sim --tool=modelsim $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildsim.log

questasim-sim-opt: questasim-sim
	$(MAKE) -C build/eslepfl_systems_cgra-x-heep_0/sim-modelsim opt

vcs-sim:
	fusesoc --cores-root . run --no-export --target=sim --tool=vcs $(FUSESOC_FLAGS) --setup --build eslepfl:systems:cgra-x-heep 2>&1 | tee buildsim.log

run-cgra-test-verilator: mcu-gen verilator-sim app-cgra-test
	cd ./build/eslepfl_systems_cgra-x-heep_0/sim-verilator; \
	./Vtestharness +firmware=../../../sw/applications/cgra_func_test/main.hex; \
	cat uart0.log; \
	cd ../../..;

run-cgra-test-questasim: mcu-gen questasim-sim app-cgra-test
	cd ./build/eslepfl_systems_cgra-x-heep_0/sim-modelsim; \
	make run PLUSARGS="c firmware=../../../sw/applications/cgra_func_test/main.hex"; \
	cat uart0.log; \
	cd ../../..;


## Generates the build output for a given application
## Uses verilator to simulate the HW model and run the FW
## UART Dumping in uart0.log to show recollected results
run: 
	$(MAKE) app PROJECT=$(PROJECT)
	cd ./build/eslepfl_systems_cgra-x-heep_0/sim-verilator; \
	./Vtestharness +firmware=../../../sw/build/main.hex; \
	cat uart0.log; \
	cd ../../..;


XHEEP_MAKE = $(HEEP_DIR)/external.mk
include $(XHEEP_MAKE)
