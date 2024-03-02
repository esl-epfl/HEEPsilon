'''
    File name: io_gen.py
    Author: Juan Sapriza
    Date created: 22/06/2023
    Python Version: Python 3.8.16
    Description: generates a io.json file from the inouts description and a SAT-MapIt output file.
'''

POSITION_NODE   = 0
POSITION_DIR    = 1
POSITION_NAME   = 2
POSITION_DEPTH  = 3
POSITION_TYPE   = 4

with open( KER_PATH + "inouts", 'r') as f:
    inouts = f.readlines()
inouts = [l.replace('\n','').split() for l in inouts]

# USI will tell us the number of coulmns used
with open(OUT_SAT_PATH,"r") as f:                                                         
    usi_ASM = f.readlines()                                                          

# Start from the USI compiler output and extract only the pseudo-assembly 
usi_ASM = usi_ASM[ usi_ASM.index("Output of the mapping with node id\n")+1 : ]
#usi_ASM = usi_ASM[ : usi_ASM.index("\nId:") ]

timestamps = []
beg = 0
end = 0

startline   = usi_ASM[ 1 ]
endline     = usi_ASM[ 1 + 2*CGRA_N_ROW] 
 
while 1: 
    try: 
        beg = end + usi_ASM[end:].index(startline)
        end = beg + 1 + usi_ASM[beg+1:].index(endline)
        timestamps.append(usi_ASM[beg+1:end])
    except:
        break

tss = []
for time in timestamps:
    ts = []
    for line in time:
        t = [int(s) for s in line.split() if s.lstrip("-").isdigit() ]
        if t:
            ts.append(t)
    tss.append(ts)

iodict = {}
iodict["function_name"] = KER_NAME

values= [ int(v[0]) for v in inouts ]

iodict["inputs"]    = []
iodict["outputs"]   = []

for line in inouts:
    dst = "inputs" if "in" in line else "outputs"
    type = line[POSITION_TYPE]
    type = "uint32_t" if type == "var" else type
    iodict[dst].append( {"name": line[POSITION_NAME], "depth": int(line[POSITION_DEPTH]), "type":type} )

for col in range(len(tss[0])):
    iodict["read_col" + str(col) ]   = []  
    iodict["write_col" + str(col) ]  = []

for ts in tss:
    for row in ts: 
        for col in range(len(row)):
            if row[col] in values:
                dst = "read_col" if inouts[ values.index(row[col]) ][1] == "in" else "write_col"
                iodict[dst + str(col) ].append( {"name" : inouts[values.index(row[col])][2] } )


iojson = json.dumps(iodict, indent=4)
with open( DATA_DIR + "io.json", 'w' ) as f:
    f.write(iojson)

