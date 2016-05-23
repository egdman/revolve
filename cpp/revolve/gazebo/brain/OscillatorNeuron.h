#ifndef REVOLVE_GAZEBO_BRAIN_OSCILLATORNEURON_H_
#define REVOLVE_GAZEBO_BRAIN_OSCILLATORNEURON_H_

#include "Neuron.h"
#include "NeuralConnection.h"

namespace revolve {
namespace gazebo {

class OscillatorNeuron : public Neuron
{
public:
	OscillatorNeuron(sdf::ElementPtr neuron);
	virtual double CalculateOutput(double t);

protected:
	double period_;
	double phaseOffset_;
	double gain_;
};

}
}

#endif // REVOLVE_GAZEBO_BRAIN_OSCILLATORNEURON_H_