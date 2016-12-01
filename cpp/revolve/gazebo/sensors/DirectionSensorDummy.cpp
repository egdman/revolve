#include <revolve/gazebo/sensors/DirectionSensorDummy.h>
#include <iostream>

using namespace gazebo;
using namespace sensors;

// GZ_REGISTER_STATIC_SENSOR("direction", DirectionSensorDummy)

DirectionSensorDummy::DirectionSensorDummy()
: Sensor(sensors::OTHER)
{}

DirectionSensorDummy::~DirectionSensorDummy()
{}