#include "LinearNeuron.h"

namespace revolve {
namespace gazebo {

LinearNeuron::LinearNeuron(sdf::ElementPtr neuron)
{
	auto type = neuron->GetAttribute("type")->GetAsString();
	if (!neuron->HasElement("rv:bias") || !neuron->HasElement("rv:gain")) {
			std::cerr << "A `" << type << "` neuron requires `rv:bias` and `rv:gain` elements." << std::endl;
			throw std::runtime_error("Robot brain error");
	}
	this->bias_ = neuron->GetElement("rv:bias")->Get< double >();
	this->gain_ = neuron->GetElement("rv:gain")->Get< double >();
}


double LinearNeuron::CalculateOutput(double /*t*/)
{
	double inputValue = 0;

	for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it)
	{
		auto inConnection = *it;
		inputValue += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
	}

	return this->gain_ * (inputValue - this->bias_);
}


}
}