/*! \file FileInput.cpp
 *  \brief Implementierung der Klasse FileInput
 *  \author cpr
 *  \date 2020
 */

#include "FileInput.h"

#include <iostream>
#include <ios>
#include <streambuf>
#include <fstream>

using namespace std;

Network FileInput::load(const std::string &a_fileName)
{
	if (!fileContentsToLines(a_fileName))
	{
		return Network("", 0);
	}
	int lineNumber = 0;
	Block networkBlock = parse(lineNumber);
	networkBlock.prettyPrint();
	cout << endl << endl;
	return blockToNetwork(networkBlock);
}

Block FileInput::parse(int &a_lineNumber) const
{
	Block block;
	ReadState state = ReadState::EXPECTING_OPENER;
	while (a_lineNumber < static_cast<int>(fileLines_.size()))
	{

		string line = fileLines_[a_lineNumber];
		ReadEvent event = getEvent(line);
		switch (state)
		{
		case ReadState::EXPECTING_OPENER:
			if (event == ReadEvent::OPENER)
			{
				a_lineNumber++;
				state = ReadState::EXPECTING_VALUE_OR_NAME;
				break;
			}
			error(state, event, a_lineNumber);
			return block;
		case ReadState::EXPECTING_VALUE_OR_NAME:
			if (event == ReadEvent::NAME)
			{
				std::string childName;
				Block child;
				childName = getBlockName(line);
				a_lineNumber++;
				child = parse(a_lineNumber);
				block.addChild(childName, child);
				state = ReadState::EXPECTING_NAME_OR_CLOSER;
				break;
			}
			if (event == ReadEvent::VALUE)
			{
				block.addValue(line);
				a_lineNumber++;
				state = ReadState::EXPECTING_VALUE_OR_CLOSER;
				break;
			}
			error(state, event, a_lineNumber);
			return block;

		case ReadState::EXPECTING_VALUE_OR_CLOSER:

			if (event == ReadEvent::VALUE)
			{
				block.addValue(line);
				a_lineNumber++;
				state = ReadState::EXPECTING_VALUE_OR_CLOSER;
				break;
			}
			if (event == ReadEvent::CLOSER)
			{
				a_lineNumber++;
				state = ReadState::EXPECTING_NAME_OR_CLOSER;
				return block;
				break;
			}
			error(state, event, a_lineNumber);
			return block;
		case ReadState::EXPECTING_NAME_OR_CLOSER:
			if (event == ReadEvent::NAME)
			{
				std::string childName;
				Block child;
				childName = getBlockName(line);
				a_lineNumber++;
				child = parse(a_lineNumber);
				block.addChild(childName, child);
				state = ReadState::EXPECTING_NAME_OR_CLOSER;
				break;
			}
			if (event == ReadEvent::CLOSER)
			{
				a_lineNumber++;
				state = ReadState::EXPECTING_NAME_OR_CLOSER;
				return block;
				break;
			}
			error(state, event, a_lineNumber);
			return block;
		default:

			error(state, event, a_lineNumber);
			return block;
		}
	}

	cerr << "Unexpected end-of-file at line " << a_lineNumber + 1 << endl;
	return block;
}

void FileInput::error(ReadState a_state, ReadEvent a_event,
		int a_lineNumber) const
{
	cerr << "Parser error: Line " << a_lineNumber + 1 << ": In state "
			<< static_cast<int>(a_state) << ": Unexpected event "
			<< static_cast<int>(a_event) << "." << endl;
	return;
}

FileInput::ReadEvent FileInput::getEvent(const std::string &a_line) const
{
	if (a_line.empty())
	{
		cerr << "Line may not be empty!" << endl;
		return ReadEvent::VALUE;
	}
	if (a_line.at(a_line.size() - 1) == ':')
	{
		return ReadEvent::NAME;
	}
	else if (a_line.at(0) == '{')
	{
		return ReadEvent::OPENER;
	}
	else if (a_line.at(0) == '}')
	{
		return ReadEvent::CLOSER;
	}
	else
	{
		return ReadEvent::VALUE;
	}
}

std::string FileInput::getBlockName(const std::string &a_line) const
{
	if (a_line.empty())
	{
		cerr << "Line may not be empty!" << endl;
		return string("");
	}
	return a_line.substr(0, a_line.size() - 1);
}

Network FileInput::blockToNetwork(const Block &a_networkBlock) const
{
	// Lies Network-Name
	if (!a_networkBlock.hasChild("name"))
	{
		cerr << "network: Child \"name\" missing." << endl;
		return Network("", 0);
	}
	string name = a_networkBlock.child("name").value(0);

	// Lies Network-Gatterlaufzeit
	if (!a_networkBlock.hasChild("propagationDelay"))
	{
		cerr << "network: Child \"propagationDelay\" missing." << endl;
		return Network("", 0);
	}
	sgnl::Nanoseconds propagationDelay = toPositiveInteger(
			a_networkBlock.child("propagationDelay").value(0));

	// Setze Network-Name und Gatterlaufzeit
	Network ret(name, propagationDelay);

	// Lies Network-Inputs und fuege sie hinzu
	if (!a_networkBlock.hasChild("inputs"))
	{
		cerr << "network: Child \"inputs\" missing." << endl;
		return ret;
	}
	Block inputs = a_networkBlock.child("inputs");
	for (int i = 0; i < inputs.nValues(); i++)
	{
		ret.addInput(inputs.value(i));
	}

	// Lies Network-Outputs und fuege sie hinzu
	if (!a_networkBlock.hasChild("outputs"))
	{
		cerr << "network: Child \"outputs\" missing." << endl;
		return ret;
	}
	Block outputs = a_networkBlock.child("outputs");
	for (int i = 0; i < outputs.nValues(); i++)
	{
		ret.addOutput(outputs.value(i));
	}

	// Lies Gates und fuege sie hinzu
	if (!a_networkBlock.hasChild("gates"))
	{
		cerr << "network: Child \"gates\" missing." << endl;
		return ret;
	}
	Block gates = a_networkBlock.child("gates");
	for (int i = 0; i < gates.nChildren(); i++)
	{
		Block gate = gates.child(i);
		if (gate.nValues() < 2)
		{
			cerr << "gate: Need name and logic (2 parameters)." << endl;
			return ret;
		}
		string name = gate.value(0);
		string logic_str = gate.value(1);
		const GateLogic &logic = logicFromString(logic_str);
		ret.addGate(name, logic);
	}

	// Lies Network-Verbindungen und fuege sie hinzu
	if (!a_networkBlock.hasChild("connections"))
	{
		cerr << "network: Child \"connections\" missing." << endl;
		return ret;
	}
	Block connections = a_networkBlock.child("connections");
	for (int i = 0; i < connections.nChildren(); i++)
	{
		Block connection = connections.child(i);

		// Lies gemeinsame Quelle (Output) der Verbindungen
		if (!connection.hasChild("from"))
		{
			cerr << "connection " << i << ": Child \"from\" missing." << endl;
			return ret;
		}
		Block from = connection.child("from");
		if (from.nValues() < 2)
		{
			cerr << "connection " << i << ", from: Need type and name "
					"(2 parameters) or type, name and output pin index "
					"(3 parameters)." << endl;
			return ret;
		}
		string fromType = from.value(0);
		string fromName = from.value(1);
		Pin fromPin;
		if (fromType == "gate")
		{
			if (from.nValues() < 3)
			{
				cerr << "connection " << i << ", from: Type is gate, then "
						"parameter \"output pin index\" must exist." << endl;
				return ret;
			}
			int fromPinIndex = FileInput::toPositiveInteger(from.value(2));
			fromPin = Pin(fromName, fromPinIndex);
		}

		// Lies Ziele (Inputs) der Verbindungen
		if (!connection.hasChild("to"))
		{
			cerr << "connection " << i << ": Child \"to\" missing." << endl;
			return ret;
		}
		Block tos = connection.child("to");
		for (int k = 0; k < tos.nChildren(); k++)
		{
			Block to = tos.child(k);
			if (to.nValues() < 2)
			{
				cerr << "connection " << i << ", to no. " << k << ": Need type "
						"and name (2 parameters) or type, name and input pin "
						"index (3 parameters)." << endl;
				return ret;
			}
			string toType = to.value(0);
			string toName = to.value(1);
			Pin toPin;
			if (toType == "gate")
			{
				if (to.nValues() < 3)
				{
					cerr << "connection " << i << ", to no. " << k
							<< ": Type is "
									"gate, then parameter \"input pin index\" "
									"must exist." << endl;
					return ret;
				}
				int toPinIndex = FileInput::toPositiveInteger(to.value(2));
				toPin = Pin(toName, toPinIndex);
			}

			// Fuege Verbindungen hinzu
			if (fromType == "gate")
			{
				if (toType == "gate")
				{
					ret.connectGates(fromPin.gateName(), fromPin.idx(),
							toPin.gateName(), toPin.idx());
				}
				else if (toType == "network-output")
				{
					ret.connectOutput(toName, fromPin.gateName(),
							fromPin.idx());
				}
				else
				{
					cerr << "connection " << i << ", to no. " << k
							<< ": Unknown "
									"to-type: " << toType << endl;
					return ret;
				}
			}
			else if (fromType == "network-input")
			{
				if (toType == "gate")
				{
					ret.connectInput(fromName, toPin.gateName(), toPin.idx());
				}
				else if (toType == "network-output")
				{
					cerr << "connection " << i << ", to no. " << k
							<< ": Invalid connection: Cannot connect network input"
									" with network output directly." << endl;
				}
				else
				{
					cerr << "connection " << i << ", to no. " << k
							<< ": Unknown to-type: " << toType << endl;
					return ret;
				}
			}
			else
			{
				cerr << "connection " << i << ", to no. " << k << ": Unknown "
						"from type: " << fromType << endl;
				return ret;
			}
		}
	}
	ret.prettyPrint();
	return ret;
}

bool FileInput::fileContentsToLines(const std::string &a_fileName)
{
	fileLines_.clear();
	string path("Saves/");
	path += a_fileName + ".nw";
	ifstream file;
	file.open(path.c_str());
	vector<string> ret;
	if (!file.is_open())
	{
		cerr << "Cannot open load file at \"" << path << "\"." << endl;
		return false;
	}
	string line;
	while (getline(file, line))
	{
		trimLine(line);
		if (!line.empty())  // keine Leerzeilen
		{
			fileLines_.push_back(line);
		}
	}
	file.close();
	return true;
}

void FileInput::trimLine(std::string &a_line) const
{
	string whitespace(" \t");
	size_t strBegin = a_line.find_first_not_of(whitespace);
	if (strBegin == string::npos)
	{
		a_line = "";
		return;
	}
	size_t strEnd = a_line.find_last_not_of(whitespace);
	size_t strRange = strEnd - strBegin + 1;
	a_line = a_line.substr(strBegin, strRange);
}

int FileInput::toPositiveInteger(const std::string &a_value) const
{
	int integer = -1;
	try
	{
		integer = stoi(a_value);
	} catch (...)
	{
		cerr << a_value << " is not an integer." << endl;
	}
	if (integer < 0)
	{
		cerr << "Must be positive (have " << integer << ")." << endl;
	}
	return integer;
}

const GateLogic& FileInput::logicFromString(const std::string &a_str) const
{
	if (a_str == "DFF")
		return pgl::dff;
	if (a_str == "TFF")
		return pgl::tff;
	if (a_str == "BUF1")
		return pgl::buf1;
	if (a_str == "AND2")
		return pgl::and2;
	if (a_str == "OR2")
		return pgl::or2;
	if (a_str == "NOT1")
		return pgl::not1;
	cerr << "Unknown GateLogic: " << a_str << ", defaulting to BUF1." << endl;
	return pgl::buf1;
}
