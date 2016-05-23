#include "InputNeuron.h"

namespace revolve {
namespace gazebo {

InputNeuron::InputNeuron(sdf::ElementPtr neuron)
{
	input_ = 0;
}

double InputNeuron::CalculateOutput(double /*t*/)
{
	return input_;
}

void InputNeuron::SetInput(double value)
{
	input_ = value;
}

}
}