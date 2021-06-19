/*! \file Signal.cpp
 *  \brief Definitionen und Instanzierungen im namespace signal
 *  \author cpr
 *  \date 2020
 */

#include "Signal.h"


namespace sgnl
{
	using namespace std;

	///@cond INTERNAL
	std::random_device rd_;
	std::mt19937 gen_(rd_());
	std::uniform_int_distribution<> distr_(-999, 999);
	///@endcond

	sgnl::Volt applyNoise(sgnl::Volt a_voltage)
	{
		return a_voltage + static_cast<double>(distr_(gen_))
								/(1000.0/NOISE_ABS_MAX_VOLT);
	}

	std::vector<sgnl::Volt> applyNoise(
		std::vector<sgnl::Volt> a_voltages)
	{
		vector<Volt> ret;
		ret.resize(a_voltages.size());
		for(int i=0; i<static_cast<int>(a_voltages.size()); i++)
		{
			ret[i] = applyNoise(a_voltages[i]);
		}
		return ret;
	}

	std::vector<sgnl::Volt> logicalToVolt(
		std::vector<bool> a_logicLevels)
	{
		vector<Volt> ret;
		ret.resize(a_logicLevels.size());
		for(int i=0; i<static_cast<int>(a_logicLevels.size()); i++)
		{
			if(a_logicLevels[i] == true)
			{
				ret[i] = HIGH_LEVEL_VOLT;
			}
			else
			{
				ret[i] = LOW_LEVEL_VOLT;
			}
		}
		return ret;
	}

	std::vector<bool> voltageToLogical(
		std::vector<sgnl::Volt> a_voltages,
		std::vector<bool> a_prevLogicLevels)
	{
		vector<bool> ret;
		ret.resize(a_voltages.size());
		for(int i=0; i<static_cast<int>(a_voltages.size()); i++)
		{
			if(a_voltages[i] >= HIGH_THRESH_VOLT+DEADBAND_VOLT/2)
			{
				ret[i] = true;
			}
			else if(a_voltages[i] < HIGH_THRESH_VOLT+DEADBAND_VOLT/2
				 && a_voltages[i] > HIGH_THRESH_VOLT-DEADBAND_VOLT/2)
			{
				ret[i] = a_prevLogicLevels[i];
			}
			else
			{
				ret[i] = false;
			}
		}
		return ret;
	}

}  // namespace signal
