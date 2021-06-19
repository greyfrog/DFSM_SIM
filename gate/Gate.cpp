/*! \file Gate.cpp
 *  \brief Implementierung der Klasse Gate
 *  \author cpr
 *  \date 2020
 */

#include "Gate.h"

using namespace sgnl;
using namespace std;


Gate::Gate() :
	pLogic_(nullptr),
	propagationDelay_(0),
	name_("###undefined###")
{

}

Gate::Gate(
	std::string a_name,
	const GateLogic* a_pLogic,
	sgnl::Nanoseconds a_propagationDelay) :
		 pLogic_( a_pLogic),
		propagationDelay_(a_propagationDelay),
		name_(a_name)

{

	prevInputs_logical_.resize(pLogic_->nInputs());
	prevInputs_.resize(pLogic_->nInputs());
	inputs_.resize(pLogic_->nInputs());
	prevOutputs_logical_.resize(pLogic_->nOutputs());
	prevOutputs_.resize(pLogic_->nOutputs());
	outputs_.resize(pLogic_->nOutputs());
	for(int i=0; i<static_cast<int>(prevOutputs_logical_.size()); i++)
	{
		prevOutputs_logical_[i] = false;
		prevOutputs_[i] = LOW_LEVEL_VOLT;
		outputs_[i] = LOW_LEVEL_VOLT;
	}
	for(int i=0; i<static_cast<int>(prevInputs_.size()); i++)
	{
		prevInputs_logical_[i] = false;
		prevInputs_[i] = LOW_LEVEL_VOLT;
		inputs_[i] = LOW_LEVEL_VOLT;
	}
}

void Gate::setInput(int a_idx, sgnl::Volt a_voltage)
{
	inputs_[a_idx] = a_voltage;
	return;
}

void Gate::simulationStep()
{
	vector<bool> inputs_logical
		= voltageToLogical(inputs_, prevInputs_logical_);

	vector<bool> outputs_logical
		= pLogic_->outputsFor(
			inputs_logical,
			prevInputs_logical_,
			prevOutputs_logical_);

	outputs_ = logicalToVolt(outputs_logical);

	outputs_ = applyPropagationDelay(outputs_);
	outputs_ = applyNoise(outputs_);

	prevOutputs_logical_ = outputs_logical;
	prevInputs_logical_ = inputs_logical;
	prevOutputs_ = outputs_;
	prevInputs_ = inputs_;

	return;
}

std::vector<sgnl::Volt> Gate::outputs() const
{
	return outputs_;
}

int Gate::nInputs() const
{
	return static_cast<int>(prevInputs_.size());
}

int Gate::nOutputs() const
{
	return static_cast<int>(outputs_.size());
}

std::string Gate::logicName() const
{
	return pLogic_->name();
}

std::vector<sgnl::Volt> Gate::applyPropagationDelay(
		std::vector<sgnl::Volt> a_outputs)
{
	if(propagationDelay_ == 0)
	{
		return a_outputs;
	}
	vector<Volt> delayed;
	delayed.resize(a_outputs.size());
	double k = (1.0/propagationDelay_);
	for(int i=0; i<static_cast<int>(a_outputs.size()); i++)
	{
		delayed[i] = k * (a_outputs[i] + (propagationDelay_-1)*prevOutputs_[i]);
	}
	return delayed;
}
