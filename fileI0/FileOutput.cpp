/*! \file FileOutput.cpp
 *  \brief Implementierung der Klasse FileOutput
 *  \author cpr
 *  \date 2020
 */

#include "FileOutput.h"

#include <iostream>
#include <fstream>
#include <ios>

using namespace std;

bool FileOutput::save(const Network &a_network, const std::string &a_fileName)
{
	string path("Saves/");
	path += a_fileName + ".nw";
	ofstream file;
	file.open(path.c_str());
	if (!file.is_open())
	{
		cerr << "Cannot open save file at \"" << path << "\"." << endl;
		return false;
	}

	file << "{\n";

	// TODO: Alle Bloecke ins Save-File (file) schreiben (vorhandene Methoden
	// nutzen!)
	// ...
	file << "    name:\n";
	//Name section
	file << makeNameBlock(a_network.name());
//delay section
	file << "    propagationDelay:\n";
	file << makePropagationDelayBlock(a_network.getPropagationDelay_());
//input section
	file << "    inputs:\n";
	file << makeInputsBlock(a_network.inputNames());
//output section
	file << "    outputs:\n";
	file << makeOutputsBlock(a_network.outputNames());
//gates section
	file << "    gates:\n";
	file << makeGatesBlock(a_network.getGates_());
//connection section
	int counterLoop = 0;
	file << "    connections:\n";
	file << "     {\n";
	std::map<Pin, std::vector<Pin>> gate2GateConnections =
			a_network.getGate2GateConnections_();
	for (std::map<Pin, std::vector<Pin>>::const_iterator it =
			gate2GateConnections.begin(); it != gate2GateConnections.end();
			it++)

	{
		file << "        [" << to_string(counterLoop) << "]" << ":\n";
		file << makeGate2GateConnectionBlock(it->first, it->second);
		counterLoop++;
	}

//network input to gate
	std::map<std::string, std::vector<Pin>> InputConnections =
			a_network.getInputConnections_();
	for (std::map<std::string, std::vector<Pin>>::const_iterator it =
			InputConnections.begin(); it != InputConnections.end(); it++)
	{
		file << "        [" << to_string(counterLoop) << "]" << ":\n";
		file << makeInputConnectionBlock(it->first, it->second);
		counterLoop++;
	}

//network output to gate
	std::map<std::string, Pin> OutputConnections =
			a_network.getOutputConnections_();
	for (std::map<std::string, Pin>::const_iterator it =
			OutputConnections.begin(); it != OutputConnections.end(); it++)
	{
		file << "        [" << to_string(counterLoop) << "]" << ":\n";
		file << makeOutputConnectionBlock(it->first, it->second);
		counterLoop++;
	}
	file << "     }\n";
	file << "}\n";
	file.close();
	return true;
}

std::string FileOutput::makeNameBlock(const std::string &a_name)
{
	string ret;
	ret += "    {\n";
	ret += "        " + a_name + "\n";
	ret += "    }\n";
	return ret;
}

std::string FileOutput::makePropagationDelayBlock(int a_propagationDelay)
{
	string ret;
	ret += "    {\n";
	ret += "        " + to_string(a_propagationDelay) + "\n";
	ret += "    }\n";
	return ret;
}

std::string FileOutput::makeInputsBlock(
		const std::vector<std::string> &a_inputNames)
{
	string ret;
	ret += "    {\n";
	for (int i = 0; i < static_cast<int>(a_inputNames.size()); i++)
	{
		ret += "        " + a_inputNames[i] + "\n";
	}
	ret += "    }\n";
	return ret;
}

std::string FileOutput::makeOutputsBlock(
		const std::vector<std::string> &a_outputNames)
{
	string ret;
	ret += "    {\n";
	for (int i = 0; i < static_cast<int>(a_outputNames.size()); i++)
	{
		ret += "        " + a_outputNames[i] + "\n";
	}
	ret += "    }\n";
	return ret;
}

std::string FileOutput::makeGatesBlock(
		const std::map<std::string, Gate> &a_gates)
{
	string ret;
	ret += "    {\n";
	int g = 0;
	for (map<string, Gate>::const_iterator it = a_gates.begin();
			it != a_gates.end(); it++)
	{
		ret += "        [" + to_string(g) + "]:\n";
		ret += "        {\n";
		ret += "            " + it->first + "\n";
		ret += "            " + it->second.logicName() + "\n";
		ret += "        }\n";
		g++;
	}
	ret += "    }\n";
	return ret;
}

std::string FileOutput::makeGate2GateConnectionBlock(const Pin &a_gateOutputPin,
		const std::vector<Pin> &a_gateInputPins)
{
	string ret;
	ret += "        {\n";
	ret += "            from:\n";
	ret += "            {\n";
	ret += "                gate\n";
	ret += "                " + a_gateOutputPin.gateName() + "\n";
	ret += "                " + to_string(a_gateOutputPin.idx()) + "\n";
	ret += "            }\n";
	ret += "            to:\n";
	ret += "            {\n";
	for (int i = 0; i < static_cast<int>(a_gateInputPins.size()); i++)
	{
		ret += "                [" + to_string(i) + "]:\n";
		ret += "                {\n";
		ret += "                    gate\n";
		ret += "                    " + a_gateInputPins[i].gateName() + "\n";
		ret += "                    " + to_string(a_gateInputPins[i].idx())
				+ "\n";
		ret += "                }\n";
	}
	ret += "            }\n";
	ret += "        }\n";
	return ret;
}

std::string FileOutput::makeOutputConnectionBlock(
		const std::string &a_outputName, const Pin &a_gateOutputPin)
{
	string ret;
	ret += "        {\n";
	ret += "            from:\n";
	ret += "            {\n";
	ret += "                gate\n";
	ret += "                " + a_gateOutputPin.gateName() + "\n";
	ret += "                " + to_string(a_gateOutputPin.idx()) + "\n";
	ret += "            }\n";
	ret += "            to:\n";
	ret += "            {\n";
	ret += "                [0]:\n";
	ret += "                {\n";
	ret += "                    network-output\n";
	ret += "                    " + a_outputName + "\n";
	ret += "                }\n";
	ret += "            }\n";
	ret += "        }\n";
	return ret;
}

std::string FileOutput::makeInputConnectionBlock(const std::string &a_inputName,
		const std::vector<Pin> &a_gateInputPins)
{
	string ret;
	ret += "        {\n";
	ret += "            from:\n";
	ret += "            {\n";
	ret += "                network-input\n";
	ret += "                " + a_inputName + "\n";
	ret += "            }\n";
	ret += "            to:\n";
	ret += "            {\n";
	for (int i = 0; i < static_cast<int>(a_gateInputPins.size()); i++)
	{
		ret += "                [" + to_string(i) + "]:\n";
		ret += "                {\n";
		ret += "                    gate\n";
		ret += "                    " + a_gateInputPins[i].gateName() + "\n";
		ret += "                    " + to_string(a_gateInputPins[i].idx())
				+ "\n";
		ret += "                }\n";
	}
	ret += "            }\n";
	ret += "        }\n";
	return ret;
}
