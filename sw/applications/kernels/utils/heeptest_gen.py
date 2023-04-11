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

if len(sys.argv) > 1 :
    config_path = sys.argv[1]
else:
    sys.exit("[ERROR] No path specified")

build_main = False
if len(sys.argv) > 2 :
    if sys.argv[2] == "m" :
        build_main = True
    else :
        sys.exit(f"[ERROR] Invalid option '{sys.argv[2]}', set 'm' for build main.c or leave it empty")

# Templates:
header_tpl_name = "utilities/test_header.h.tpl"
main_tpl_name = "utilities/test_main.c.tpl"

# Files being generated:
header_file_name = str(config_path) + "cgra_bitstream.h"
main_file_name = str(config_path) + "main.c"

# Bitstream:
imem_bit_name = str(config_path) + "cgra_imem.bit"
kmem_bit_name = str(config_path) + "cgra_kmem.bit"
bit_info_file_name = str(config_path) + "bit_info.json"
io_file_name = str(config_path) + "io.json"

if not os.path.exists(imem_bit_name) :
    sys.exit("[ERROR] No bitstream detected in " + str(imem_bit_name))

if not os.path.exists(kmem_bit_name) :
    sys.exit("[ERROR] No bitstream detected in " + str(kmem_bit_name))

if not os.path.exists(bit_info_file_name) :
    sys.exit("[ERROR] No bitstream info detected in " + str(bit_info_file_name))

imem_bit_file = open(imem_bit_name , 'r')
kmem_bit_file = open(kmem_bit_name , 'r')
with open(bit_info_file_name) as f:
    bit_info_data = json.loads(f.read())

##########################
# Header file generation #
##########################

# Kernel id
kernel_str = ""
kernel_num = 1
for kernel_id in bit_info_data["kernels"]:
    kernel_str += f"#define {kernel_id.upper()}_KER_ID {kernel_num}\n"
    kernel_num += 1

#kmem
kmem_str = ""
kmem_bit_lines = kmem_bit_file.readlines()
for kmem_line in kmem_bit_lines[0:-1] :
    kmem_line = kmem_line.replace("\n",",\n")
    kmem_str += f"\t{kmem_line}"
kmem_line = kmem_bit_lines[-1].replace("\n","")
kmem_str += f"\t{kmem_line}"

# imem
imem_str = ""
imem_bit_lines = imem_bit_file.readlines()
for imem_line in imem_bit_lines[0:-1] :
    imem_line = imem_line.replace("\n",",\n")
    imem_str += f"\t{imem_line}"
imem_line = imem_bit_lines[-1].replace("\n","")
imem_str += f"\t{imem_line}"

with open(header_tpl_name) as t:
    template = string.Template(t.read())

final_output = template.substitute(kernel_id=kernel_str,word_config=kmem_str ,machine_code=imem_str )
with open(header_file_name, "w") as output:
    output.write(final_output)

########################
# Main file generation #
########################

if build_main :

    with open(io_file_name) as f:
        io_data = json.loads(f.read())

    # input random generation
    rng_input_str = ""
    for inout in io_data["inputs"]:
        min_val = "0"
        max_val = "UINT_MAX - 1"
        if inout.get("min") : 
            min_val = str(inout.get("min"))
        if inout.get("max") : 
            max_val = str(inout.get("max"))
        if inout['num'] > 1 :
            rng_input_str += f"{inout['type']} {inout['name']}[{inout['num']}];\n\t\t"
            rng_input_str += f"{inout['type']} {inout['name']}_sw[{inout['num']}];\n\t\t"
            rng_input_str += f"for(int i=0 ; i < {inout['num']} ; i++)\n\t\t\t"
            rng_input_str += f"{inout['name']}_sw[i] = {inout['name']}[i] = rand() % ({max_val} - {min_val} + 1) + {min_val};\n\t\t"
        else :
            rng_input_str += f"{inout['type']} {inout['name']} = rand() % ({max_val} - {min_val} + 1) + {min_val};\n\t\t"

    rng_input_str += "\n\t\t"

    # input random generation
    input_max = [0,0,0,0]
    for col_num in range(4):
        var_num = 0
        for inout in io_data[f"read_col{col_num}"]:
            rng_input_str += f"cgra_input[{col_num}][{var_num}] = {inout['name']};\n\t\t"
            var_num += 1
            input_max[col_num] += 1

    output_max = [0,0,0,0]
    for col_num in range(4):
        for inout in io_data[f"write_col{col_num}"]:
            output_max[col_num] += 1

    input_max_single = max(input_max)
    output_max_single = max(output_max)

    if input_max_single < 1 :
        input_max_single += 1
    if output_max_single < 1 :
        output_max_single += 1

    input_max_str = str(input_max_single)
    output_max_str = str(output_max_single)

    # Kernel id always to one
    kernel_id_str = "1"

    # sw call
    sw_call_str = ""
    check_str = ""
    for var in io_data["outputs"] :
        if var["name"] == "return" :
            if var["num"] > 1 :
                sw_call_str += f"{var['type']} * res_sw;\n"
                check_str += f"{var['type']} res_cgra[{var['num']}];\n"
            else :
                sw_call_str += f"{var['type']} res_sw;\n"
                check_str += f"{var['type']} res_cgra;\n"
            sw_call_str += "\t\tres_sw = "
            break
    
    sw_call_str += f"{io_data['function_name']} ( "
    for var in io_data["inputs"][0:-1] :
        if var["num"] > 1 :
            sw_call_str += var["name"] + "_sw , "
        else :
            sw_call_str += var["name"] + " , "
    
    if io_data["inputs"][-1]["num"] > 1 :
        sw_call_str +=  io_data["inputs"][-1]["name"] + "_sw );\n"
    else :
        sw_call_str +=  io_data["inputs"][-1]["name"] + " );\n"

    # Build check
    # Output reads
    func_return = False
    for col_num in range(4):
        val_num = 0
        for inout in io_data[f"write_col{col_num}"] :
            if inout.get("name") == "return" : 
                func_return = True
                check_str += f"\t\tres_cgra = cgra_output[{col_num}][{val_num}];\n"
            else:
                check_str += f"\t\tres_cgra[{inout['id']}] = cgra_output[{col_num}][{val_num}];\n"
                val_num += 1

    # Output checks: return and Arrays
    for inout in io_data["outputs"]:
        if inout['name'] == "return":
            if inout['num'] > 1 :
                check_str += f"\t\tfor(int i=0 ; i < {inout['num']} ; i++)\n"
                check_str += "\t\t{\n"
                check_str += f"\t\t\tif (res_cgra[i] != res_sw[i])\n"
                check_str += f"\t\t\t\terrors++;\n"
                check_str += "\t\t}\n"
            else:
                check_str += "\t\tif (res_cgra != res_sw) {\n"
                check_str += "\t\t\terrors++;\n"
                check_str += "\t\t}\n\t\t"
        else:
            if inout['num'] > 1 :
                check_str += f"for(int i=0 ; i < {inout['num']} ; i++)\n"
                check_str += "\t\t{\n"
                check_str += f"\t\t\tif ({inout['name']}[i] != {inout['name']}_sw[i])\n"
                check_str += f"\t\t\t\terrors++;\n"
                check_str += "\t\t}\n"

    with open(main_tpl_name) as t:
        template = string.Template(t.read())

    final_output = template.substitute(input_max=input_max_str , output_max=output_max_str , rng_input=rng_input_str , sw_call=sw_call_str , kernel_id=kernel_id_str , check=check_str)
    with open(main_file_name, "w") as output:
        output.write(final_output)

