# Repo folder structure

    .             
    ├── hw      
    │   ├── rtl            
    │   └── vendor         
    ├── scripts     
    │   ├── sim   
    │   └── synthesis              
    ├── sw       
    │   ├── applications          
    │   ├── device
    │   ├── x_heep_applications (hw/vendor/esl_epfl_x_heep/sw/applications)       
    │   ├── x_heep_device (hw/vendor/esl_epfl_x_heep/sw/device)
    │   └── x_heep_linker (hw/vendor/esl_epfl_x_heep/sw/linker)               
    ├── tb
    ├── util
    └── README.md
    


Minimal configuration of a cgra-x-heep
============================

# Prerequisite

The simulation enrironment is based on the one from [X-HEEP](https://github.com/esl-epfl/x-heep).

Follow the prequisite steps on the [vendorized X-HEEP](https://github.com/esl-epfl/cgra_x_heep/tree/main/hw/vendor/esl_epfl_x_heep#prerequisite).

Consider that both the `environment.yml` and `python-requirements.txt` are now located in:
```bash
hw/vendor/esl_epfl_x_heep/
```
The first steps should be adapted as:
```bash
conda env create -f hw/vendor/esl_epfl_x_heep/environment.yml
conda activate core-v-mini-mcu
pip3 install --user -r hw/vendor/esl_epfl_x_heep/python-requirements.txt
```

# Adding external IPs

This repository relies on [vendor](https://docs.opentitan.org/doc/ug/vendor_hw/) to add new IPs.
In the ./util folder, the vendor.py scripts implements what is describeb above.

# Compiling with Makefile

You can compile the example applications and the platform using the Makefile. Type 'make help' for more information.

## Generate core-v-mini-mcu package

First, you have to generate the SystemVerilog package and C header file of the core-v-mini-mcu (x-heep):

```
make mcu-gen
```

To change the default cpu type (i.e., cv32e20), the default bus type (i.e., onetoM) type
or the memory size (i.e., number of banks):

```
make mcu-gen CPU=cv32e40p BUS=NtoM MEMORY_BANKS=16
```

The last command generates x-heep with the cv32e40p core, with a parallel bus, and 16 memory banks,
each 32KB, for a total memory of 512KB.

## Compiling Software

```
make app-helloworld
```

This will create the executable file to be loaded in your target system.

## Simulating

This project supports simulation with Verilator, Synopsys VCS, and Siemens Questasim.

### Compiling for Verilator

To simulate your application with Verilator, first compile the HDL:

```
make verilator-sim
```

then, go to your target system built folder

```
cd ./build/eslepfl_systems_cgra-x-heep_0/sim-verilator
```

and type to run your compiled software:

```
./Vtestharness +firmware=../../../sw/x_applications/hello_world/hello_world.hex
```

or to execute all these three steps type:

```
make run-helloworld-verilator
```

or to run the CGRA functional test

```
make run-cgra-test-verilator
```


### Compiling for VCS

To simulate your application with VCS, first compile the HDL:

```
make vcs-sim
```

then, go to your target system built folder

```
cd ./build/eslepfl_systems_cgra-x-heep_0/sim-vcs
```

and type to run your compiled software:

```
./eslepfl_systems_cgra-x-heep_0 +firmware=../../../sw/x_applications/hello_world/hello_world.hex
```

### Compiling for Questasim

To simulate your application with Questasim, first set the env variable `MODEL_TECH` to your Questasim bin folder, then compile the HDL:

```
make questasim-sim
```

then, go to your target system built folder

```
cd ./build/eslepfl_systems_cgra-x-heep_0/sim-modelsim/
```

and type to run your compiled software:

```
make run PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex"
```

or to execute all these three steps type:

```
make run-helloworld-verilator
```

or to run the CGRA functional test

```
make run-cgra-test-verilator
```

You can also use vopt for HDL optimized compilation:

```
make questasim-sim-opt
```

then go to

```
cd ./build/eslepfl_systems_cgra-x-heep_0/sim_opt-modelsim/
```
and 

```
make run RUN_OPT=1 PLUSARGS="c firmware=../../../sw/x_applications/hello_world/hello_world.hex"
```

Questasim version must be >= Questasim 2020.4

### UART DPI

To simulate the UART, we use the LowRISC OpenTitan [UART DPI](https://github.com/lowRISC/opentitan/tree/master/hw/dv/dpi/uartdpi). 
Read how to interact with it in the Section "Interact with the simulated UART" [here](https://docs.opentitan.org/doc/ug/getting_started_verilator/).
The output of the UART DPI module is printed in the `uart0.log` file in the simulation folder.

For example, to see the "hello world!" output of the Verilator simulation:

```
cd ./build/eslepfl_systems_cgra-x-heep_0/sim-verilator
./Vtestharness +firmware=../../../sw/x_applications/hello_world/hello_world.hex
cat uart0.log
```
## Debug

Follow the [Debug](./hw/vendor/esl_epfl_x_heep/Debug.md) guide to debug core-v-mini-mcu.

## Execute From Flash

Follow the [ExecuteFromFlash](./hw/vendor/esl_epfl_x_heep/ExecuteFromFlash.md) guide to execute code directly from the FLASH with modelsim or vcs (verilator cannot simulate the execution from flash).
