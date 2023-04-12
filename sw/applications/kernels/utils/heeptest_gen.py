'''
    File name: heeptest_gen.py
    Author: Ruben Rodriguez
    Date created: 26/11/2022
    Python Version: Python 3.9.13
    Description: Generate test code (main and header) from compiled kernels
'''

import os
import sys
import io
import json
import string

#################
# File Settings #
#################

if len(sys.argv) > 2 :
    source_path = sys.argv[1]
    dest_path   = sys.argv[2]
else:
    sys.exit("[ERROR] No path specified")

# Templates:
bitstream_tpl_name  = "bitstream.h.tpl"
source_tpl_name     = "source.c.tpl"
header_tpl_name     = "header.h.tpl"


# Variables
source_path = str(source_path)
dest_path   = str(dest_path)
filename = source_path[ source_path.rfind('/') +1: ]
FILENAME = filename.upper()
Filename = filename.capitalize()
if 1:
    print("filename = ", filename )
    print("FILENAME = ", FILENAME )
    print("Filename = ", Filename )

# Files being generated:
bitstream_file_name     = dest_path + "/cgra_bitstream.h"
header_file_name        = dest_path + "/" + filename + ".h"
source_file_name        = dest_path + "/" + filename + ".c"

# Bitstream:
imem_bit_name       = source_path + "/cgra/cgra_imem.bit"
kmem_bit_name       = source_path + "/cgra/cgra_kmem.bit"
io_file_name        = source_path + "/cgra/io.json"


##########################
# Checking required files exist #
##########################

if not os.path.exists(imem_bit_name) :
    sys.exit("[ERROR] No bitstream detected in " + str(imem_bit_name))

if not os.path.exists(kmem_bit_name) :
    sys.exit("[ERROR] No bitstream detected in " + str(kmem_bit_name))

if not os.path.exists(io_file_name) :
    sys.exit("[ERROR] No bitstream info detected in " + str(io_file_name))


##########################
# Bitstream file generation #
##########################

mem_str = {}
for mem_filename in [imem_bit_name, kmem_bit_name]:
    with open(mem_filename , 'r') as mem_bit_file:
        mem_str[mem_filename] = ""
        mem_bit_lines = mem_bit_file.readlines()
        for mem_line in mem_bit_lines[0:-1] :
            mem_line = mem_line.replace("\n",",\n")
            mem_str[mem_filename] += f"\t{mem_line}"
        mem_line = mem_bit_lines[-1].replace("\n","")
        mem_str[mem_filename] += f"\t{mem_line}"

with open(bitstream_tpl_name) as t:
    template = string.Template(t.read())
final_output = template.substitute(\
                                    word_config     =   mem_str[kmem_bit_name],\
                                    machine_code    =   mem_str[imem_bit_name],\
                                    date            =   "Today",\
                                    description     =   "This is a kernel description",\
                                    FILENAME        =   FILENAME,\
                                    filename        =   filename,\
                                    Filename        =   Filename,\
                                    )


with open(bitstream_file_name, "w") as output:
    output.write(final_output)

print( bitstream_file_name + ">>>\n" + final_output )


########################
# Source file generation #
########################

with open(io_file_name) as f:
    io_data = json.loads(f.read())


# Some variables


# In/out Variables
in_vars_str = ""
for in_vars in io_data["inputs"]: # There should only be 1! 
    in_var = f"{in_vars['name']}"
    in_var_depth = f"{in_vars['num']}"
    in_vars_str += f"{in_vars['type']}\t{in_vars['name']}\t[{in_vars['num']}];\n"

out_vars_str = ""
out_var = ""
for out_vars in io_data["outputs"]: # There should only be 1! 
    out_var = f"{out_vars['name']}"
    out_vars_str += f"{out_vars['type']}\t{out_vars['name']}\t[{out_vars['num']}];\n"
    out_vars_str += f"{out_vars['type']}\t{out_vars['name']}_sw\t[{out_vars['num']}];\n"


print("IN vars: >>>\n" + in_vars_str)
print("OUT vars: >>>\n" + out_vars_str)

# input random generation
config_str = ""
for inout in io_data["inputs"]:
    min_val = "0"
    max_val = "0"
    if inout.get("min") : 
        min_val = str(inout.get("min"))
    if inout.get("max") : 
        max_val = str(inout.get("max"))

    config_str += f"\tfor(int i = 0; i < {inout['num']}; i++ )\n"
    config_str += f"\t\t{inout['name']}[i] = kcom_getRand() % ({max_val} - {min_val} + 1) + {min_val};\n"





# Adding the input to the CGRA input
input_max = [0,0,0,0]
for col_num in range(4):
    var_num = 0
    for inout in io_data[f"read_col{col_num}"]:
        config_str += f"\tcgra_input[{col_num}][{var_num}] = {inout['name']};\n"
        var_num += 1
        input_max[col_num] += 1

output_max = [0,0,0,0]
for col_num in range(4):
    for inout in io_data[f"write_col{col_num}"]:
        output_max[col_num] += 1

print( "Config string >>>\n" + config_str )

# sw call
sw_call_str = out_var + " = " + filename + "( " + in_var + " );" 

print("SW call >>>\n" + sw_call_str )


check_str = ""
# Build check
# Output reads
func_return = False
for col_num in range(4):
    val_num = 0
    for inout in io_data[f"write_col{col_num}"] :
        if inout.get("name") != "" : 
            out_col = col_num




with open(source_tpl_name) as t:
    template = string.Template(t.read())

final_output = template.substitute( \
                                    in_vars         = in_vars_str, \
                                    out_vars        = out_vars_str, \
                                    config          = config_str, \
                                    filename        = filename,\
                                    FILENAME        = FILENAME,\
                                    Filename        = Filename,\
                                    out_col         = out_col,\
                                    in_var_depth    = in_var_depth,\
                                    date            = "Today",\
                                    description     = "A description",\
                                    shortname       = filename[:3],\
                                    function        = filename,\
                                    )

print("Sourcefile: >>> \n"+ final_output)

with open(source_file_name, "w") as output:
    output.write(final_output)





with open(header_tpl_name) as t:
    template = string.Template(t.read())

final_output = template.substitute( \
                                    in_vars         = in_vars_str, \
                                    out_vars        = out_vars_str, \
                                    config          = config_str, \
                                    filename        = filename,\
                                    FILENAME        = FILENAME,\
                                    Filename        = Filename,\
                                    out_col         = out_col,\
                                    in_var_depth    = in_var_depth,\
                                    date            = "Today",\
                                    description     = "A description",\
                                    shortname       = filename[:3],\
                                    function        = filename,\
                                    )

print("Header file: >>> \n"+ final_output)

with open(header_file_name, "w") as output:
    output.write(final_output)


