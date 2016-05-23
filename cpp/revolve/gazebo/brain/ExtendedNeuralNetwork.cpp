#include "ExtendedNeuralNetwork.h"

#include "../motors/Motor.h"
#include "../sensors/Sensor.h"
#include <fstream>
#include <iostream>

namespace gz = gazebo;

namespace revolve {
namespace gazebo {

ExtendedNeuralNetwork::ExtendedNeuralNetwork(std::string modelName, sdf::ElementPtr node,
				  std::vector< MotorPtr > & motors, std::vector< SensorPtr > & sensors)
{
	
	// Create transport node
	node_.reset(new gz::transport::Node());
	node_->Init();

    // remember the name of the robot
    modelName_ = modelName;

 //  	// Listen to network modification requests
	// alterSub_ = node_->Subscribe("~/"+modelName+"/modify_neural_network",
	// 							 &ExtendedNeuralNetwork::modify, this);

 //    responsePub_ = node_->Advertise<gz::msgs::Response>("~/"+modelName+"/modify_neural_network_response");


	// Map neuron sdf elements to their id's
    std::map<std::string, sdf::ElementPtr> neuronDescriptions;

    std::map<std::string, NeuronPtr> idToNeuron;


	// List of all hidden neuron id's
	std::vector<std::string> hiddenIds;

	// Number of input neurons for mapping them to the input buffer
	numInputNeurons_ = 0;

	// Number of output neurons for mapping them to the output buffer
	numOutputNeurons_ = 0;

	// Get the first sdf neuron element
    auto neuron = node->HasElement("rv:neuron") ? node->GetElement("rv:neuron") : sdf::ElementPtr();

    while (neuron) {
		if (!neuron->HasAttribute("layer") || !neuron->HasAttribute("id")) {
			std::cerr << "Missing required neuron attributes (id or layer). '" << std::endl;
			throw std::runtime_error("Robot brain error");
		}
		auto layer = neuron->GetAttribute("layer")->GetAsString();
		auto neuronId = neuron->GetAttribute("id")->GetAsString();


		// check if a neuron with this id has been already added
		if (neuronDescriptions.count(neuronId)) {
			std::cerr << "Duplicate neuron ID '"
					<< neuronId << "'" << std::endl;
			throw std::runtime_error("Robot brain error");
		}

		// add this neuron to the id->sdf map
		neuronDescriptions[neuronId] = neuron;

		// add the neuron id to the appropriate list of id's
		if ("input" == layer) {
//			inputIds.push_back(neuronId);
		}

		else if ("output" == layer) {
//			outputIds.push_back(neuronId);
		}

		else if ("hidden" == layer) {
			hiddenIds.push_back(neuronId);
		}

		else {
			std::cerr << "Unknown neuron layer '" << layer << "'." << std::endl;
			throw std::runtime_error("Robot brain error");
		}

		neuron = neuron->GetNextElement("rv:neuron");
	}


	// Add output neurons for motors:

	// map of numbers of output neurons for each body part
	std::map<std::string, unsigned int> outputCountMap;

	for (auto it = motors.begin(); it != motors.end(); ++it) {
		auto motor = *it;
		auto partId = motor->partId();

		if (!outputCountMap.count(partId)) {
			outputCountMap[partId] = 0;
		}

		for (unsigned int i = 0, l = motor->outputs(); i < l; ++i) {
			std::stringstream neuronId;
			neuronId << partId << "-out-" << outputCountMap[partId];
			outputCountMap[partId]++;

			auto neuronDescription = neuronDescriptions.find(neuronId.str());
			if (neuronDescription == neuronDescriptions.end()) {
				std::cerr << "Required output neuron " << neuronId.str() <<
						" for motor could not be located"
						<< std::endl;
				throw std::runtime_error("Robot brain error");
			}
			
			auto newNeuron = this->neuronHelper(neuronDescription->second);
			idToNeuron[neuronId.str()] = newNeuron;
			
		}
	}


	// Add input neurons for sensors:

	// map of number of input neurons for each part:

	std::map<std::string, unsigned int> inputCountMap;

	for (auto it = sensors.begin(); it != sensors.end(); ++it) {
		auto sensor = *it;
		auto partId = sensor->partId();

		if (!inputCountMap.count(partId)) {
			inputCountMap[partId] = 0;
		}

		for (unsigned int i = 0, l = sensor->inputs(); i < l; ++i) {
			std::stringstream neuronId;
			neuronId << partId << "-in-" << inputCountMap[partId];
			inputCountMap[partId]++;

			auto neuronDescription = neuronDescriptions.find(neuronId.str());
			if (neuronDescription == neuronDescriptions.end()) {
				std::cerr << "Required input neuron " << neuronId.str() <<
						" for sensor could not be located"
						<< std::endl;
				throw std::runtime_error("Robot brain error");
			}

			auto newNeuron = this->neuronHelper(neuronDescription->second);
			idToNeuron[neuronId.str()] = newNeuron;
		}
	}

	// initialize the array for sensor inputs:
	inputs_ = new double[numInputNeurons_];
	outputs_ = new double[numOutputNeurons_];


	// Add hidden neurons:
	for (auto it = hiddenIds.begin(); it != hiddenIds.end(); ++it) {
		auto neuronDescription = neuronDescriptions.find(*it);
		auto newNeuron = this->neuronHelper(neuronDescription->second);
		idToNeuron[*it] = newNeuron;
	}


	// Add connections:
	auto connection = node->HasElement("rv:neural_connection") ? node->GetElement("rv:neural_connection") : sdf::ElementPtr();
	while (connection) {
		if (!connection->HasAttribute("src") || !connection->HasAttribute("dst")
				|| !connection->HasAttribute("weight")) {
			std::cerr << "Missing required connection attributes (`src`, `dst` or `weight`)." << std::endl;
			throw std::runtime_error("Robot brain error");
		}

		auto src = connection->GetAttribute("src")->GetAsString();
		auto dst = connection->GetAttribute("dst")->GetAsString();
		double weight;
		connection->GetAttribute("weight")->Get(weight);

		// Use connection helper to set the weight
		connectionHelper(src, dst, weight, idToNeuron);

		// Load the next connection
		connection = connection->GetNextElement("rv:neural_connection");
	}
}


ExtendedNeuralNetwork::~ExtendedNeuralNetwork()
{
	delete inputs_;
	delete outputs_;
}


void ExtendedNeuralNetwork::connectionHelper(const std::string &src, const std::string &dst, double weight,
	const std::map<std::string, NeuronPtr> &idToNeuron)
{
	auto srcNeuron = idToNeuron.find(src);
	if (srcNeuron == idToNeuron.end()) {
		std::cerr << "Could not find source neuron '" << src << "'" << std::endl;
		throw std::runtime_error("Robot brain error");
	}

	auto dstNeuron = idToNeuron.find(dst);
	if (dstNeuron == idToNeuron.end()) {
		std::cerr << "Could not find destination neuron '" << dst << "'" << std::endl;
		throw std::runtime_error("Robot brain error");
	}

	NeuralConnectionPtr newConnection(new NeuralConnection(
		srcNeuron->second,
		dstNeuron->second,
		weight
	));

	(dstNeuron->second)->AddIncomingConnection(newConnection);
	connections_.push_back(newConnection);
}


NeuronPtr ExtendedNeuralNetwork::neuronHelper(sdf::ElementPtr neuron)
{

	if (!neuron->HasAttribute("type")) {
		std::cerr << "Missing required `type` attribute for neuron." << std::endl;
		throw std::runtime_error("Robot brain error");
	}

	auto type = neuron->GetAttribute("type")->GetAsString();
	auto layer = neuron->GetAttribute("layer")->GetAsString();

	NeuronPtr newNeuron;

	if ("input" == layer) {
		newNeuron.reset(new InputNeuron(neuron));

		this->inputNeurons_.push_back(newNeuron);
		inputPositionMap_[newNeuron] = numInputNeurons_;
		numInputNeurons_++;
	}

	else {

		if ("Sigmoid" == type) {
			newNeuron.reset(new SigmoidNeuron(neuron));
		}
		else if ("Simple" == type) {
			newNeuron.reset(new LinearNeuron(neuron));
		}
		else if ("Oscillator" == type) {
			newNeuron.reset(new OscillatorNeuron(neuron));
		}
		else {
			std::cerr << "Unsupported neuron type `" << type << '`' << std::endl;
			throw std::runtime_error("Robot brain error");
		}

		if ("output" == layer) {
			this->outputNeurons_.push_back(newNeuron);
			outputPositionMap_[newNeuron] = numOutputNeurons_;
			numOutputNeurons_++;
		}

	}

	this->allNeurons_.push_back(newNeuron);
	return newNeuron;
}



void ExtendedNeuralNetwork::update(const std::vector<MotorPtr>& motors,
		const std::vector<SensorPtr>& sensors,
		double t, double step) 
{
	boost::mutex::scoped_lock lock(networkMutex_);

	// Read sensor data into the input buffer
	unsigned int p = 0;
	for (auto sensor : sensors) {
		sensor->read(&inputs_[p]);
		p += sensor->inputs();
	}

	// Feed inputs into the input neurons
	for (auto it = inputNeurons_.begin(); it != inputNeurons_.end(); ++it) {
		auto inNeuron = *it;
		int pos = inputPositionMap_[inNeuron];
		inNeuron->SetInput(inputs_[pos]);
	}

	// Calculate new states of all neurons
	for (auto it = allNeurons_.begin(); it != allNeurons_.end(); ++it) {
		(*it)->Update(t);
	}


	// Flip states of all neurons
	for (auto it = allNeurons_.begin(); it != allNeurons_.end(); ++it) {
		(*it)->FlipState();
	}

	// Write outputs to the output buffer:
	for (auto it = outputNeurons_.begin(); it != outputNeurons_.end(); ++it) {
		auto outNeuron = *it;
		int pos = outputPositionMap_[outNeuron];
		outputs_[pos] = outNeuron->GetOutput();
	}
	
	// Send new signals to the motors
	p = 0;
	for (auto motor: motors) {
		motor->update(&outputs_[p], step);
		p += motor->outputs();
	}
}


void ExtendedNeuralNetwork::modify(ConstModifyNeuralNetworkPtr &req)
{}


}
}