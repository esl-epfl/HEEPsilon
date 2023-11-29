#!/usr/bin/env python3

# Copyright EPFL contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

# Simplified version of occamygen.py https://github.com/pulp-platform/snitch/blob/master/util/occamygen.py

import argparse
import hjson
import pathlib
import sys
import re
import logging
from subprocess import run
import csv
from jsonref import JsonRef
from mako.template import Template
import collections
from math import *

# Compile a regex to trim trailing whitespaces on lines.
re_trailws = re.compile(r'[ \t\r]+$', re.MULTILINE)

def string2int(hex_json_string):
    return (hex_json_string.split('x')[1]).split(',')[0]

def write_template(tpl_path, outdir, outfile, **kwargs):
    if tpl_path:
        tpl_path = pathlib.Path(tpl_path).absolute()
        if tpl_path.exists():
            tpl = Template(filename=str(tpl_path))
            if outfile == None:
                filename = outdir / tpl_path.with_suffix("").name
            else:
                filename = outfile
            with open(filename, "w") as file:
                code = tpl.render_unicode(**kwargs)
                code = re_trailws.sub("", code)
                file.write(code)
        else:
            raise FileNotFoundError

def main():
    parser = argparse.ArgumentParser(prog="heepsilongen")
    parser.add_argument("--cfg",
                        "-c",
                        metavar="file",
                        type=argparse.FileType('r'),
                        required=True,
                        help="A configuration file")

    parser.add_argument("--outdir",
                        "-of",
                        type=pathlib.Path,
                        required=True,
                        help="Target directory.")

    parser.add_argument("--outfile",
                        "-o",
                        type=pathlib.Path,
                        required=False,
                        help="Target filename, if omitted the template basename is taken.")

    # Parse arguments.
    parser.add_argument("--pkg-sv",
                        metavar="PKG_SV",
                        help="Name of top-level package file (output)")

    parser.add_argument("--tpl-sv",
                        metavar="TPL_SV",
                        help="Name of SystemVerilog template for your module (output)")

    parser.add_argument("--header-c",
                        metavar="HEADER_C",
                        help="Name of header file (output)")

    parser.add_argument("-v",
                        "--verbose",
                        help="increase output verbosity",
                        action="store_true")

    args = parser.parse_args()

    if args.verbose:
        logging.basicConfig(level=logging.DEBUG)

    # Read HJSON description of System.
    with args.cfg as file:
        try:
            srcfull = file.read()
            obj = hjson.loads(srcfull, use_decimal=True)
            obj = JsonRef.replace_refs(obj)
        except ValueError:
            raise SystemExit(sys.exc_info()[1])

    if not args.outdir.is_dir():
            exit("Out directory is not a valid path.")

    outdir = args.outdir
    outdir.mkdir(parents=True, exist_ok=True)

    outfile = args.outfile

    cgra_num_columns = int(obj['cgra']['num_columns'])
    cgra_num_rows = int(obj['cgra']['num_rows'])
    cgra_max_columns = obj['cgra']['max_columns']
    cgra_rcs_num_instr = int(obj['cgra']['rcs_num_instr'])
    cgra_cmem_bk_depth = obj['cgra']['cmem_bk_depth']
    cgra_kmem_depth = obj['cgra']['kmem_depth']

    # Check if value are the default for the CGRA
    if cgra_max_columns == 'default':
        cgra_max_columns = cgra_num_columns
    else:
        cgra_max_columns = int(cgra_max_columns)

    if cgra_cmem_bk_depth == 'default':
        cgra_cmem_bk_depth = cgra_max_columns*cgra_rcs_num_instr
    else:
        cgra_cmem_bk_depth = int(cgra_cmem_bk_depth)

    # Compute the log2 constant of the cmem bank depth for address generation
    cgra_cmem_bk_depth_log2 = int(ceil(log2(cgra_cmem_bk_depth)))
    # Same for the number of instruction per RC
    cgra_rcs_num_instr_log2 = int(ceil(log2(cgra_rcs_num_instr)))
    # Compute kernel configuration word width
    cgra_kmem_width = cgra_max_columns + cgra_cmem_bk_depth_log2 + cgra_rcs_num_instr_log2
    

    kwargs = {
        "cgra_num_columns"        : cgra_num_columns,
        "cgra_num_rows"           : cgra_num_rows,
        "cgra_max_columns"        : cgra_max_columns,
        "cgra_rcs_num_instr"      : cgra_rcs_num_instr,
        "cgra_rcs_num_instr_log2" : cgra_rcs_num_instr_log2,
        "cgra_kmem_depth"         : cgra_kmem_depth,
        "cgra_kmem_width"         : cgra_kmem_width,
        "cgra_cmem_bk_depth"      : cgra_cmem_bk_depth,
        "cgra_cmem_bk_depth_log2" : cgra_cmem_bk_depth_log2
    }

    ###########
    # Package #
    ###########
    if args.pkg_sv != None:
        write_template(args.pkg_sv, outdir, outfile, **kwargs)

    if args.tpl_sv != None:
        write_template(args.tpl_sv, outdir, outfile, **kwargs)

    if args.header_c != None:
        write_template(args.header_c, outdir, outfile, **kwargs)

if __name__ == "__main__":
    main()
