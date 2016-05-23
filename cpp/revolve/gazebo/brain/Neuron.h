#ifndef REVOLVE_GAZEBO_BRAIN_NEURON_H_
#define REVOLVE_GAZEBO_BRAIN_NEURON_H_

#include "Brain.h"

namespace revolve {
namespace gazebo {


class Neuron
{
public:
	Neuron();
	virtual ~Neuron() {};
	virtual double CalculateOutput(double t) = 0;

	void AddIncomingConnection(const NeuralConnectionPtr &connection);

	double GetOutput() const;

	virtual void SetInput(double value) {};

	void Update(double t);

	void FlipState();

protected:
	std::vector<NeuralConnectionPtr> incomingConnections_;
	double output_;
	double newOutput_;

};

}
}
#endif // REVOLVE_GAZEBO_BRAIN_NEURON_H_