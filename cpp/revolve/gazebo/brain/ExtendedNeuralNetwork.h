#ifndef REVOLVE_GAZEBO_BRAIN_EXTENDEDNEURALNETWORK_H_
#define REVOLVE_GAZEBO_BRAIN_EXTENDEDNEURALNETWORK_H_

#include "Brain.h"
#include "Neuron.h"
#include "NeuralConnection.h"
#include <map>
#include <vector>
#include <string>

#include "LinearNeuron.h"
#include "SigmoidNeuron.h"
#include "OscillatorNeuron.h"
#include "InputNeuron.h"

#include <gazebo/gazebo.hh>
#include <revolve/msgs/neural_net.pb.h>

namespace revolve {
namespace gazebo {

typedef const boost::shared_ptr<revolve::msgs::ModifyNeuralNetwork const> ConstModifyNeuralNetworkPtr;

class ExtendedNeuralNetwork : public Brain
{
public:
	ExtendedNeuralNetwork(std::string modelName, sdf::ElementPtr node,
				  std::vector< MotorPtr > & motors, std::vector< SensorPtr > & sensors);

	virtual ~ExtendedNeuralNetwork();

	virtual void update(const std::vector< MotorPtr > & motors, const std::vector< SensorPtr > & sensors,
			double t, double step);


protected:

		// Mutex for updating the network
	boost::mutex networkMutex_;

	/**
	 * Transport node
	 */
	::gazebo::transport::NodePtr node_;

	/**
	 * Network modification subscriber
	 */
	::gazebo::transport::SubscriberPtr alterSub_;


	/**
	 * Publisher for network modification responses
	 */
	::gazebo::transport::PublisherPtr responsePub_;


	NeuronPtr neuronHelper(sdf::ElementPtr neuron);

	void connectionHelper(const std::string &src, const std::string &dst, double weight,
		const std::map<std::string, NeuronPtr> &idToNeuron);


	void modify(ConstModifyNeuralNetworkPtr &req);

    /**
     * Name of the robot
     */
    std::string modelName_;

    // buffer of input values from the sensors
    double * inputs_;
    double * outputs_;

    std::vector<NeuronPtr> allNeurons_;
	std::vector<NeuronPtr> inputNeurons_;
	std::vector<NeuronPtr> outputNeurons_;

	std::map<NeuronPtr, int> outputPositionMap_;
	std::map<NeuronPtr, int> inputPositionMap_;

    std::vector<NeuralConnectionPtr> connections_;

    int numInputNeurons_;
    int numOutputNeurons_;

};


}
}

#endif // REVOLVE_GAZEBO_BRAIN_EXTENDEDNEURALNETWORK_H_