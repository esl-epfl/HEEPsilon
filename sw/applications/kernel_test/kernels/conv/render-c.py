from mako.template import Template
import renderassembly



input_dim = 16
filter_dimension = 3
input_channel = 16
output_channel = 16

data = {
    'input_dim': input_dim,
    'output_row': input_dim-filter_dimension+1,
    'output_col': input_dim-filter_dimension+1,
    'input_channel': input_channel,
    'output_channel': output_channel,
}

template_path = 'conv-template.h'

template = Template(filename=template_path)

output_path = 'conv.h'


with open(output_path, 'w') as f:
    f.write(template.render(**data))
print(f'File conv.h created/updated')

renderassembly.renderassembly(input_dim, filter_dimension, input_channel, output_channel)

