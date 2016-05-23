#ifndef REVOLVE_GAZEBO_BRAIN_SIGMOIDNEURON_H_
#define REVOLVE_GAZEBO_BRAIN_SIGMOIDNEURON_H_

#include "Neuron.h"
#include "NeuralConnection.h"

namespace revolve {
namespace gazebo {

class SigmoidNeuron : public Neuron
{
public:
	SigmoidNeuron(sdf::ElementPtr neuron);
	virtual double CalculateOutput(double t);


protected:
	double gain_;
	double bias_;
};

}
}

#endif // REVOLVE_GAZEBO_BRAIN_SIGMOIDNEURON_H_