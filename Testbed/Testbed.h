/*
 * Testbed.h
 *
 *  Created on: Jun 27, 2021
 *      Author: Nutzer
 */
/*! \file Testbed.h
 *  \brief Definition der Klasse Testbed
 *  \author Thai Nguyen, Dung Tran
 *  \date 2020
 */
#include "../source/Clock.h"
#include "../source/Constant.h"
#include "../network/Network.h"
#include "../plot/BMPPlot.h"
#include "../plot/simplebmp/simplebmp.h"
#ifndef TESTBED_TESTBED_H_
#define TESTBED_TESTBED_H_
/*! \class Testbed
 * \brief Modelliert ein Pruefstand. Diese Stellt Signalquellen, Taktsignale zur Verfuegung. Testbed uebernimmt die Simulation
 */
class Testbed
{
private:
	Network network_; /**< Network*/
	std::map<std::string, Clock> clock_;/**< Rechecktquelle des Networks*/
	std::map<std::string, Constant> constant_; /**< Konstantquelle des Networks*/

public:
	/*! \brief Konstruktor des Testbed
	 *  \param a_network Ein Network
	 */
	Testbed(const Network &a_network);
	/*! \brief Hinzufuegen die Konstantquelle des Networks
	 * \param a_inputName Network Input
	 * \param a_voltage gewuenschte Spannung
	 *  \return false wenn Input Name schon existiert ist, sonst hinzufuegen Konstantquelle und return true
	 */
	bool addConstantSource(const std::string &a_inputName,
			const sgnl::Volt a_voltage);
	/*! \brief Hinzufuegen die Rechteckquelle des Networks
	 * \param a_inputName Network Input
	 * \param a_phase Phase der Rechteckquelle
	 * \param a_period Periode der Rechteckquelle
	 *  \return false wenn Input Name schon existiert ist, sonst hinzufuegen Rechteckquelle und return true
	 */
	bool addClockSource(const std::string &a_inputName,
			const sgnl::Nanoseconds a_period, const sgnl::Nanoseconds a_phase);
	/*! \brief  Das Network muss einen Simulationsschritt durchfuehren koennen.
	 *  \param a_duration Simulationsdauer
	 */
	void simulationstep(const sgnl::Nanoseconds a_duration);
	/*! \brief Check ob Input Name gueltig ist
	 * \return bool true wenn Name gueltig, sonst false
	 */
	bool assert_InputValid(const std::string &a_inputName) const;

};

#endif /* TESTBED_TESTBED_H_ */
