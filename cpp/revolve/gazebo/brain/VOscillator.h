#ifndef REVOLVE_GAZEBO_BRAIN_VOSCILLATOR_H_
#define REVOLVE_GAZEBO_BRAIN_VOSCILLATOR_H_

#include "Neuron.h"
#include "NeuralConnection.h"

namespace revolve {
namespace gazebo {

class VOscillator : public Neuron
{
public:
	VOscillator(sdf::ElementPtr neuron);
	virtual double CalculateOutput(double t);


protected:
	double alpha_;
	double tau_;
	double energy_;

	double lastTime_;
	double stateDeriv_;
};

}
}

#endif // REVOLVE_GAZEBO_BRAIN_VOSCILLATOR_H_