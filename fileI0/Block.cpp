#include <iostream>
#include "Block.h"

using namespace std;


void Block::prettyPrint() const
{
	cout << "Block data printout:" << endl;
	cout << "====================" << endl << endl;
	printIndented(0);
}

void Block::addValue(const std::string& a_value)
{
	if(children_.size()>0)
	{
		cerr << "Warning: Block should only contain EITHER values OR "
				"child blocks, not both." << endl;
	}
	values_.push_back(a_value);
	return;
}

void Block::addChild(const std::string& a_name, const Block& a_block)
{
	if(values_.size() > 0)
	{
		cerr << "Warning: Block should only contain EITHER values OR "
				"child blocks, not both." << endl;
	}
	if(children_.count(a_name) > 0)
	{
		cerr << "Warning: Child block with name " << a_name
			 << " already exists!" << endl;
	}
	children_[a_name] = a_block;
	return;
}

int Block::nChildren() const
{
	return static_cast<int>(children_.size());
}

Block Block::child(const std::string& a_name) const
{
	if(!hasChild(a_name))
	{
		cerr << "Warning: Child with name " << a_name << " does not exist, use"
				"addChild(...) first!" << endl;
		return children_.end()->second;
	}
	return children_.at(a_name);
}

Block Block::child(int a_idx) const
{
	if(a_idx >= static_cast<int>(children_.size()))
	{
		cerr << "Warning: Block has only " << children_.size() << " children "
				" (tried to access index " << a_idx << ")." << endl;
		return children_.end()->second;
	}
	map<string, Block>::const_iterator it = children_.begin();
	advance(it, a_idx);
	return it->second;
}

bool Block::hasChild(const std::string& a_name) const
{
	return (children_.count(a_name)>0);
}

int Block::nValues() const
{
	return static_cast<int>(values_.size());
}

std::string Block::value(int a_idx) const
{
	if(a_idx>=static_cast<int>(values_.size()))
	{
		cerr << "Warning: Block has only " << values_.size() << " values "
				" (tried to access index " << a_idx << ")." << endl;
		return "";
	}
	return values_.at(a_idx);
}

void Block::printIndented(int a_indent) const
{
	cout << string(a_indent, ' ') << "{" << endl;
	for(int i=0; i<static_cast<int>(values_.size()); i++)
	{
		cout << string(a_indent, ' ') << "  " << values_[i] << endl;
	}
	for(map<string, Block>::const_iterator
			it  = children_.begin();
			it != children_.end();
			it++)
	{
		cout << string(a_indent, ' ') << "  " << it->first << ":" << endl;
		it->second.printIndented(a_indent+2);
	}
	cout << string(a_indent, ' ') << "}" << endl;
	return;
}
