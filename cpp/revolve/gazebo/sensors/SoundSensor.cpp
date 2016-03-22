#include "SoundSensor.h"

#include <iostream>
#include <stdexcept>


namespace gz = gazebo;

namespace revolve {
namespace gazebo {
	
SoundSensor::SoundSensor(::gazebo::physics::ModelPtr model, sdf::ElementPtr sensor,
		std::string partId, std::string sensorId):
        Sensor(model, sensor, partId, sensorId, 1) // last parameter is the number of input neurons this sensor generates
		
{
	this->output_ = 0.0;
	// Create transport node
	node_.reset(new gz::transport::Node());
	node_->Init();
	
	// subscribe to sound plugin messages
    soundPluginSub_ =  node_->Subscribe("~/revolve/sound_source_poses", &SoundSensor::calculateOutput, this);
	
	// connect to the update signal
	this->updateConnection_ = this->sensor_->ConnectUpdated(boost::bind(&SoundSensor::OnUpdate, this));
}

SoundSensor::~SoundSensor() {}

void SoundSensor::OnUpdate()
{
	return;
}

void SoundSensor::calculateOutput(const boost::shared_ptr<::gazebo::msgs::PosesStamped const> &_msg)
{
	if (_msg->pose_size() <= 0) {
		output_ = 0.0;
		return;
	}
	else {
		std::vector<gz::math::Vector3> srcPositions;
		for (int i = 0; i < _msg->pose_size(); ++i) {
			gz::msgs::Pose poseMsg = _msg->pose(i);
			gz::msgs::Vector3d position = poseMsg.position();
			
			srcPositions.push_back(gz::math::Vector3(position.x(), position.y(), position.z()));

		}
		
        // this is the relative pose of sensor with respect to parent link:
        ::gz::math::Pose sensorPose = this->sensor_->GetPose();

        std::string parentLinkName = this->sensor_->GetParentName();
        ::gz::physics::LinkPtr link = this->model_->GetLink( parentLinkName );

        if (!link) {
            std::string errMes("Sound sensor: could not find the link: ");
            errMes.append(parentLinkName);
            throw std::runtime_error(errMes);
        }

        // this is the absolute pose of the parent link:
        ::gz::math::Pose linkPose = link->GetWorldCoGPose();

        ::gz::math::Pose absSensorPose = linkPose + sensorPose;
        ::gz::math::Vector3 sensorPosition = absSensorPose.pos;
        ::gz::math::Quaternion sensorRotation = absSensorPose.rot;

        // for now sensor orientation does not matter, this is temporary
		output_ = 0.0;
        for (unsigned int i = 0; i < srcPositions.size(); ++i) {
			gz::math::Vector3 srcPos = srcPositions[i];
			double dist = sensorPosition.Distance(srcPos);
			double distSq = dist * dist;
			double intensity = 1.0 / (distSq + 0.0001);
            output_ += intensity;
		}
	}
}

void SoundSensor::read(double * input) {
	input[0] = this->output_;
}

	
} // namespace gazebo
} // namespace revolve