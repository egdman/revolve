#ifndef REVOLVE_GAZEBO_BRAIN_NEATLEARNER_H_
#define REVOLVE_GAZEBO_BRAIN_NEATLEARNER_H_

#include "Brain.h"
#include "ExtendedNeuralNetwork.h"
#include <map>
#include <vector>
#include <string>

#include <gazebo/gazebo.hh>
#include <revolve/msgs/neural_net.pb.h>

namespace revolve {
namespace gazebo {


enum State
{
	WARMUP,
	EVALUATION
};


class NeatLearner : public Brain
{
public:
	NeatLearner(std::string modelName, sdf::ElementPtr node,
				  std::vector< MotorPtr > & motors, std::vector< SensorPtr > & sensors);

	virtual ~NeatLearner();

	virtual void update(const std::vector< MotorPtr > & motors, const std::vector< SensorPtr > & sensors,
			double t, double step);


protected:

	void updatePose(const boost::shared_ptr<::gazebo::msgs::PosesStamped const> &msg);
	
	// Mutex for updating the network
	boost::mutex networkMutex_;

	// Pointer to the underlying controller
	NeuralNetworkPtr controller_;

	// Transport node
	::gazebo::transport::NodePtr node_;

	// Subscriber to pose updates
	::gazebo::transport::SubscriberPtr poseSub_;

	// Name of the robot
    std::string modelName_;

    std::vector<double> startPosition_;
    std::vector<double> position_;

    // Time stamp
    double currentTime_;
    double lastTime_;

    State currentState_;

};


}
}

#endif // REVOLVE_GAZEBO_BRAIN_EXTENDEDNEURALNETWORK_H_