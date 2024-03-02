from mako.template import Template
import renderassembly



input_dim = 16
filter_dimension = 3
input_channel = 16
output_channel = 16
first_cycle = 4*(input_dim - 3)
second_cycle = 4*(input_dim - 3)*(input_dim)

data = {
    'input_dim': input_dim,
    'output_row': input_dim-filter_dimension+1,
    'output_col': input_dim-filter_dimension+1,
    'input_channel': input_channel,
    'output_channel': output_channel,
    'first_cycle' : first_cycle, 
    'second_cycle' : second_cycle,
}
# Percorso del file template
template_path = 'conv-template.h'
# Carica il template
template = Template(filename=template_path)
# Percorso del file di output 
output_path = 'conv.h'

# Render del template e scrittura su file
with open(output_path, 'w') as f:
    f.write(template.render(**data))
print(f'File conv.h created/updated')

renderassembly.renderassembly(input_dim, filter_dimension, input_channel, output_channel, first_cycle, second_cycle)

