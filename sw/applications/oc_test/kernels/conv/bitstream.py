def estrai_valori(file_path):

    with open(file_path, 'r') as file:
        lines = file.readlines()
    

    kmem_start = [i for i, line in enumerate(lines) if 'kmem:' in line][0]
    imem_start = [i for i, line in enumerate(lines) if 'imem:' in line][0]
    

    kmem_values = " ".join(lines[kmem_start:imem_start]).replace('kmem:', '').replace(',', '').split()
    imem_values = " ".join(lines[imem_start:]).replace('imem:', '').replace(',', '').split()

    return kmem_values, imem_values

def crea_file_c(kmem_values, imem_values, output_file):
    with open(output_file, 'w') as file:
        file.write("const uint32_t cgra_imem_conv[CGRA_IMEM_SIZE] = {" + ", ".join(imem_values) + "};\n")
        file.write("const uint32_t cgra_kmem_conv[CGRA_KMEM_SIZE] = {" + ", ".join(kmem_values) + "};\n")

def main():
    file_di_testo = '/home/nicolo/Documents/GitHub/HEEPsilon/sw/applications/oc_test/kernels/conv/4x4/bitstreams'  # Sostituisci con il percorso corretto del file bitstream
    output_c_file = '/home/nicolo/Documents/GitHub/HEEPsilon/sw/applications/oc_test/kernels/conv/bitstream.h'  # Sostituisci con il percorso desiderato per il file C generato

    kmem_values, imem_values = estrai_valori(file_di_testo)
    crea_file_c(kmem_values, imem_values, output_c_file)

if __name__ == "__main__":
    main()
