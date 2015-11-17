"""
"""
from __future__ import absolute_import
import yaml
from ..spec import BodyImplementation, NeuralNetImplementation
from ..spec.msgs import *
from .decode import BodyDecoder, NeuralNetworkDecoder


def yaml_to_robot(body_spec, nn_spec, yaml):
    """
    :param body_spec:
    :type body_spec: BodyImplementation
    :param nn_spec:
    :type nn_spec: NeuralNetImplementation
    :param yaml:
    :type yaml: stream
    :return:
    """
    obj = YamlToRobot(body_spec, nn_spec)
    return obj.get_protobuf(yaml)


class YamlToRobot:
    """
    Sample converter creates a Robot protobuf message
    from a YAML stream and a body / neural net spec.
    """

    def __init__(self, body_spec, nn_spec):
        """
        :param body_spec:
        :type body_spec: BodyImplementation
        :return:
        """
        self.body_spec = body_spec
        self.nn_spec = nn_spec
        self.body_decoder = BodyDecoder(body_spec)
        self.brain_decoder = NeuralNetworkDecoder(nn_spec, body_spec)

    def get_protobuf(self, stream):
        """
        Returns a protobuf `Robot` for the given stream.

        :param stream:
        :type stream: stream
        :return:
        :rtype: Robot
        """
        obj = yaml.load(stream)

        robot = Robot()
        robot.id = obj.get('id', 0)
        robot.body.CopyFrom(self.body_decoder.decode(obj))
        robot.brain.CopyFrom(self.brain_decoder.decode(obj))
        return robot


# testing:
 #   def get_yaml(selfself, pbRobot):
    def get_yaml(self, stream):

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

        bot_pb = yaml_to_robot(self.body_spec, self.nn_spec, bot_yaml)
        body = bot_pb.body
        brain = bot_pb.brain
        yaml.dump_all([body, brain], stream)
        return 0