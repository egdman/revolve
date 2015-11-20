from ..spec import BodyImplementation, NeuralNetImplementation
from ..spec.msgs import Body, BodyPart, NeuralNetwork
from ..spec.exception import err

class NeuralNetworkEncoder:
    def __init__(self, spec):
        self.spec = spec
        self.neurons = {}
        self.connections = []

    def parse_neural_network(self, network):
        yaml_network = {}

        neurons = network.neuron
        connections = network.connection

        self._parse_neurons(neurons)
        self._parse_connections(connections)
        yaml_network['neurons'] = self.neurons
        yaml_network['connections'] = self.connections
        return yaml_network


    def _parse_neurons(self, neurons):

        for neuron in neurons:
            neuron_id = neuron.id
            neuron_layer = neuron.layer
            neuron_type = neuron.type
            neuron_part_id = neuron.partId

            if neuron_id in self.neurons:
                err("Duplicate neuron ID '%s'" % neuron_id)

            spec = self.spec.get(neuron_type)
            if spec is None:
                err("Unknown neuron type '%s'" % current["type"])
            neuron_params = spec.unserialize_params(neuron.param)

            self.neurons[neuron_id] = {
                "id": neuron_id,
                "layer": neuron_layer,
                "type": neuron_type,
                "part_id": neuron_part_id,
                "params": neuron_params
            }


    def _parse_connections(self, connections):
        for conn in connections:
            conn_src = conn.src
            conn_dst = conn.dst
            conn_weight = conn.weight

            self.connections.add({
                'src': conn_src,
                'dst': conn_dst,
                'weight': conn_weight
            })





class BodyEncoder:
    def __init__(self, spec):
        self.spec = spec
        self.unique_ids = set()

    # parse protobuf body into a dictionary
    def parse_body(self, body):
        yaml_body = {}
        self._parse_part(yaml_body, body)
        return yaml_body


    def _parse_part(self, yaml_part, part, dst_slot = None):

 #       print part
        part_id = part.id

        # check for duplicate part ids:
        if part_id in self.unique_ids:
            err("Duplicate part ID '%s'" % part_id)

        yaml_part['id'] = part_id
        yaml_part['type'] = part_type = part.type
        yaml_part['orientation'] = part.orientation
        yaml_part['label'] = part.label

        spec = self.spec.get(part_type)

        # check if part type is in spec:
        if spec is None:
            err("Part type '%s' not in implementation spec." % part_type)

        # Check destination slot arity
        if dst_slot is not None and dst_slot >= spec.arity:
            err("Cannot attach part '%s' with arity %d at slot %d" %
                (part_id, spec.arity, dst_slot))

        params = spec.unserialize_params(part.param)
        yaml_part['params'] = params

        connections = part.child
        yaml_part['children'] = {}

        for connection in connections:
            conn_src = connection.src

            if conn_src >= spec.arity:
                err("Cannot attach to slot %d of part '%s' with arity %d." %
                    (conn_src, part_id, spec.arity))

            if conn_src == dst_slot:
                err("Part '%s': Attempt to use slot %d for child which is already "
                    "attached to parent." % (part_id, conn_src))

            self._process_body_connection(connection, yaml_part)





    def _process_body_connection(self, connection, yaml_part):
        conn_src = connection.src
        conn_dst = connection.dst
        conn_part = connection.part

        # add child to parent:
        yaml_part['children'][conn_src] = {}
        yaml_child_part = yaml_part['children'][conn_src]

        # set child part:
        self._parse_part(yaml_child_part, conn_part, conn_dst)
        yaml_child_part['slot'] = conn_dst





