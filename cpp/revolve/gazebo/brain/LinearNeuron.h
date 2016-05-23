#ifndef REVOLVE_GAZEBO_BRAIN_LINEARNEURON_H_
#define REVOLVE_GAZEBO_BRAIN_LINEARNEURON_H_

#include "Neuron.h"
#include "NeuralConnection.h"

namespace revolve {
namespace gazebo {

class LinearNeuron : public Neuron
{
public:
	LinearNeuron(sdf::ElementPtr neuron);
	virtual double CalculateOutput(double t);


protected:
	double gain_;
	double bias_;
};

}
}

#endif // REVOLVE_GAZEBO_BRAIN_LINEARNEURON_H_