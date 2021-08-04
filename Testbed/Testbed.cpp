/*
 * Testbed.cpp
 *
 *  Created on: Jun 27, 2021
 *      Author: Nutzer
 */
/*! \file Testbed.cpp
 *  \brief Implementierung der Klasse Testbed
 *  \author Thai Nguyen, Dung Tran
 *  \date 2021
 */
#include "Testbed.h"
#include <iostream>
#include <utility>
#include <algorithm>
using namespace std;
using namespace sgnl;
Testbed::Testbed(const Network &a_network) :
		network_(a_network)
{
	if (!network_.consistent())
	{
		cerr << "Das Network ist nicht konsistent" << endl;
	}
}

bool Testbed::addConstantSource(const string &a_inputName,
		const sgnl::Volt a_voltage)
{
	if (!assert_InputValid(a_inputName))
	{
		return false;
	}
	else
	{
		constant_.insert(
				pair<string, Constant>(a_inputName, Constant(a_voltage)));
		return true;
	}

}

bool Testbed::addClockSource(const string &a_inputName,
		const sgnl::Nanoseconds a_period, const sgnl::Nanoseconds a_phase)
{
	if (!assert_InputValid(a_inputName))
	{
		return false;
	}
	else
	{
		clock_.insert(
				pair<string, Clock>(a_inputName, Clock(a_period, a_phase)));
		return true;
	}
}

void Testbed::simulationstep(const sgnl::Nanoseconds a_duration)
{
	unsigned int duration = a_duration;
	map<string, vector<Volt>> inputTestResults;
	for (map<string, vector<Volt>>::iterator it = inputTestResults.begin();
			it != inputTestResults.end(); it++)
	{
		it->second.resize(duration);
	}

	map<string, vector<Volt>> outputTestResults;
	for (map<string, vector<Volt>>::iterator it = outputTestResults.begin();
			it != outputTestResults.end(); it++)
	{
		it->second.resize(duration);
	}

	for (unsigned int t = 0; t < duration; t++)
	{
		map<string, Volt> outputs = network_.outputs();
		for (map<string, Volt>::iterator it = outputs.begin();
				it != outputs.end(); it++)
		{
			string outName = it->first;
			Volt a_outVoltage = outputs.at(outName);
			outputTestResults[outName].push_back(a_outVoltage);
		}

		for (map<string, Clock>::iterator it = clock_.begin();
				it != clock_.end(); it++)
		{
			it->second.simulationStep();
			string inName = it->first;
			Volt a_inVoltage = clock_.at(inName).output();
			inputTestResults[inName].push_back(a_inVoltage);
		}

		for (map<string, Constant>::iterator it = constant_.begin();
				it != constant_.end(); it++)
		{
			it->second.simulationStep();
			string inName = it->first;
			Volt a_inVoltage = constant_.at(inName).output();
			inputTestResults[inName].push_back(a_inVoltage);
		}

		map<string, Volt> input;
		for (map<string, vector<Volt>>::iterator it = inputTestResults.begin();
				it != inputTestResults.end(); it++)
		{
			string inname = it->first;
			vector<Volt> inVoltages = it->second;
			input[inname] = inVoltages[t];

		}
		network_.simulationStep(input);

	}
    BMPPlot bmpPlot_(network_.name());
	bmpPlot_.setTimeTicks(duration / 10);

	for (map<string, vector<Volt>>::const_iterator it =
			inputTestResults.begin(); it != inputTestResults.end(); it++)
	{
		bmpPlot_.add(it->first, it->second);
	}

	for (map<string, vector<Volt>>::const_iterator it =
			outputTestResults.begin(); it != outputTestResults.end(); it++)
	{
		bmpPlot_.add(it->first, it->second);
	}
	bmpPlot_.save();
}

bool Testbed::assert_InputValid(const std::string &a_inputName) const
{
	std::vector<std::string> it = network_.inputNames();
	if (std::find(it.begin(), it.end(), a_inputName) == it.end())
	{
		cerr << "Network: Input with name " << a_inputName << " does not exist."
				<< endl;
		return false;
	}

	if (clock_.count(a_inputName) != 0)
	{
		cerr << "Network: Input with name " << a_inputName
				<< " has already been connected in Clock." << endl;
		return false;
	}

	if (constant_.count(a_inputName) != 0)
	{
		cerr << "Network: Input with name " << a_inputName
				<< " has already been connected in Constant." << endl;
		return false;
	}
	return true;

}
