/*! \file Network.cpp
 *  \brief Implementierung der Klasse Network
 *  \author cpr
 *  \date 2020
 */

#include "Network.h"

#include <iostream>
#include <utility>

using namespace sgnl;
using namespace std;

Network::Network(std::string a_name, sgnl::Nanoseconds a_propagationDelay) :
		name_(a_name), propagationDelay_(a_propagationDelay)
{
	if (a_propagationDelay < 0)
	{
		cerr << "Network: Propagation delay must be >= 0." << endl;
		propagationDelay_ = 0;
	}
}

std::string Network::name() const
{
	return name_;
}

std::vector<std::string> Network::outputNames() const
{
	vector<string> ret;
	for (map<string, Pin>::const_iterator it = outputConnections_.begin();
			it != outputConnections_.end(); it++)
	{
		ret.push_back(it->first);
	}
	return ret;
}

std::vector<std::string> Network::inputNames() const
{
	vector<string> ret;
	for (map<string, vector<Pin> >::const_iterator it = inputConnections_.begin();
			it != inputConnections_.end(); it++)
	{
		ret.push_back(it->first);
	}
	return ret;
}

void Network::simulationStep(const std::map<std::string, sgnl::Volt> &a_inputs)
{

	for (map<string, vector<Pin> >::const_iterator it = inputConnections_.begin();
			it != inputConnections_.end(); it++)
	{
		if (a_inputs.count(it->first) == 0)
		{
			cerr << "Network: Input " << it->first << " not supplied." << endl;
			return;
		}
	}

	for (map<string, vector<Pin> >::const_iterator it = inputConnections_.begin();
			it != inputConnections_.end(); it++)
	{
		string inName = it->first;
		Volt inVoltage = a_inputs.at(inName);
		for (int i = 0; i < static_cast<int>(it->second.size()); i++)
		{
			string inGateName = it->second[i].gateName();
			int inGateIdx = it->second[i].idx();
			gates_[inGateName].setInput(inGateIdx, inVoltage);
		}
	}

	for (map<Pin, vector<Pin> >::iterator it = gate2GateConnections_.begin();
			it != gate2GateConnections_.end(); it++)
	{
		string outGateName = it->first.gateName();
		int outGateIdx = it->first.idx();
		vector<Volt> outVoltages = gates_[outGateName].outputs();
		Volt outVoltage = outVoltages[outGateIdx];

		for (int i = 0; i < static_cast<int>(it->second.size()); i++)
		{
			string inGateName = it->second[i].gateName();
			int inGateIdx = it->second[i].idx();
			gates_[inGateName].setInput(inGateIdx, outVoltage);
		}
	}

	for (map<string, Gate>::iterator it = gates_.begin(); it != gates_.end();
			it++)
	{
		it->second.simulationStep();
	}
}

std::map<std::string, sgnl::Volt> Network::outputs() const
{
	map<string, Volt> ret;
	for (map<string, Pin>::const_iterator it = outputConnections_.begin();
			it != outputConnections_.end(); it++)
	{
		string outGateName = it->second.gateName();
		int outGateIdx = it->second.idx();
		 vector<Volt> outVoltages = gates_.at(outGateName).outputs();
		Volt outVoltage = outVoltages[outGateIdx];
		ret[it->first] = outVoltage;
	}
	return ret;
}

void Network::prettyPrint() const
{
	cout << "Network definition" << endl;
	cout << "==================" << endl;
	cout << endl;

	cout << "Name: " << name_ << endl;
	cout << "Propagation delay: " << propagationDelay_ << " nsec" << endl
			<< endl;

	cout << "Inputs" << endl;
	for (map<string, vector<Pin> >::const_iterator it = inputConnections_.begin();
			it != inputConnections_.end(); it++)
	{
		string inName = it->first;
		cout << "    " << inName << endl;
		for (int i = 0; i < static_cast<int>(it->second.size()); i++)
		{
			string gateName = it->second[i].gateName();
			string typeName = gates_.at(gateName).logicName();
			int idx = it->second[i].idx();
			cout << "     => " << gateName << " (" << typeName << ") I" << idx
					<< endl;
		}
	}
	cout << endl;

	cout << "Outputs" << endl;
	for (map<string, Pin>::const_iterator it = outputConnections_.begin();
			it != outputConnections_.end(); it++)
	{
		string outName = it->first;
		string gateName = it->second.gateName();
		string typeName = gates_.at(gateName).logicName();
		int idx = it->second.idx();
		cout << "    " << gateName << " (" << typeName << ") O" << idx << " => "
				<< outName << endl;
	}
	cout << endl;

	cout << "Gate-to-gate connections" << endl;
	for (map<Pin, vector<Pin> >::const_iterator it = gate2GateConnections_.begin();
			it != gate2GateConnections_.end(); it++)
	{
		string outGateName = it->first.gateName();
		string outTypeName = gates_.at(outGateName).logicName();
		int outIdx = it->first.idx();
		for (int i = 0; i < static_cast<int>(it->second.size()); i++)
		{
			string inGateName = it->second[i].gateName();
			string inTypeName = gates_.at(inGateName).logicName();
			int inIdx = it->second[i].idx();
			cout << "    " << outGateName << " (" << outTypeName << ") O"
					<< outIdx << " => " << inGateName << " (" << inTypeName
					<< ") I" << inIdx << endl;
		}
	}
	cout << endl;
	return;
}

bool Network::consistent() const
{
	bool ret = true;
	if (inputConnections_.empty())
	{
		cerr << "Network: Network needs at least one input." << endl;
		ret = false;
	}
	if (outputConnections_.empty())
	{
		cerr << "Network: Network needs at least one output." << endl;
		ret = false;
	}
	if (gates_.empty())
	{
		cerr << "Network: Network needs at least one gate." << endl;
		ret = false;
	}

	for (map<string, vector<Pin> >::const_iterator it = inputConnections_.begin();
			it != inputConnections_.end(); it++)
	{
		if (it->second.size() < 1)
		{
			cerr << "Network: Input " << it->first << " is not connected."
					<< endl;
			ret = false;
		}
	}

	for (map<string, Pin>::const_iterator it = outputConnections_.begin();
			it != outputConnections_.end(); it++)
	{
		if (it->second.gateName() == "")
		{
			cerr << "Network: Output " << it->first << " is not connected."
					<< endl;
			ret = false;
		}
	}

	for (map<string, Gate>::const_iterator g_it = gates_.begin();
			g_it != gates_.end(); g_it++)
	{
		string gateName = g_it->first;
		int nGateInputs = g_it->second.nInputs();
		for (int i = 0; i < nGateInputs; i++)
		{
			Pin pinToCheck(gateName, i);
			int nOccurrences = 0;
			for (map<Pin, vector<Pin> >::const_iterator c_it =
					gate2GateConnections_.begin();
					c_it != gate2GateConnections_.end(); c_it++)
			{
				for (int k = 0; k < static_cast<int>(c_it->second.size()); k++)
				{
					if (c_it->second[k] == pinToCheck)
					{
						nOccurrences++;
					}
				}
			}
			for (map<string, vector<Pin> >::const_iterator i_it =
					inputConnections_.begin(); i_it != inputConnections_.end();
					i_it++)
			{
				for (int k = 0; k < static_cast<int>(i_it->second.size()); k++)
				{
					if (i_it->second[k] == pinToCheck)
					{
						nOccurrences++;
					}
				}
			}
			if (nOccurrences != 1)
			{
				cerr << "Network: Gate " << gateName << ", input " << i
						<< " must be connected exactly once (have "
						<< nOccurrences << ")." << endl;
				ret = false;
			}
		}
	}

	for (map<string, Gate>::const_iterator g_it = gates_.begin();
			g_it != gates_.end(); g_it++)
	{
		string gateName = g_it->first;
		int nGateOutputs = g_it->second.nOutputs();
		bool breakOuterLoop = false;
		for (int i = 0; i < nGateOutputs; i++)
		{
			Pin pinToCheck(gateName, i);
			if (gate2GateConnections_.count(pinToCheck) > 0)
			{
				break;
			}
			for (map<string, Pin>::const_iterator o_it = outputConnections_.begin();
					o_it != outputConnections_.end(); o_it++)
			{
				if (o_it->second == pinToCheck)
				{
					breakOuterLoop = true;
					break;
				}
			}
			if (breakOuterLoop)
			{
				break;
			}
			cerr << "Network: Gate " << gateName << ", output " << i
					<< " must be connected at least once." << endl;
			ret = false;
		}
	}

	if (ret == true)
	{
		cout << "Network: Consistency check: OK." << endl;
	}
	else
	{
		cerr << "Network: Consistency check: Erroneous Network, s. a." << endl;
	}

	return ret;
}

bool Network::connectOutput(std::string a_outputName, std::string a_outGateName,
		int a_outPinIdx)
{
	if (outputConnections_.count(a_outputName) == 0)
	{
		cerr << "Network: outputName: Output with name " << a_outputName
				<< " does not exist." << endl;
		return false;
	}
	if (!(outputConnections_[a_outputName].empty()))
	{
		cerr << "Network: outputName: Output with name " << a_outputName
				<< " has already been connected." << endl;
		return false;
	}
	if (gates_.count(a_outGateName) == 0)
	{
		cerr << "Network: outGateName: Gate with name " << a_outGateName
				<< " does not exist." << endl;
		return false;
	}
	if (gates_[a_outGateName].nOutputs() <= a_outPinIdx)
	{
		cerr << "Network: outGateIdx: Gate with name " << a_outGateName
				<< " has only " << gates_[a_outGateName].nOutputs()
				<< " outputs (idx " << a_outPinIdx << " requested)." << endl;
		return false;
	}

	Pin outPin(a_outGateName, a_outPinIdx);
	outputConnections_[a_outputName] = outPin;
	return true;
}

bool Network::connectInput(std::string a_inputName, std::string a_inGateName,
		int a_inPinIdx)
{
	if (inputConnections_.count(a_inputName) == 0)
	{
		cerr << "Network: inputName: Input with name " << a_inputName
				<< " does not exist." << endl;
		return false;
	}
	if (gates_.count(a_inGateName) == 0)
	{
		cerr << "Network: inGateName: Gate with name " << a_inGateName
				<< " does not exist." << endl;
		return false;
	}
	if (gates_[a_inGateName].nInputs() <= a_inPinIdx)
	{
		cerr << "Network: inGateIdx: Gate with name " << a_inGateName
				<< " has only " << gates_[a_inGateName].nInputs()
				<< " inputs (idx " << a_inPinIdx << " requested)." << endl;
		return false;
	}

	Pin inPin(a_inGateName, a_inPinIdx);
	if (inPinAlreadyConnected(inPin))
	{
		cerr << "Network: Gate " << a_inGateName << ", input " << a_inPinIdx
				<< " has already been connected." << endl;
		return false;
	}

	if (inputConnections_.count(a_inputName) == 0)
	{
		inputConnections_.insert(
				pair<string, vector<Pin> >(a_inputName, vector<Pin>()));
	}
	inputConnections_[a_inputName].push_back(inPin);
	return true;
}

bool Network::connectGates(std::string a_outGateName, int a_outPinIdx,
		std::string a_inGateName, int a_inPinIdx)
{
	if (gates_.count(a_outGateName) == 0)
	{
		cerr << "Network: outGateName: Gate with name " << a_outGateName
				<< " does not exist." << endl;
		return false;
	}
	if (gates_[a_outGateName].nOutputs() <= a_outPinIdx)
	{
		cerr << "Network: outGateIdx: Gate with name " << a_outGateName
				<< " has only " << gates_[a_outGateName].nOutputs()
				<< " outputs (idx " << a_outPinIdx << " requested)." << endl;
		return false;
	}
	if (gates_.count(a_inGateName) == 0)
	{
		cerr << "Network: inGateName: Gate with name " << a_inGateName
				<< " does not exist." << endl;
		return false;
	}
	if (gates_[a_inGateName].nInputs() <= a_inPinIdx)
	{
		cerr << "Network: inGateIdx: Gate with name " << a_inGateName
				<< " has only " << gates_[a_inGateName].nInputs()
				<< " inputs (idx " << a_inPinIdx << " requested)." << endl;
		return false;
	}

	Pin inPin(a_inGateName, a_inPinIdx);
	if (inPinAlreadyConnected(inPin))
	{
		cerr << "Network: Gate " << a_inGateName << ", input " << a_inPinIdx
				<< " has already been connected." << endl;
		return false;
	}

	Pin outPin(a_outGateName, a_outPinIdx);
	if (gate2GateConnections_.count(outPin) == 0)
	{
		gate2GateConnections_.insert(
				pair<Pin, vector<Pin> >(outPin, vector<Pin>()));
	}
	gate2GateConnections_[outPin].push_back(inPin);
	return true;
}




bool Network::inPinAlreadyConnected(Pin a_inPin) const
{
	for (map<Pin, vector<Pin> >::const_iterator it = gate2GateConnections_.begin();
			it != gate2GateConnections_.end(); it++)
	{
		for (int i = 0; i < static_cast<int>(it->second.size()); i++)
		{
			if (a_inPin == it->second[i])
			{
				return true;
			}
		}
	}
	for (map<string, vector<Pin> >::const_iterator it = inputConnections_.begin();
			it != inputConnections_.end(); it++)
	{
		for (int i = 0; i < static_cast<int>(it->second.size()); i++)
		{
			if (a_inPin == it->second[i])
			{
				return true;
			}
		}
	}
	return false;
}

bool Network::addOutput(const std::string &a_name)
{
	if (!assert_NameValid(a_name))
	{
		return false;
	}
	outputConnections_.insert(pair<string, Pin>(a_name, Pin()));
	return true;
}

bool Network::addInput(const std::string &a_name)
{
	if (!assert_NameValid(a_name))
	{
		return false;
	}
	inputConnections_.insert(pair<string, vector<Pin> >(a_name, vector<Pin>()));
	return true;
}

bool Network::addGate(const std::string &a_name, const GateLogic &a_logic)
{
	if (!assert_NameValid(a_name))
	{
		return false;
	}
	gates_.insert(pair<string, Gate>(a_name,Gate(a_name, &a_logic, propagationDelay_)));
	return true;
}

bool Network::assert_NameValid(std::string a_name) const
{
	if (a_name.empty())
	{
		cerr << "Network: Name may not be empty." << endl;
		return false;
	}
	if (gates_.count(a_name) > 0)
	{
		cerr << "Network: A gate with name " << a_name << " already exists."
				<< endl;
		return false;
	}
	if (inputConnections_.count(a_name) > 0)
	{
		cerr << "Network: An input with name " << a_name << " already exists."
				<< endl;
		return false;
	}
	if (outputConnections_.count(a_name) > 0)
	{
		cerr << "Network: An output with name " << a_name << " already exists."
				<< endl;
		return false;
	}
	return true;
}

 sgnl::Nanoseconds Network::getPropagationDelay_() const
{
	return propagationDelay_;
}

std::map<std::string, Gate> Network::getGates_() const
{
	return gates_;
}

std::map<std::string, std::vector<Pin> > Network::getInputConnections_() const
{
	return inputConnections_;
}

std::map<std::string, Pin> Network::getOutputConnections_() const
{
	return outputConnections_;
}

std::map<Pin, std::vector<Pin> > Network::getGate2GateConnections_() const
{
	return gate2GateConnections_;
}
