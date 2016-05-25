#include "SigmoidNeuron.h"
#include <cmath>

namespace revolve {
namespace gazebo {


/* *
 * Constructor from an sdf element
 */
// SigmoidNeuron::SigmoidNeuron(sdf::ElementPtr neuron)
// {
// 	auto type = neuron->GetAttribute("type")->GetAsString();
// 	if (!neuron->HasElement("rv:bias") || !neuron->HasElement("rv:gain")) {
// 		std::cerr << "A `" << type << "` neuron requires `rv:bias` and `rv:gain` elements." << std::endl;
// 		throw std::runtime_error("Robot brain error");
// 	}
// 	this->bias_ = neuron->GetElement("rv:bias")->Get< double >();
// 	this->gain_ = neuron->GetElement("rv:gain")->Get< double >();
// }


/* *
 * Constructor from a map of parameter names and values
 */
SigmoidNeuron::SigmoidNeuron(const std::string &id, const std::map<std::string, double> &params):
Neuron(id)
{
	if (!params.count("rv:bias") || !params.count("rv:gain")) {
		std::cerr << "A `" << "Sigmoid" << "` neuron requires `rv:bias` and `rv:gain` elements." << std::endl;
		throw std::runtime_error("Robot brain error");
	}
	this->bias_ = params.find("rv:bias")->second;
	this->gain_ = params.find("rv:gain")->second;
}


double SigmoidNeuron::CalculateOutput(double /*t*/)
{
	double inputValue = 0;

	for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it)
	{
		auto inConnection = it->second;
		inputValue += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
	}

	return 1.0 / ( 1.0 + exp( - this->gain_ * (inputValue - this->bias_) ) );
}


}
}