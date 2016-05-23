#include "OscillatorNeuron.h"
#include <cmath>

namespace revolve {
namespace gazebo {

OscillatorNeuron::OscillatorNeuron(sdf::ElementPtr neuron)
{
	if (!neuron->HasElement("rv:period") || !neuron->HasElement("rv:phase_offset") ||
				!neuron->HasElement("rv:amplitude")) {
			std::cerr << "An `Oscillator` neuron requires `rv:period`, `rv:phase_offset` and `rv:amplitude` elements." << std::endl;
			throw std::runtime_error("Robot brain error");
	}

		// Set period, phase offset and gain
		this->period_ = neuron->GetElement("rv:period")->Get< double >();
		this->phaseOffset_ = neuron->GetElement("rv:phase_offset")->Get< double >();
		this->gain_ = neuron->GetElement("rv:amplitude")->Get< double >();
}

double OscillatorNeuron::CalculateOutput(double t)
{
	return 0.5 * (1.0 + this->gain_ *
		sin( 2.0*M_PI/(this->period_) * (t - this->period_ * this->phaseOffset_) ));
}


}
}