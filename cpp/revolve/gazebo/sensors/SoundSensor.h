#ifndef REVOLVE_GAZEBO_SENSORS_SOUNDSENSOR_H_
#define REVOLVE_GAZEBO_SENSORS_SOUNDSENSOR_H_

#include "Sensor.h"
#include <gazebo/msgs/msgs.hh>
#include "boost/shared_ptr.hpp"

namespace revolve {
namespace gazebo {
	

class SoundSensor : public Sensor
{
public:
	SoundSensor(::gazebo::physics::ModelPtr model, sdf::ElementPtr sensor,
			std::string partId, std::string sensorId);
			
	virtual ~SoundSensor();
	
	/**
	 * Read the value of this sensor into the
	 * input address.
	 */
	virtual void read(double * input);

	/**
	 * Called when the sound sensor is updated
	 */
	void OnUpdate();
	
protected:

	// Transport node
	::gazebo::transport::NodePtr node_;
	
    // Subscriber to SoundPlugin messages:
    ::gazebo::transport::SubscriberPtr soundPluginSub_;

	// Pointer to the update connection
	::gazebo::event::ConnectionPtr updateConnection_;
	
	// Calculate output value based on the position of the sensor and the positions of sound sources
    virtual void calculateOutput(const boost::shared_ptr<::gazebo::msgs::PosesStamped const> &_msg);
private:
	double output_;
};
} // namespace gazebo
} // namespace revolve
	


#endif // REVOLVE_GAZEBO_SENSORS_SOUNDSENSOR_H_
