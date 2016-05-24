#include "Neuron.h"
#include <iostream>

namespace revolve {
namespace gazebo {

Neuron::Neuron()
{
	this->output_ = 0;
	this->newOutput_ = 0;
}

void Neuron::AddIncomingConnection(const std::string &socketName, NeuralConnectionPtr connection)
{
	if (this->incomingConnections_.count(socketName)) {
		std::cerr << "Neuron socket '" << socketName << "' is already used" << std::endl;
		throw std::runtime_error("Robot brain error");
	}

	this->incomingConnections_[socketName] = connection;
}


void Neuron::Update(double t)
{
	this->newOutput_ = this->CalculateOutput(t);
}


void Neuron::FlipState()
{
	this->output_ = this->newOutput_;
}


double Neuron::GetOutput() const
{
	return this->output_;
}


std::string Neuron::GetUniqueSocketId() const
{
	int id = rand();

	while (this->incomingConnections_.count(std::to_string(id))) {
		id = rand();
	}
	return std::to_string(id);
}


}
}
