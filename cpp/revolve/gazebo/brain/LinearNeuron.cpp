#include "LinearNeuron.h"
#include <sstream>

namespace revolve {
namespace gazebo {


/* *
 * Constructor from a map of parameter names and values
 */
LinearNeuron::LinearNeuron(const std::string &id, const std::map<std::string, double> &params):
Neuron(id)
{
	if (!params.count("rv:bias") || !params.count("rv:gain")) {
		std::cerr << "A `" << "Simple" << "` neuron requires `rv:bias` and `rv:gain` elements." << std::endl;
		throw std::runtime_error("Robot brain error");
	}
	this->bias_ = params.find("rv:bias")->second;
	this->gain_ = params.find("rv:gain")->second;
}


double LinearNeuron::CalculateOutput(double t)
{
	double inputValue = 0;

	for (auto it = this->incomingConnections_.begin(); it != this->incomingConnections_.end(); ++it)
	{
		auto inConnection = it->second;
		inputValue += inConnection->GetInputNeuron()->GetOutput() * inConnection->GetWeight();
	}

	double result = this->gain_ * (inputValue - this->bias_);

	// limit output:
	if (result > 10000.0) {
		result = 10000.0;
	}
	else if (result < -10000.0) {
		result = -10000.0;
	}

	// std::stringstream filename;
	// filename << "/home/dmitry/projects/debug/out_neurons/" << Id() << ".log";
	// std::ofstream logFile;
	// logFile.open(filename.str(), std::ofstream::out | std::ofstream::app);
	// logFile << t << "," << result << std::endl;
	// logFile.close();

	return result;
}


}
}