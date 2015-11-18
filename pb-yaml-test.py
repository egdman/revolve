from revolve.convert.yaml import yaml_to_robot, YamlToRobot
from revolve.examples.generated_sdf import body_spec, brain_spec

bot_yaml = '''
---
!!map {
  ? !!str "body"
  : !!map {
    ? !!str "children"
    : !!map {
      ? !!int "1"
      : !!map {
        ? !!str "id"
        : !!str "Hinge",
        ? !!str "params"
        : !!map {
          ? !!str "blue"
          : !!float "0.0",
          ? !!str "green"
          : !!float "0.0",
          ? !!str "length"
          : !!float "0.5",
          ? !!str "red"
          : !!float "1.0",
        },
        ? !!str "type"
        : !!str "Hinge",
      },
      ? !!int "4"
      : !!map {
        ? !!str "id"
        : !!str "Wheel",
        ? !!str "params"
        : !!map {
          ? !!str "blue"
          : !!float "0.0",
          ? !!str "green"
          : !!float "1.0",
          ? !!str "red"
          : !!float "0.0",
        },
        ? !!str "type"
        : !!str "Wheel",
      },
      ? !!int "5"
      : !!map {
        ? !!str "id"
        : !!str "Wheel2",
        ? !!str "params"
        : !!map {
          ? !!str "blue"
          : !!float "0.0",
          ? !!str "green"
          : !!float "1.0",
          ? !!str "red"
          : !!float "0.0",
        },
        ? !!str "type"
        : !!str "Wheel",
      },
    },
    ? !!str "id"
    : !!str "Core",
    ? !!str "type"
    : !!str "Core",
  },
  ? !!str "brain"
  : !!map {
    ? !!str "params"
    : !!map {
      ? !!str "Wheel-out-0"
      : !!map {
        ? !!str "period"
        : !!int "3",
        ? !!str "type"
        : !!str "Oscillator",
      },
      ? !!str "Wheel2-out-0"
      : !!map {
        ? !!str "period"
        : !!int "3",
        ? !!str "type"
        : !!str "Oscillator",
      },
    },
  },
}
        '''

# EXAMPLE OF A PROTOBUF ROBOT:
bot_pb = yaml_to_robot(body_spec, brain_spec, bot_yaml)

pbt = YamlToRobot(body_spec, brain_spec)

outFile = file('example_generated.yaml', 'w')
pbt.get_yaml(bot_pb, outFile)