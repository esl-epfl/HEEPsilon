'''
    File name: heeptest_gen.py
    Authors: Ruben Rodriguez, Juan Sapriza
    Date created: 19/04/2023
    Python Version: Python 3.9.13
    Description: Generate test code (main and header) from compiled kernels
'''

'''``````````````````````````````````````````````````````````````````````````
INCLUDED MODULES
``````````````````````````````````````````````````````````````````````````'''

import os
import sys
import io
import json
import string
from datetime import date

'''``````````````````````````````````````````````````````````````````````````
OBTAINMENT AND CHECKING OF INPUT FILES

This script expects as input the name of a kernel. 
The kernel should be located in
    ../kernels/     (rel-path)
and there must be a folder with the name of the kernel. 

Inside the kernel directory, a second folder with called
    data/           (auxFolder_name)
is expected. Inside it, 4 files need to be complete and correct:
    * cgra_imem.bit - Containing the bitstream for instruction memory. 
    * cgra_kmem.bit - Containing a 16 word-long array, with the kernel ID on
                      its element 1. 
    * function.h    - The function implemented, that will be run to compare 
                      software vs. CGRA. It should take the same inputs and 
                      return the same outputs as the CGRA. It should not 
                      modify its inputs (the same are used for the CGRA). 
    * io.json       - The description of inputs and outputs of the CGRA.

When run from the directory containing this script, simply call it with:
    python heeptest_gen.py <name_of_kernel>

The generated <name_of_kernel>.c and <name_of_kernel>.h will be stored inside
the <name_of_kernel> folder. 

``````````````````````````````````````````````````````````````````````````'''

# Make sure that only one argument was passed.
if len(sys.argv) == 2 :
    kernel = sys.argv[1]
else:
    sys.exit("[ERROR] No path specified")


'''``````````````````````````````````````````````````````````````````````````
GETTING NAMES AND PATHS
``````````````````````````````````````````````````````````````````````````'''
# Get the template file names. 
source_tpl_name     = "source.c.tpl"
header_tpl_name     = "header.h.tpl"

# The location of the kernels relative to this script.
rel_path            = "../kernels/" 
# The location of the source files inside the kernel's folder.
auxFolder_name      = "data/"

# Where the source files will be fetched from.
source_path = rel_path + str(kernel)
# Where the output files will be stored. 
dest_path   = source_path

# For the same <name_of_kernel>, different formats are computed.
# (i.e. kernel, KERNEL, Kernel, kern). 
filename = kernel
FILENAME = filename.upper()
Filename = filename.capitalize()
shortname = filename[0:min(4,len(filename))]

# Files being generated:
header_file_name        = dest_path + "/" + filename + ".h"
source_file_name        = dest_path + "/" + filename + ".c"

# Sources files to be fetch:
imem_bit_name       = source_path + "/" + auxFolder_name + "cgra_imem.bit"
kmem_bit_name       = source_path + "/" + auxFolder_name + "cgra_kmem.bit"
io_file_name        = source_path + "/" + auxFolder_name + "io.json"
function_name       = source_path + "/" + auxFolder_name + "function.h"

'''``````````````````````````````````````````````````````````````````````````
CONFIGURATIONS
``````````````````````````````````````````````````````````````````````````'''
cols_n = 4

'''``````````````````````````````````````````````````````````````````````````
CHECKING INPUT FILES

The 4 input files are required for this script.
``````````````````````````````````````````````````````````````````````````'''

if not os.path.exists(imem_bit_name) :
    sys.exit("[ERROR] No bitstream: " + str(imem_bit_name))

if not os.path.exists(kmem_bit_name) :
    sys.exit("[ERROR] No bitstream: " + str(kmem_bit_name))

if not os.path.exists(io_file_name) :
    sys.exit("[ERROR] No bitstream info: " + str(io_file_name))

if not os.path.exists(function_name) :
    sys.exit("[ERROR] No function: " + str(function_name))

'''``````````````````````````````````````````````````````````````````````````
BITSTREAM GENERATION

From the imem and kmem bitstreams, variables containing that information are
generated and will be stored statically in the kernel source file.
``````````````````````````````````````````````````````````````````````````'''
mem_str = {}
for mem_filename in [imem_bit_name, kmem_bit_name]:
    with open(mem_filename , 'r') as mem_bit_file:
        mem_str[mem_filename] = ""
        mem_bit_lines = mem_bit_file.readlines()
        for mem_line in mem_bit_lines[0:-1] :
            mem_line = mem_line.replace("\n",", ")
            mem_str[mem_filename] += f" {mem_line}"
        mem_line = mem_bit_lines[-1].replace("\n","")
        mem_str[mem_filename] += f" {mem_line}"

'''``````````````````````````````````````````````````````````````````````````
VARIABLE DECLARATION

``````````````````````````````````````````````````````````````````````````'''

with open(io_file_name) as f:
    io_data = json.loads(f.read())

# Setting up the input variable.
# If it is an array, a pointer to the input variable is stored in the input 
# array of the CGRA. 
# If it is only one word, the value is copied in the input array of the CGRA. 
in_vars_str     = ""
in_args_str     = ""
in_vars_n       = len( io_data["inputs"] )

for in_vars in io_data["inputs"]: 

    in_var_name     = f"{in_vars['name']}"
    in_var_depth    = in_vars['num']

    sufix = ""
    if in_var_depth > 1:
        sufix = f"[{in_var_depth}]"

    # Two strings are generated, one with the declaration of the variable
    # (in_vars_str) and the other with planly its name (in_args_str) to be
    # used as argument in the software call. 
    in_vars_str += f"static {in_vars['type']}\t{in_var_name}{sufix};\n"
    in_args_str += f"{in_vars['name']}, "

in_args_str = in_args_str[:-2] # Remove the last comma + space from the arg.

# Setting up the output variables. 
# There should only be one!
# ------------------------------------------------------------------------------------------------------ complete this

out_vars_str    = ""
for out_vars in io_data["outputs"]: 
    out_var_name    = f"{out_vars['name']}"
    out_vars_n   = out_vars['num']

    sufix   = ""
    prefix = ""
    if out_vars_n > 1:
        sufix   = f"[{out_vars_n}]"
        prefix  = "*"
    out_var_cgra_str   = f"{out_var_name}_cgra"
    out_var_soft_str   = f"{out_var_name}_soft"
    out_vars_str += f"static {out_vars['type']}\t{out_var_cgra_str}{sufix};\n"
    out_vars_str += f"static {out_vars['type']}\t{prefix}{out_var_soft_str};\n"

'''``````````````````````````````````````````````````````````````````````````
CONFIGURATION FUNCTION

During the configuration, random numbers are assigned to the input variables.
The random values can be bounded to min and max values defined in the io.json. 

Afterwards, this input variables are copied into the CGRA input array.
``````````````````````````````````````````````````````````````````````````'''

# First, the random (bounded) values are obtained into the input variables.
config_str = ""
for inout in io_data["inputs"]:
    min_val = "0"
    max_val = "UINT_MAX - 1"

    if inout.get("min") : 
        min_val = str(inout.get("min"))
    if inout.get("max") : 
        max_val = str(inout.get("max"))

    # If the variable is an array, a for loop is used to fill the random values.     
    if inout['num'] == 1:
        config_str  +=  f"\t{inout['name']} = kcom_getRand() % ({max_val} - {min_val} + 1) + {min_val};\n"
    else:
        config_str  +=  f"\tfor(int i = 0; i < {inout['num']}; i++ )\n"
        config_str  +=  f"\t\t{inout['name']}[i] = kcom_getRand() % ({max_val} - {min_val} + 1) + {min_val};\n"

# The input variables are copied into the CGRA input array.
input_max = [0,0,0,0]
for col_num in range(cols_n):
    var_num = 0

    for inout in io_data[f"read_col{col_num}"]:
        config_str  +=  f"\tcgra_input[{col_num}][{var_num}] = {inout['name']};\n"
        var_num     +=  1
        input_max[col_num] += 1

output_max = [0,0,0,0]
for col_num in range(cols_n):
    for inout in io_data[f"write_col{col_num}"]:
        output_max[col_num] += 1

output_max.append(1) # At least one object we will have
o_max = max(output_max)
out_var_cols_used_str = str(o_max) # The maximum number of used columns.

'''``````````````````````````````````````````````````````````````````````````
RESULT CROSS-CHECK

In order to determine the effectiveness of the computation, the results 
obtained from software and the CGRA are compared. 

For clarity, it extracts the information from the CGRA output array into an
output variable.
``````````````````````````````````````````````````````````````````````````'''

check_load_str = ""

val_idx = 0
for col_idx in range(cols_n):
    for value in io_data[f"write_col{col_idx}"] :
        # If one of the elements of the CGRA outputs its value to an output
        # variable, that value is directly copied to it.
        # Otherwise, the output variable is an array and each element (id) 
        # gets the value of a column-value pair.
        if value.get("name") == out_var_name:
            check_load_str  +=  f"\t{out_var_cgra_str} = cgra_output[{col_idx}][{val_idx}];\n"
        else:
            check_load_str  +=  f"\t{out_var_cgra_str}[{val_idx}] = cgra_output[{col_idx}][{value['id']}];\n"
            val_idx         += 1
# The check expression is comparing the values stored in the variable if it is
# an array, otherwise, performs an elemnt-wise comparison.
sufix = "[i]" if out_vars_n > 1 else ""
cgra_res_elem_str = out_var_cgra_str + sufix
soft_res_elem_str = out_var_soft_str + sufix


'''``````````````````````````````````````````````````````````````````````````
CREATION OF THE SOURCE FILE

A source file is created based on the template in the directory of this script.

Roughly, the source file will include:
    * The bitstreams
    * An input and output array, where the CGRA will take and drop its results.
    * Return variables where the software result and the extracted result from
    the CGRA are compared.
    * A config() function that sets the inputs to random numbers accoding to 
    its needs.
    * A software() function that merely calls the kernel function from 
    function.h and stores its result. 
    * A check() function that returns the number of differences between the 
    software and CGRA results. 
    * A kcom_kernel_t structure with all the parameters for the kernel test
    app to take this kernel and execute it.

``````````````````````````````````````````````````````````````````````````'''

with open( source_tpl_name) as t:
    template = string.Template( t.read() )

description_str = "A description of the kernel..."
date_str = date.today()

final_output = template.substitute(\
                                    filename            = filename              ,\
                                    FILENAME            = FILENAME              ,\
                                    Filename            = Filename              ,\
                                    shortname           = shortname             ,\
                                    date                = date_str              ,\
                                    description         = description_str       ,\
                                    aux_folder          = auxFolder_name        ,\
                                    cols_n              = str(cols_n)           ,\
                                    in_vars             = in_vars_str           ,\
                                    in_vars_n           = str(in_vars_n)        ,\
                                    out_vars            = out_vars_str          ,\
                                    out_vars_n          = str(out_vars_n)       ,\
                                    out_var_cols_used   = out_var_cols_used_str ,\
                                    in_args             = in_args_str           ,\
                                    imem                = mem_str[imem_bit_name],\
                                    kmem                = mem_str[kmem_bit_name],\
                                    config              = config_str            ,\
                                    out_var_soft        = out_var_soft_str      ,\
                                    function            = filename              ,\
                                    check_load          = check_load_str        ,\
                                    cgra_res_elem       = cgra_res_elem_str     ,\
                                    soft_res_elem       = soft_res_elem_str     ,\
                                    )


with open(source_file_name, "w") as output:
    output.write(final_output)

'''``````````````````````````````````````````````````````````````````````````
CREATION OF THE HEADER FILE

A header file is created based on the template in the directory of this script.

It only contains the inclusion of the kernels_common module and an extern
of the kernel structure for the kernel_test app to use.
``````````````````````````````````````````````````````````````````````````'''

with open(header_tpl_name) as t:
    template = string.Template(t.read())

final_output = template.substitute( \
                                    filename        = filename,\
                                    FILENAME        = FILENAME,\
                                    shortname       = shortname,\
                                    date            = date_str,\
                                    description     = description_str,\
                                    )

with open(header_file_name, "w") as output:
    output.write(final_output)

'''``````````````````````````````````````````````````````````````````````````
FINISH
``````````````````````````````````````````````````````````````````````````'''

print("Source and header files for kernel", Filename, "were written succesfully!")

