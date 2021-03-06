from subprocess import check_output
import os
import re
import fnmatch

gazebo_info = check_output('pkg-config gazebo --cflags', shell=True).decode('utf-8')

hit = re.search(r'-I(\S*gazebo\S*).*', gazebo_info)
if not hit:
    sys.exit("Gazebo is not installed")

gazebo_include_dir = hit.group(1)
gazebo_proto_dir = os.path.join(gazebo_include_dir, "gazebo", "msgs", "proto")

package_dir = 'spec'
proto_dir = 'msgs'
generated_dir = os.path.join(package_dir, proto_dir)

# generate protobuf python files
check_output(
    'protoc -I {GAZEBO_PROTO_DIR} -I ./{PKG_DIR} --python_out=./{PKG_DIR} {PKG_DIR}/{PROTO_DIR}/*.proto'
    .format(
        GAZEBO_INCLUDE_DIR = gazebo_include_dir,
        GAZEBO_PROTO_DIR = gazebo_proto_dir,
        PKG_DIR = package_dir,
        PROTO_DIR = proto_dir),
    shell=True)



# fix bullshit include syntax in protobuf-generated python files
generated_files = (f for f in check_output("ls {}".format(generated_dir), shell=True)
    .decode('utf-8').split() if fnmatch.fnmatch(f, "*_pb2.py"))

# all imports of 'import foo_pb2' kind are assumed to be from pygazebo.msg
patt = r'^import\s+(\w*_pb2)'
replace_with = r'from pygazebo.msg import \1'

pygazebo_package_finder = re.compile(patt, re.MULTILINE)

for fname in generated_files:
    fpath = os.path.join(generated_dir, fname)
    with open(fpath, 'r') as stream:
        text = stream.read()

    text = re.sub(pygazebo_package_finder, replace_with, text)
    with open(fpath, 'w') as stream:
        stream.write(text)
