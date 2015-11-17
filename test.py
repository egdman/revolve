from revolve.convert.yaml import YamlToRobot
from revolve.examples.generated_sdf import body_spec, brain_spec


pbt = YamlToRobot(body_spec, brain_spec)

fi = file('rbt.yaml', 'w')
pbt.get_yaml(fi)