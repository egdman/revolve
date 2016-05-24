#include "VOscillator.h"

namespace revolve {
namespace gazebo {

VOscillator::VOscillator(sdf::ElementPtr neuron)
{
	auto type = neuron->GetAttribute("type")->GetAsString();
	if (!neuron->HasElement("rv:alpha") || !neuron->HasElement("rv:tau") || !neuron->HasElement("rv:energy")) {
			std::cerr << "A `" << type << "` neuron requires `rv:alpha`, `rv:tau` and `rv:energy` elements." << std::endl;
			throw std::runtime_error("Robot brain error");
	}
	this->alpha_ = neuron->GetElement("rv:alpha")->Get< double >();
	this->tau_ = neuron->GetElement("rv:tau")->Get< double >();
	this->energy_ = neuron->GetElement("rv:energy")->Get< double >();

	this->lastTime_ = 0;
	this->stateDeriv_ = 0;
}


double VOscillator::CalculateOutput(double t)
{
	double deltaT = t - lastTime_;
	lastTime_ = t;

	double xInput = 0;  // input from X-neuron of the same oscillator
	double vInput = this->output_; // input from V-neuron of the same oscillator (this neuron)

	double vExternal = 0; // input from V-neuron of another oscillator
	double xExternal = 0; // input from X-neuron of another oscillator

	double otherInputs = 0; // all other inputs

	for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it)
	{
		auto socketId = it->first;
		auto inConnection = it->second;

		

		if (socketId == "from_x") {
			xInput += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
		}
		else if (socketId == "from_x_ext") {
			xExternal += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
		} 
		else if (socketId == "from_v_ext") {
			vExternal += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
		}

		else {
			otherInputs += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
		}

		

	}

	stateDeriv_ = (- (alpha_ / energy_) * vInput * ( xInput*xInput + vInput*vInput )
		+ alpha_ * vInput - xInput + xExternal + vExternal + otherInputs) / tau_;

	return vInput + deltaT * stateDeriv_;
}


}
}