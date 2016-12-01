#include "DirectionSensor.h"

#include <iostream>
#include <stdexcept>


namespace gz = gazebo;

namespace revolve {
namespace gazebo {
	
DirectionSensor::DirectionSensor(::gazebo::physics::ModelPtr model, sdf::ElementPtr sensor,
		std::string partId, std::string sensorId):
        Sensor(model, sensor, partId, sensorId, 1) // last parameter is the number of input neurons this sensor generates
		
{
	this->output_ = 0.0;
	// Create transport node
	node_.reset(new gz::transport::Node());
	node_->Init();
	
	// subscribe to sound plugin messages
    soundPluginSub_ =  node_->Subscribe("~/revolve/sound_source_poses", &DirectionSensor::calculateOutput, this);
	
	// connect to the update signal
	this->updateConnection_ = this->sensor_->ConnectUpdated(boost::bind(&DirectionSensor::OnUpdate, this));

    // this is the relative pose of this sensor in the coordinate system of the parent link
    this->sensorPose_ = this->sensor_->GetPose();

    // this is the sensor axis in the link coordinate system:
    this->sensorAxis_ = this->sensorPose_.rot.RotateVector(::gz::math::Vector3(0, 0, 1));

    // name of the parent link
    std::string parentLinkName = this->sensor_->GetParentName();

    // ptr to the parent link
    this->linkPtr_ = this->model_->GetLink( parentLinkName );
    if (!(this->linkPtr_)) {
        std::string errMes("Sound sensor: could not find the link: ");
        errMes.append(parentLinkName);
        throw std::runtime_error(errMes);
    }




}

DirectionSensor::~DirectionSensor() {}

void DirectionSensor::OnUpdate()
{
	return;
}

void DirectionSensor::calculateOutput(const boost::shared_ptr<::gazebo::msgs::PosesStamped const> &_msg)
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
		
        // this is the absolute pose of the parent link in the world coordinate system:
        ::gz::math::Pose linkPose = this->linkPtr_->GetWorldCoGPose();

        // this is the relative position of the sensor in the world coordinate system:
        ::gz::math::Vector3 sensorRelPosRot = linkPose.rot.RotateVector((this->sensorPose_).pos);

        // absolute position of the sensor in the world coordinate system:
        ::gz::math::Vector3 sensorAbsPos = linkPose.pos + sensorRelPosRot;

        // the sensor axis in the world coordinate system:
        ::gz::math::Vector3 sensorAxis = linkPose.rot.RotateVector(this->sensorAxis_);

//        // FOR DEBUG:
//        std::cout << "link   pos = " << linkPose.pos.x << "," << linkPose.pos.y << "," << linkPose.pos.z << std::endl;
//        std::cout << "sensor pos = " << sensorPose_.pos.x << "," << sensorPose_.pos.y << "," << sensorPose_.pos.z << std::endl;
//        std::cout << "abs    pos = " << sensorAbsPos.x << "," << sensorAbsPos.y << "," << sensorAbsPos.z << std::endl;

        // for now sensor orientation does not matter, this is temporary
		output_ = 0.0;
        for (unsigned int i = 0; i < srcPositions.size(); ++i) {
			gz::math::Vector3 srcPos = srcPositions[i];
            double dist = sensorAbsPos.Distance(srcPos);
            ::gz::math::Vector3 sensorToSource = (srcPos - sensorAbsPos).Normalize();

            double dot = sensorAxis.Dot(sensorToSource);

//            // FOR DEBUG:
//            std::cout << "dir  = " << sensorToSource.x << "," << sensorToSource.y << "," << sensorToSource.z << std::endl;
//            std::cout << "axis = " << sensorAxis.x << "," << sensorAxis.y << "," << sensorAxis.z << std::endl;
//            std::cout << "dot = " << dot << "\n" << std::endl;

//            if (dot < 0) {
//               dot = 0;
//	      }


			double distSq = dist * dist;
//			double intensity = 1.0 / (distSq + 0.0001);
            double intensity = 1.0;
            output_ += intensity*dot;
        }

        // // FOR DEBUG:
        // std::cout << "microphone " << this->sensorId() << " = " << this->output_ << std::endl;
	}
}

void DirectionSensor::read(double * input) {
	input[0] = this->output_;
}

	
} // namespace gazebo
} // namespace revolve
