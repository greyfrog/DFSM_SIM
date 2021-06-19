/*! \file PredefinedGateLogics.h
 *  \brief Definitionen und Implementierungen der vordefinierten
 *  Gatelogiken. Diese legen Anzahl der Inputs und Outputs, Name sowie
 *  boole'sche Uebertragungsfunktion (outputsFor(...)) fest
 *  \author cpr
 *  \date 2020
 */

#ifndef GATELOGICS_H_
#define GATELOGICS_H_

#include "GateLogic.h"

/*! \namespace pgl
 *  \brief Vordefinierte Gatelogiken (predefined gate logics)
 */
namespace pgl
{

/*! \class AND2
 *  \brief OUT0 = IN0 ^ IN1
 */
class AND2: public GateLogic
{
public:
	AND2() :
			GateLogic(2, 1, "AND2")
	{
	}

	std::vector<bool> outputsFor(const std::vector<bool> &a_inputs,
			const std::vector<bool> &a_prevInputs,
			const std::vector<bool> &a_prevOutputs) const
	{
		assertDimensionsMatch(a_inputs, a_prevInputs, a_prevOutputs);
		std::vector<bool> out;
		out.resize(nOutputs_);
		out[0] = a_inputs[0] && a_inputs[1];
		return out;
	}
};
extern AND2 and2; /**< Vordefinierte Gatelogik */

/*! \class TFF
 *  \brief OUT0 = (!prevIN1^IN1)^!prevOUT0 v prevOUT0
 */
class TFF: public GateLogic
{
public:
	TFF() :
			GateLogic(1, 1, "TFF")
	{
	}

	std::vector<bool> outputsFor(const std::vector<bool> &a_inputs,
			const std::vector<bool> &a_prevInputs,
			const std::vector<bool> &a_prevOutputs) const
	{
		assertDimensionsMatch(a_inputs, a_prevInputs, a_prevOutputs);
		std::vector<bool> out;
		out.resize(nOutputs_);
		if (!a_prevInputs[0] && a_inputs[0]) // Positive Flanke!
		{
			out[0] = !a_prevOutputs[0];  // Output toggelt
		}
		else
		{
			out[0] = a_prevOutputs[0];  // Vorherigen Output halten
		}
		return out;
	}
};
extern TFF tff; /**< Vordefinierte Gatelogik */

/*! \class BUF1
 *  \brief OUT0 = IN0
 */
class BUF1: public GateLogic
{
public:
	BUF1() :
			GateLogic(2, 1, "BUF1")
	{
	}

	std::vector<bool> outputsFor(const std::vector<bool> &a_inputs,
			const std::vector<bool> &a_prevInputs,
			const std::vector<bool> &a_prevOutputs) const
	{
		assertDimensionsMatch(a_inputs, a_prevInputs, a_prevOutputs);
		std::vector<bool> out;
		out.resize(nOutputs_);
		out[0] = a_inputs[0];
		return out;
	}
};
extern BUF1 buf1; /**< Vordefinierte Gatelogik */

/*! \class NOT1
 *  \brief OUT0 = !IN0
 */
class NOT1: public GateLogic
{
public:
	NOT1() :
			GateLogic(1, 1, "NOT1")
	{
	}

	std::vector<bool> outputsFor(const std::vector<bool> &a_inputs,
			const std::vector<bool> &a_prevInputs,
			const std::vector<bool> &a_prevOutputs) const
	{
		assertDimensionsMatch(a_inputs, a_prevInputs, a_prevOutputs);
		std::vector<bool> out;
		out.resize(nOutputs_);
		out[0] = !a_inputs[0]; //Output toggelt
		return out;
	}
};
extern NOT1 not1; /**< Vordefinierte Gatelogik */
/*! \class OR2
 *  \brief OUT0 = IN0 ⋁  IN1
 */
class OR2: public GateLogic
{
public:
	OR2() :
			GateLogic(2, 1, "OR2")
	{
	}

	std::vector<bool> outputsFor(const std::vector<bool> &a_inputs,
			const std::vector<bool> &a_prevInputs,
			const std::vector<bool> &a_prevOutputs) const
	{
		assertDimensionsMatch(a_inputs, a_prevInputs, a_prevOutputs);
		std::vector<bool> out;
		out.resize(nOutputs_);
		out[0] = a_inputs[0] || a_inputs[1];
		return out;
	}
};
extern OR2 or2; /**< Vordefinierte Gatelogik */
/*! \class DFF
 *  \brief OUT0 = IN0, falls positive Flanke an IN1 erkannt, andernfalls OUT0 = OUT0alt (Halten)
 */
class DFF: public GateLogic
{
public:
	DFF() :
			GateLogic(2, 1, "DFF")
	{
	}

	std::vector<bool> outputsFor(const std::vector<bool> &a_inputs,
			const std::vector<bool> &a_prevInputs,
			const std::vector<bool> &a_prevOutputs) const
	{
		assertDimensionsMatch(a_inputs, a_prevInputs, a_prevOutputs);
		std::vector<bool> out;
		out.resize(nOutputs_);
		if (!a_prevInputs[1] && a_inputs[1]) // Positive Flanke an IN1
		{
			out[0] = a_inputs[0];

		}
		else
		{
			out[0] = a_prevOutputs[0];  // Vorherigen Output halten
		}

		return out;
	}
};
extern DFF dff; /**< Vordefinierte Gatelogik */
}  // namespace pgl
#endif /* GATELOGICS_H_ */
