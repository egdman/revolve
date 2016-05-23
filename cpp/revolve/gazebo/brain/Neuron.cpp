#include "Neuron.h"

namespace revolve {
namespace gazebo {

Neuron::Neuron()
{
	this->output_ = 0;
	this->newOutput_ = 0;
}

void Neuron::AddIncomingConnection(const NeuralConnectionPtr &connection)
{
	this->incomingConnections_.push_back(connection);
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


}
}
