from revolve.convert.yaml import yaml_to_robot, YamlToRobot
from revolve.examples.generated_sdf import body_spec, brain_spec

# Load example of a YAML robot:
bot_yaml = file('example_source.yaml', 'r')

# Convert to protoBuf robot:
bot_pb = yaml_to_robot(body_spec, brain_spec, bot_yaml)

pbt = YamlToRobot(body_spec, brain_spec)

outFile = file('example_generated.yaml', 'w')

# Test protoBuf-to-YAML converter:
pbt.get_yaml(bot_pb, outFile)