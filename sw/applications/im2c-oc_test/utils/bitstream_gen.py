'''
    File name: bitstream_gen.py
    Author: Clement DUBOS
    Date created: 19/04/2022
    Python Version: Python 3.10.4
    Description: Encode instructions for the CGRA from usi pseudo asm
'''

# import sys
import copy
# from pickle import ADDITEMS

# WALL = True        #Set to 'True' to get all warnings at run time

# # Change to the file name you want to compile
# FILE_NAME = "pseudo_assembly_dbl_max"

SIZE_DEC_INSTR  = 5
SIZE_EPFL_ASM   = 6

type_1_instr        = ['SADD','SSUB','SMUL','FXPMUL','SLT','SRT','SRA',
                        'LAND','LOR','LXOR','LNAND','LNOR','LXNOR']
type_2_instr        = ['BSFA', 'BZFA']
type_3_instr        = ['BEQ','BNE','BLT','BGE']
type_5_instr        = ['LWD','SWD']
type_6_instr        = ['LWI','SWI']

muxA_list_ext = np.concatenate((muxA_list , ["ROUT"]),axis=0)
muxB_list_ext = np.concatenate((muxB_list , ["ROUT"]),axis=0)

def print_w(string) :
    '''
        Print only if WALL == True

        Parameter
        -----------------------------
        String

        Return
        -----------------------------
        String if WALL == True
    '''
    if (WALL) :
        print(string)
    return

def string_instr(instr) :
    '''
        Return instruction as a string

        Parameter
        -----------------------------
        instruction (list of strings)

        Return
        -----------------------------
        instruction (string)
    '''
    string = '['+instr[0]+' '+instr[1]+' '+instr[2]+' '+instr[3]+']'
    return string

def time_lines (lines) :
    '''
        Reorganize the instructions in a timed order for each PE

        Parameter
        -----------------------------
        list of the lines(string) of bit_count_pseudo_assembly

        Return
        -----------------------------
        list of list of string
    '''
    cnt = 0
    timed_lines = [[] for _ in range (PE_NBR)]
    for line in lines :
        if line == "\n" :
            cnt = 0
        else :
            if (line[-1] == '\n') :
                timed_lines[cnt].append(line[:-1])
            else :
                timed_lines[cnt].append(line[:])
            cnt += 1
    return timed_lines

def decode_instruction(instruction) :
    '''
        Separate operation and operators of instruction

        Parameter
        -----------------------------
        instruction (string)

        Return
        -----------------------------
        separated instruction (list of strings)
    '''
    instr = ['' for _ in range(SIZE_DEC_INSTR)]
    case_list = ['Operation','OP1','OP2','OP3','OP4','END']
    case = case_list[0]
    once = False
    for char in instruction :
        if case == 'Operation' :
            if char != ' ' :
                instr[0] += char
            else :
                case = case_list[1]
        elif case == 'OP1' :
            if (char == ' ') or (char == ',') :
                    case = case_list[2]
                    once = True
            else :
                instr[1] += char
        elif case == 'OP2' :
            if (char == ' ') or (char == ',') :
                if not once :
                    case = case_list[3]
                    once = True
                else :
                    once = False
            else :
                instr[2] += char
        elif case == 'OP3' :
            if (char == ' ') or (char == ',') :
                if not once :
                    case = case_list[4]
                    once = True
                else :
                    once = False
            else :
                instr[3] += char
        elif case == 'OP4' :
            if (char == ' ') or (char == ',') :
                if not once :
                    case = case_list[5]
                    once = True
                else :
                    once = False
            else :
                instr[4] += char
        elif case == 'END' :
            return instr
    if (instr[0] == 'MV') :
        instr[0] = 'SADD'
        instr[1],instr[2] = instr[2],instr[1]
    elif (instr[0] == 'SUB') :
        instr[0] = 'SSUB'
    elif (instr[0] == 'ADD') :
        instr[0] = 'SADD'
    elif (instr[0] == 'MUL') :
        instr[0] = 'SMUL'
    elif (instr[0] == 'DIV') :
        instr[0] = 'SDIV'
    instr[1] = 'ZERO' if instr[1] == '0' else instr[1]
    instr[2] = 'ZERO' if instr[2] == '0' else instr[2]
    for i in range(SIZE_DEC_INSTR) :
        if instr[i] == 'ROUTA' :
            instr[i] = 'ROUT'
        if (instr[i] == 'Rout') or (instr[i] == 'ROUT') :
            instr[i] = 'ROUT'
    for i in range (SIZE_DEC_INSTR) :
        if (instr[i] == '') :
            instr[i] = '-'
    return instr

def translate_instructions(instr_usi) :
    '''
        Gives us the equivalent instruction in epfl assembly

        Parameter
        -----------------------------
        separated instruction (list of strings)

        Return
        -----------------------------
        instruction (string)
    '''
    instr_epfl = ['' for _ in range (SIZE_EPFL_ASM)]
    op = instr_usi[0]
    check = False

    if op == 'NOP' :
        instr_epfl = rcs_nop_instr
    elif op in type_1_instr :
        instr_epfl=[instr_usi[2] if (instr_usi[2] in muxA_list_ext) else "ZERO",\
                    instr_usi[3] if (instr_usi[3] in muxB_list_ext) else "ZERO",op,instr_usi[1],\
                    '-','-']

        if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
            instr_epfl[-1] = instr_usi[2]
            instr_epfl[0] = 'IMM'
            check = True

        if instr_usi[3] not in muxB_list_ext and instr_usi[3] not in ['-','ROUT'] :
            if check :
                sys.exit("Can't have two immediates as input : "+ string_instr(instr_usi) + " -> " + string_instr(instr_epfl))
            instr_epfl[-1] = instr_usi[3]
            instr_epfl[1] = 'IMM'
    elif op in type_3_instr :
        instr_epfl=[instr_usi[1],instr_usi[2] if (instr_usi[2] in muxA_list_ext) else "ZERO",op,'-',\
                    '-',instr_usi[3]]

        if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
            sys.exit("Can't have immediates as operand B : "+ string_instr(instr_usi) + " -> " + string_instr(instr_epfl))
    elif op in type_2_instr :
        instr_epfl=[instr_usi[2] if (instr_usi[2] in muxA_list_ext) else "ZERO",\
                    instr_usi[3] if (instr_usi[3] in muxB_list_ext) else "ZERO",op,instr_usi[1],\
                    instr_usi[4],"-"]
        if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
            instr_epfl[-1] = instr_usi[2]
            check = True
        elif instr_usi[3] not in muxB_list_ext and instr_usi[3] not in ['-','ROUT'] :
            if check :
                sys.exit("Can't have two immediates as input : "+ string_instr(instr_epfl))
            instr_epfl[-1] = instr_usi[3]
    elif op in type_6_instr :
        if op == 'INA' :
            instr_epfl=[instr_usi[2],'-',op,instr_usi[1],'-','-']

            if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
                instr_epfl[-1] = instr_usi[2]
                instr_epfl[0] = 'IMM'
        elif op == 'INB' :
            instr_epfl=['-',instr_usi[2],op,instr_usi[1],'-','-']

            if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
                instr_epfl[-1] = instr_usi[2]
                instr_epfl[1] = 'IMM'

        else : # LWI, SWI
            if op in ['LWI'] :
                instr_epfl=['-',instr_usi[2] if (instr_usi[2] in muxA_list_ext) else "ZERO",op,instr_usi[1],\
                    '-','-']

                if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
                    instr_epfl[-1] = instr_usi[2]
                    instr_epfl[1] = 'IMM'
            else : # SWI
                instr_epfl=[instr_usi[1],instr_usi[2] if (instr_usi[2] in muxA_list_ext) else "ZERO",op,'-',\
                    '-','-']

                if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
                    instr_epfl[-1] = instr_usi[2]
                    instr_epfl[1] = 'IMM'

    elif op in type_5_instr :
        if op == 'SWD' :
            instr_epfl = [instr_usi[1],'-',op,'-','-','-']
        else :
            instr_epfl = ['-','-',op,instr_usi[1],'-','-']
    elif op == 'JUMP' :
        instr_epfl=[instr_usi[2] if (instr_usi[2] in muxA_list_ext) else "ZERO",\
                    instr_usi[3] if (instr_usi[3] in muxB_list_ext) else "ZERO",'JUMP',\
                    '-','-','-']

        if instr_usi[2] not in muxA_list_ext and instr_usi[2] not in ['-','ROUT'] :
            instr_epfl[-1] = instr_usi[2]
            instr_epfl[0] = 'IMM'
            check = True

        if instr_usi[3] not in muxB_list_ext and instr_usi[3] not in ['-','ROUT'] :
            if check :
                sys.exit("Can't have two immediates as input : "+ string_instr(instr_epfl))
            instr_epfl[-1] = instr_usi[3]
            instr_epfl[1] = 'IMM'
    elif op == 'EXIT' :
        instr_epfl = ['-','-','EXIT','-','-','-']
    else :
        sys.exit("Line doesn't correspond to any known instruction : " + string_instr(instr_usi))

    for i in [0,1] :
        if instr_epfl[i] == 'ROUT' :
            instr_epfl[i] = 'SELF'

    if instr_epfl[4] == 'ROUT' :
        instr_epfl[4] = 'SELF'

    if instr_epfl[3] == 'ROUT' :
            instr_epfl[3] = '-'

    if instr_epfl[-1] == 'ONE' :
            instr_epfl[-1] = '1'

    if intr_log :
        print(instr_epfl)

    return instr_epfl

def translate_usi_asm(sched) :
    '''
        Translate usi pseudo-assembly to epfl cgra assembly

        Parameter
        -----------------------------
        schedule of the execution (list of list of string) in usi pseudo-assembly

        Return
        -----------------------------
        schedule of the execution (list of list of string) in epfl cgra assembly
    '''
    new_sched = [[['' for _ in range(SIZE_EPFL_ASM)] for _ in range(len(sched[0]))] for _ in range(PE_NBR)]
    i = 0
    j = 0
    for pe in sched :
        if intr_log :
            print('Cell ' , i)
        for instruction in pe :

            new_sched[i][j] = translate_instructions(decode_instruction(instruction))
            #print(new_sched[i][j])
            j += 1
        j = 0
        i += 1

    return new_sched

def transpose_grid(usi_trans, n_col) :
    '''
        Transpose the PE grid because epfl doesn't use the same convention as usi

        Parameter
        -----------------------------
        Translated usi pseudo-assembly in epfl format (list of list of string)

        Return
        -----------------------------
        epfl format assembly (list of list of string)
    '''
    epfl_asm = copy.copy(usi_trans)

    for i in range(CGRA_N_ROW):
        for j in range(n_col):
            epfl_asm[j*CGRA_N_COL+i] = usi_trans[i*n_col+j]

    return epfl_asm

def set_ker_conf_word(code,n_col) :
    '''
        Set the kernel configuration word

        Parameter
        -----------------------------
        the translated and reformated cgra asm instruction (list of list of string)

        Return
        -----------------------------
        the kernel configuration word, k, kernel start aderss
    '''
    nbr_col = n_col # Columns used
    ker_num_instr = len(code[0]) # Number of instruction in the kernel
    ker_conf_w = get_bin(int(pow(2,nbr_col))-1,CGRA_N_COL) +\
                                get_bin(kernel_start, CGRA_IMEM_NL_LOG2) +\
                                get_bin(ker_num_instr-1, RCS_NUM_CREG_LOG2)
    # Used for multi-column kernels
#    k = ker_num_instr
    return ker_conf_w,nbr_col

def create_rcs_instructions(start_add,rcs_instructions,epfl_ASM,nbr_instr,col_nbr) :
    '''
        Convert epfl_asm to rcs_instruction format

        Parameter
        -----------------------------
        epfl_asm (list of list of list of string)

        Return
        -----------------------------
        rcs_instructions (list of list of list of string)
    '''   #zegmanek
    for l in range(col_nbr) :
        for j in range (CGRA_N_ROW) :
            for i in range(nbr_instr) :
                rcs_instructions[j][start_add+nbr_instr*l+i] = epfl_ASM[j+CGRA_N_ROW*l][i]
                # print(rcs_instructions[j][start_add+nbr_instr*l+i])
    for l in range(col_nbr,CGRA_N_COL) :
        for j in range (CGRA_N_ROW) :
            for i in range(nbr_instr) :
                rcs_instructions[j][start_add+nbr_instr*l+i] = rcs_nop_instr
    return rcs_instructions

def used_col(usi_ASM) :
    '''
        Check if the format is all the PEs at a time (fomat_line) or only the collumns used (format_col)

        Parameter
        -----------------------------
        usi_ASM (list of string)

        Return
        -----------------------------
        int
    '''
    cnt = 0
    for line in usi_ASM :
        if line != '\n' :
            cnt += 1
        else :
            break
    if cnt == 0 :
        sys.exit("ERROR : No instruction")
    for i in range(1,CGRA_N_COL+1) :
        if cnt == i*CGRA_N_ROW :
            return i
    sys.exit("ERROR : Column half-defined")

'''--------------------------------
CODE
--------------------------------'''
# USI will tell us the number of coulmns used
with open(OUT_SAT_PATH,"r") as f:                                                          # Open the pseudo-assembly file
    usi_ASM = f.readlines()                                                          # Store each line
# cgra_n_col = used_col(usi_ASM)                                                              # Return the number of column

# Start from the USI ompiler output and extract only the pseudo-assembly (between "T = 0\n"s)
usi_ASM = usi_ASM[ usi_ASM.index("T = 0\n")+1 : ]
usi_ASM = usi_ASM[ : usi_ASM.index("T = 0\n") ]
# Replace all time stamps for empty lines
usi_ASM = [ "\n" if "T = " in s else s for s in usi_ASM ]
# Add the exit instruction at the end
usi_ASM.append("\n")
usi_ASM.append("EXIT\n")
# Fill the rest of the instruction with NOPs
[ usi_ASM.append("NOP\n") for _ in range(usi_ASM.index("\n")-1)]

cgra_n_col = CGRA_N_COL
usi_ASM_timed = time_lines(usi_ASM)                                                         # Convert theformat for it being time wise for each PE(RC)
usi_translated = translate_usi_asm(usi_ASM_timed)                                           # Convert to epfl_asm format
epfl_ASM = transpose_grid(usi_translated, cgra_n_col) # if cgra_n_col == 4 else usi_translated            # Transpose the confiuration of the PEs if format_line else under 16 PEs we have a format_col
start_add = kernel_start                                                                   # Save current start address
k = len(epfl_ASM[0])                                                                        # Used for multi-column kernels
ker_conf_words[KERNEL_ID],nbr_col = set_ker_conf_word(epfl_ASM,cgra_n_col)                # Set the configuartion word of the kernel
kernel_start = kernel_start + k*nbr_col                                                   # Update start address for next kernel
KERNEL_ID += 1                                                                            # Update ID for next kernel
rcs_instructions = create_rcs_instructions(start_add,rcs_instructions,epfl_ASM,k,nbr_col)   # Convert it to rcs_instruction to be read by inst_encoder.py
