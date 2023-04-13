#!/usr/bin/bash
source /edadk/startup/bashrc_heep.sh

# Questasim
#if you want to update questa, change the path here and inside the edadk.conf file
export MODEL_TECH=/softs/mentor/qsta/2020.4/bin 
export MGLS_LICENSE_FILE=1717@ielsrv01.epfl.ch
export PATH="/softs/mentor/qsta/2020.4/bin/:$PATH"
export MTI_VCO_MODE=64
# RISC-V compiler
export RISCV=/shares/eslfiler1/apps/linux/Development/rv32imc

# VERIBLE
export VERIBLE=/shares/eslfiler1/apps/linux/Development/verible
export PATH=$VERIBLE/bin:$PATH

