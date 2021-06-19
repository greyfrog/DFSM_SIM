/*! \file GateLogic.cpp
 *  \brief Implementierung der Klasse GateLogic
 *  \author cpr
 *  \date 2020
 */

#include "GateLogic.h"

#include <iostream>

using namespace std;


GateLogic::GateLogic(
	int a_nInputs,
	int a_nOutputs,
	std::string a_name) :
		nInputs_(a_nInputs),
		nOutputs_(a_nOutputs),
		name_(a_name)
{

}

GateLogic::~GateLogic()
{

}

 std::string GateLogic::name() const
{
	return name_;
}

int GateLogic::nInputs() const
{
	return nInputs_;
}

int GateLogic::nOutputs() const
{
	return nOutputs_;
}

bool GateLogic::assertDimensionsMatch(
	const std::vector<bool> &a_inputs,
	const std::vector<bool> &a_prevInputs,
	const std::vector<bool> &a_prevOutputs) const
{
	if(nInputs_ != static_cast<int>(a_inputs.size()))
	{
		cerr << "GateLogic: Number of inputs must be " << nInputs_
			 << " (have " << a_inputs.size() << ")." << endl;
		return false;
	}
	if(nInputs_ != static_cast<int>(a_prevInputs.size()))
	{
		cerr << "GateLogic: Number of previous inputs must be " << nInputs_
			 << " (have " << a_prevInputs.size() << ")." << endl;
		return false;
	}
	if(nOutputs_ != static_cast<int>(a_prevOutputs.size()))
	{
		cerr << "GateLogic: Number of previous outputs must be " << nOutputs_
			 << " (have " << a_prevOutputs.size() << ")." << endl;
		return false;
	}
	return true;
}
