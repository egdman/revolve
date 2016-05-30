#include "DifferentialCPG.h"
#include <sstream>

namespace revolve {
namespace gazebo {


/* *
 * Constructor from a map of parameter names and values
 */
DifferentialCPG::DifferentialCPG(const std::string &id, const std::map<std::string, double> &params):
Neuron(id)
{
	if (!params.count("rv:bias")) {
		std::cerr << "A `" << "Differential CPG" << "` neuron requires `rv:bias` element." << std::endl;
		throw std::runtime_error("Robot brain error");
	}
	this->bias_ = params.find("rv:bias")->second;
	lastTime_ = 0;
}


double DifferentialCPG::CalculateOutput(double t)
{
	double deltaT = t - lastTime_;
	lastTime_ = t;

	if (deltaT > 0.1) {
		deltaT = 0.1;
	}

	double inputValue = 0;

	for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it)
	{
		auto inConnection = it->second;
		inputValue += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
	}

	double state_deriv = inputValue - this->bias_;
	double result = this->output_ + deltaT * state_deriv;

	// limit output:
	if (result > 10000.0) {
		result = 10000.0;
	}
	else if (result < -10000.0) {
		result = -10000.0;
	}

	return result;
}


}
}