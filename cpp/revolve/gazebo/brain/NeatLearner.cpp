#include "NeatLearner.h"

#include "../motors/Motor.h"
#include "../sensors/Sensor.h"
#include <fstream>
#include <iostream>

namespace gz = gazebo;

namespace revolve {
namespace gazebo {

#define WARMUP_TIME 3
#define EVALUATION_TIME 30

NeatLearner::NeatLearner(std::string modelName, sdf::ElementPtr node,
				  std::vector< MotorPtr > & motors, std::vector< SensorPtr > & sensors)
{
	
	// Create transport node
	node_.reset(new gz::transport::Node());
	node_->Init();

    // remember the name of the robot
    modelName_ = modelName;

    // initialize current robot position to 0:
    position_.resize(3);
    position_[0] = 0.0;
    position_[1] = 0.0;
    position_[2] = 0.0;

    // initialize starting position to 0:
    startPosition_.resize(3);
    startPosition_[0] = 0.0;
    startPosition_[1] = 0.0;
    startPosition_[2] = 0.0;

    // initialize time to 0:
    this->currentTime_ = 0.0;
    this->lastTime_ = 0.0;

    // set initial state:
    this->currentState_ = WARMUP;

    // subscribe to pose updates:
    poseSub_ = node_->Subscribe("~/revolve/robot_poses",
								 &NeatLearner::updatePose, this);

    // initialize the controller:
    controller_.reset(new ExtendedNeuralNetwork(modelName_, node, motors, sensors));
 
}



NeatLearner::~NeatLearner()
{}

void NeatLearner::updatePose(const boost::shared_ptr<::gazebo::msgs::PosesStamped const> &msg)
{
	for (int i = 0; i < msg->pose_size(); ++i) {
		auto poseMsg = msg->pose(i);
		auto name =	poseMsg.name();
		
		if (name == this->modelName_) {
			auto position = poseMsg.position();
			this->position_[0] = position.x();
			this->position_[1] = position.y();
			this->position_[2] = position.z();
		}
	}
	auto timeStamp = msg->time();
	double seconds = (double)timeStamp.sec();
	double nanoseconds = (double)timeStamp.nsec() / 1000000000.0;
	this->currentTime_ = seconds + nanoseconds;

}



void NeatLearner::update(const std::vector<MotorPtr>& motors,
		const std::vector<SensorPtr>& sensors,
		double t, double step) 
{
	// Update the neural network state
	this->controller_->update(motors, sensors, t, step);
	
	std::string stateName;
	if (currentState_ == WARMUP) {
		stateName = "WARMUP";

		// Discard displacement during WARMUP
		startPosition_[0] = position_[0];
		startPosition_[1] = position_[1];
		startPosition_[2] = position_[2];


		// If WARMUP is over, switch to EVALUATION
		if (currentTime_ - lastTime_ > WARMUP_TIME) {
			lastTime_ = currentTime_;
			currentState_ = EVALUATION;
		}
	}

	if (currentState_ == EVALUATION) {
		stateName = "EVALUATION";

		//// Do nothing during EVALUATION, wait for the robot to move //////

		// If evaluation is over
		if (currentTime_ - lastTime_ > EVALUATION_TIME) {


			//// CALCULATE FITNESS //////

			double dx = position_[0] - startPosition_[0];
			double dy = position_[1] - startPosition_[1];

			double distanceCovered = sqrt(dx*dx + dy*dy);
			double fitness = distanceCovered / (float)EVALUATION_TIME;

			std::cout << modelName_ << " FITNESS = " << fitness << std::endl;

			////////// TODO /////////////
			//// REMEMBER FITNESS ///////
			/////// SHARE FITNESS ///////
			//// IF ALL BRAINS EVALUATED, GENERATE NEW BRAINS (ALL THE NEAT THINGS HAPPEN HERE) ////
			//// INSERT NEW BRAIN ///////

			// Switch to WARMUP
			lastTime_ = currentTime_;
			currentState_ = WARMUP;
		}
	}
	// std::cout << "time = " << currentTime_ << ", x=" << position_[0] << ", y=" << position_[1] << 
	// ", state: " << stateName << std::endl;
}


}
}