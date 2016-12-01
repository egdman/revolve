#ifndef _REVOLVE_DIRECTION_SENSOR_DUMMY_H_
#define _REVOLVE_DIRECTION_SENSOR_DUMMY_H_

#include "gazebo/sensors/Sensor.hh"
#include "gazebo/util/system.hh"

#include <revolve/gazebo/Types.h>

namespace gazebo {
namespace sensors {
	

class GAZEBO_VISIBLE DirectionSensorDummy : public Sensor
{
	public: DirectionSensorDummy();

	public: virtual ~DirectionSensorDummy();

	public: virtual void Load(const std::string &_worldName, sdf::ElementPtr _sdf);
};

}
}

#endif // _REVOLVE_DIRECTION_SENSOR_DUMMY_H_