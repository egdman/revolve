# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: msgs/evaluation_result.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
from google.protobuf import descriptor_pb2
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='msgs/evaluation_result.proto',
  package='revolve.msgs',
  serialized_pb=_b('\n\x1cmsgs/evaluation_result.proto\x12\x0crevolve.msgs\"/\n\x10\x45valuationResult\x12\n\n\x02id\x18\x01 \x02(\x05\x12\x0f\n\x07\x66itness\x18\x02 \x02(\x01')
)
_sym_db.RegisterFileDescriptor(DESCRIPTOR)




_EVALUATIONRESULT = _descriptor.Descriptor(
  name='EvaluationResult',
  full_name='revolve.msgs.EvaluationResult',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='id', full_name='revolve.msgs.EvaluationResult.id', index=0,
      number=1, type=5, cpp_type=1, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
    _descriptor.FieldDescriptor(
      name='fitness', full_name='revolve.msgs.EvaluationResult.fitness', index=1,
      number=2, type=1, cpp_type=5, label=2,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      options=None),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  options=None,
  is_extendable=False,
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=46,
  serialized_end=93,
)

DESCRIPTOR.message_types_by_name['EvaluationResult'] = _EVALUATIONRESULT

EvaluationResult = _reflection.GeneratedProtocolMessageType('EvaluationResult', (_message.Message,), dict(
  DESCRIPTOR = _EVALUATIONRESULT,
  __module__ = 'msgs.evaluation_result_pb2'
  # @@protoc_insertion_point(class_scope:revolve.msgs.EvaluationResult)
  ))
_sym_db.RegisterMessage(EvaluationResult)


# @@protoc_insertion_point(module_scope)