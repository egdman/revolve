#include "XOscillator.h"

namespace revolve {
namespace gazebo {

XOscillator::XOscillator(sdf::ElementPtr neuron)
{
	auto type = neuron->GetAttribute("type")->GetAsString();
	if (!neuron->HasElement("rv:tau")) {
			std::cerr << "A `" << type << "` neuron requires `rv:tau` element." << std::endl;
			throw std::runtime_error("Robot brain error");
	}
	this->tau_ = neuron->GetElement("rv:tau")->Get< double >();

	this->lastTime_ = 0;
	this->stateDeriv_ = 0;
}


double XOscillator::CalculateOutput(double t)
{
	double deltaT = t - lastTime_;
	lastTime_ = t;

	double vInput = 0;  // input from V-neuron of the same oscillator
	double xInput = this->output_; // input from X-neuron of the same oscillator (this neuron)

	for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it)
	{
		auto socketId = it->first;
		auto inConnection = it->second;

		if (socketId == "from_v") {
			vInput += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
		}
	}

	stateDeriv_ = vInput / tau_;

	return xInput + deltaT * stateDeriv_;
}


}
}