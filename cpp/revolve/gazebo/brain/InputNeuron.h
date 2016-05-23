#ifndef REVOLVE_GAZEBO_BRAIN_INPUTNEURON_H_
#define REVOLVE_GAZEBO_BRAIN_INPUTNEURON_H_

#include "Neuron.h"

namespace revolve {
namespace gazebo {

class InputNeuron : public Neuron
{
public:
	InputNeuron(sdf::ElementPtr neuron);

	virtual void SetInput(double value);
	virtual double CalculateOutput(double t);

protected:
	double input_;

};

}
}

#endif // REVOLVE_GAZEBO_BRAIN_INPUTNEURON_H_