/*! \file Pin.cpp
 *  \brief Implementierung der Klasse Pin
 *  \author cpr
 *  \date 2020
 */

#include "Pin.h"

using namespace std;


Pin::Pin() :
	gateName_(""),
	idx_(-1)
{

}

Pin::Pin(std::string a_gateName, int a_idx) :
	gateName_(a_gateName),
	idx_(a_idx)
{

}

std::string Pin::gateName() const
{
	return gateName_;
}

int Pin::idx() const
{
	return idx_;
}

bool Pin::empty() const
{
	return gateName_.empty();
}

bool Pin::operator==(const Pin& a_other) const
{
	return a_other.gateName_ == gateName_ && a_other.idx_ == idx_;
}

bool Pin::operator<(const Pin& a_other) const
{
	if(gateName_ == a_other.gateName_)
	{
		return idx_ < a_other.idx_;
	}
	return gateName_ < a_other.gateName_;
}
