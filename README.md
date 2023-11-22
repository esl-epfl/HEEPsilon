<p align="left"><img src="docs/HEEPsilon_logo.png" width="500"></p>

HEEPsilon is a versatile computing platform targeting ultra low power processing of biological and environmental signals. It is built over the [X-HEEP](https://github.com/esl-epfl/x-heep) platform and extends it with [openEdgeCGRA](https://github.com/esl-epfl/OpenEdgeCGRA) a design-time resizable and run-time reprogrammable Coarse Grained Reconfigurable Array (CGRA).
For a brief insight on HEEPsilon please refer to our abstract:

📄 [An Open-Hardware Coarse-Grained Reconfigurable Array for Edge Computing](https://dl.acm.org/doi/10.1145/3587135.3591437).

As an X-HEEP spinoff, HEEPsilon keeps all X-HEEP functionalities, from RTL simulation on Verilator, VCS and Questasim to implementation on the [PYNQ-Z2 FPGA](https://www.xilinx.com/support/university/xup-boards/XUPPYNQ-Z2.html). Our cousin HEEPocrates was recently taped-out in TSMC 65nm process and is currently undertaking tests successfully.

In addition to all the tools available for X-HEEP, HEEPsilon is building a toolchain to simplify the C-code→CGRA process.

---

# Getting started

Due to its modular design, HEEPsilon respects the X-HEEP workflow. As such, you can follow [X-HEEP's getting started](https://x-heep.readthedocs.io/en/latest/How_to/GettingStarted.html) to set up the environment... HOWEVER...

Althought the HEEPsilon team will try to keep the latest version of X-HEEP available, changes in the X-HEEP setup might not reflect immediately on this repository.

👉 For the most accurate set-up instructions please refer to the documentation of the [vendorized X-HEEP](https://github.com/esl-epfl/cgra_x_heep/tree/main/hw/vendor/esl_epfl_x_heep).


# Behavioural simulations

The CGRA used in HEEPsilon can be simulated with CGRA-instruction accuracy using the [ESL-CGRA simulator](https://github.com/esl-epfl/ESL-CGRA-simulator).
This allows for fast and easy-to-debug design of kernels for the CGRA. Once you are happy with your design you can compile the assembly and get the bitstream to load into the CGRA.

# SAT-MapIt Compiler

Your kernel is too complex to be mapped manually? Try using the [SAT-MapIt mapper and compiler](https://github.com/CristianTirelli/SAT-MapIt). Properly label your C-code and let SAT-MapIt find an efficient mapping you can test in the simulator and deploy in the CGRA.

# Testing a kernel

Once you have tested your setup with the `cgra_func_test` application you can start trying out different kernels. HEEPsilon provides a set of tools to easily go from C-code to CGRA bitstreams. All kernels are converted into a standard C source and header file pair which you can use with the `kernel_test` application to measure the speed-up of your CGRA implementation as well as see stochastical variations.

# Adding a complex environment to your platform

If you application requires some hardcore input-output management, maybe you want to try out the [X-HEEP FEMU](https://github.com/simone-machetti/x-heep-femu). Connect your PYNQ-Z2 FPGA via SSH and start deploying different hardware versions of X-HEEP or HEEPsilon, test different software applications and interface with the hardware from the comfort of Python scripts or Jupyter notebooks.

# Wanna collaborate?

HEEPsilon is a newborn project that already brings together dozens of researchers from 4 universities across Switzerland, Spain and Italy. There is plenty of cool work to be done for and with HEEPsilon, join us!

Pending work includes:
* Development of new kernels for the CGRA and validation in real applications.
* Integration of the different compilation tools into a single workflow.
* Extracting variable information from the LLVM pass during C-code → CGRA assembly process.
* Characterizing the CGRA hardware for cycle and energy-accurate simulation.

# Contact us

Have some questions? Don't hesitate to contact us: juan.sapriza@epfl.ch

