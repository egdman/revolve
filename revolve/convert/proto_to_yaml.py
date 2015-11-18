from ..spec import BodyImplementation, NeuralNetImplementation
from ..spec.msgs import Body, BodyPart, NeuralNetwork
from ..spec.exception import err


class BodyEncoder:
    def __init__(self, spec):
        self.spec = spec
        self.unique_ids = set()

    # parse protobuf body into a dictionary
    def parse_body(self, body):
        yaml_body = {}
        self.parse_part(yaml_body, body)
        return yaml_body


    def parse_part(self, yaml_part, part, dst_slot = None):

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

        children = part.child
        yaml_part['children'] = {}

        for connection in children:
            conn_src = connection.src
            conn_dst = connection.dst
            conn_part = connection.part

            if conn_src >= spec.arity:
                err("Cannot attach to slot %d of part '%s' with arity %d." %
                    (conn_src, part_id, spec.arity))

            if conn_src == dst_slot:
                err("Part '%s': Attempt to use slot %d for child which is already "
                    "attached to parent." % (part_id, conn_src))

            self.process_body_connection(connection, yaml_part)





    def process_body_connection(self, connection, yaml_part):
        conn_src = connection.src
        conn_dst = connection.dst
        conn_part = connection.part

        # add child to parent:
        yaml_part['children'][conn_src] = {}
        yaml_child_part = yaml_part['children'][conn_src]

        # set child part:
        self.parse_part(yaml_child_part, conn_part, conn_dst)
        yaml_child_part['slot'] = conn_dst





