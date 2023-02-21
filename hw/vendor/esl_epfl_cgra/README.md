# HEALWEAR CGRA GIT REPOSITORY 

![](doc/Healwear_top_view.png)

## About

This repository holds the **HEALWEAR** hardware accelerator developed at ESL. It is composed of different implementations and scripts. Healwear is a **CGRA** (Coarse-Grained Reconfigurable Array) developed for low power bio-medical applications and currently (2019) being update to support newer machine learning algorithms. Further information about Healwear can be found at https://ieeexplore.ieee.org/abstract/document/7936544.

Due to an error at the creation of this repository, the VHDL files have been added only later, even though they mentionned in the commits message. The current repository is a cleaned version with the initial implementation and the vhdl files appear in the standalone_v0.1 version with all the changes applied in one shot (because the history is missing).

## Repository structure

| Folders         | Content                                                                                         |
|-----------------|-------------------------------------------------------------------------------------------------|
| apps            | Folder containing the C code version of the kernel accelerated on Healwear.                     |
| doc             | CGRA documentations.                                                                            |
| kernel_mapping  | Graphical representations of the kernels mapped on Healwear to better understand how it works.  |
| power_analysis  | Removed for now.                                                                                |
| rtl             | Healwear HDL code.                                                                              |
| testbench       | Testbench for the OPENPULP version.                                                             |
| utilities       | Contains miscellaneous scripts useful for Healwear programing/debugging/...                     |
| wrap            | Wrappers for the CGRA on OPENPULP (sv) and MUSEICv3 (vhdl).                                     |

## Tags

| Tag name        | Info about version                                                                                                                      |
|-----------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| standalone_v0.0 | Version given by Soumya and used as a starting point for future implementations. Version used in TamaRISC.                              |
| standalone_v0.1 | Version very similar to v0.0 but updated with some features for simplified test (e.g., input file to simulate data/instruction memory). |
| standalone_v1.0 | Revised version based on v0.1. This version is close to the previous one but all modules have been tested independently in rtl and also post-synthesis. All modules needed for integration in a system are present (e.g., simple DMA structure).  |
| full_CGRA_v1.0 | Version tested on both openpulp and museicv3 (19.06.2019) where the only changes are the ones required to interface with each platform (no important architecture change).  |
| CGRA_museic_v1.0 | Baseline CGRA version for MUSEICV3 platform.  |

## Branches

Healwear is implemented on OPENPULP (single/multi-core version) and MUSEICv3. Both versions have been merged under the master branch. The main differences are the rtl/data_handler.vhd file (to handle AHB and tcdm protocol) and the wrapper.

| Branches        | Info                                                                                            |
|-----------------|-------------------------------------------------------------------------------------------------|
| master          | OPENPULP and MUSEICv3 versions.                                                                 |
| pulp_cluster    | Last version not merged yet with clock gating and small updates to correct the design.          |
| museicv3        | Last version not merged yet with clock gating and small updates to correct the design.          |
| cgra_16b        | CGRA 16b version for PULP with 32b rd/wr through TCDM. Version used for DATE20 results.         |

TBD : add more documentation about the way each implementation works in its platform.
## master branch

Both the openpulp and museicv3 are under the master branch. Select the one you need in the rtl/cgra_config.vhd file.

### OPENPULP configuration

To add the cgra to the openpulp project do the following:

	1. Create a folder named hwpe-cgra-engine in the ips/ folder.
	2. Copy the rtl/ wrap/ and mem/ in that folder.
	3. Copy the file in vcompile/ips/hwpe-cgra-engine.mk sim/vcompile/ips/.
	4. Add corresponding lines on the same model that other ips in sim/vcompile/ips.mk (3 lines: build, lib and clean commands).
	5. In ips/pulp_cluster/rtl/pulp_cluster.sv add the cgra_wrap module using the xne accelerator example (code should be exactly similar, only the module name changes) or replace it with the corresponding file in openpulp folder (some differences are possible depending on the version of pulp used).
	6. Add the following line to the file sim/tcl_files/config/vsim_ips.tcl: -L cgra_engine_lib \
	7. Compile the platform as explained on pulp github webpage.

Similar method is applicable to any pulp implementation, in particular pulpissimo (but in this case the CGRA is on the soc side).

### MUSEICv3 configuration

To add the cgra to the museicv3 project do the following:

	Add cgra files to museic project:
	1. device_lib: create cgra_engine folder with same structure as other devices
	2. museic_lib/museic_proc/CONFIG/load_rtl.tcl: add cgra_engine in device_lib

	Add AHB/APB master/slave (based on FFT HW acccelerator):
	1. amba_lib/amba_global_pkg/INTERFACE/amba_global_pkg.p.vhdl: adjust the maximum number of masters
	2. museic_lib/museic_dig/INTERFACE/museic_dig_pkg.p.vhd: adjust constants (#slaves, #masters, #irq) and add the device to the list
	3. museic_lib/museic_proc/RTL/museic_proc.m.vhd: add CGRA_top_wrap module and add apb port on museic_proc archi (also in INTERFACE/...vhd
	4. museic_lib/museic_dig/RTL/museic_dig.m.vhd: add CGRA apb signals and mapping
	5. software/libmuseic/v3.0/cmsis/device/IMEC/include/museic.h: add CGRA irq and APB base address
	6. software/libmuseic/v3.0/startup/vectors.c: add CGRA_Handler (in two places!)
