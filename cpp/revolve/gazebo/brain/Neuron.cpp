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
	this->incomingConnections_.push_back(std::pair<std::string, NeuralConnectionPtr>(socketName, connection));
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


std::string Neuron::GetSocketId() const
{
	return std::to_string( this->incomingConnections_.size() );
}


}
}
