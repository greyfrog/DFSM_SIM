/*! \file Gate.h
 *  \brief Definition der Klasse Gate
 *  \author cpr
 *  \date 2020
 */

#ifndef GATE_H_
#define GATE_H_

#include "GateLogic.h"
#include "PredefinedGateLogics.h"
#include "../signal/Signal.h"

#include <vector>
#include <string>


/*! \class Gate
 *  \brief Modelliert ein digitales Logikgatter. Die Gatelogik wird
 *  ueber ein Pointer-Member auf eines der vordefinierten Gatelogik-Objekte
 *  bereitgestellt
 */
class Gate
{
public:
	/*! \brief Parameterloser Konstruktor. Dieser wird allein benoetigt, um
	 *  std::map::operator[] mit Gate als value-type verwendbar zu machen.
	 *  Es wird ein nicht verwendbares Gate erzeugt!
	 */
	Gate();

	/*! \brief Konstruktor. Initialisiert das Gate mit den uebergebenen
	 *  Parameterwerten. Die Groessen der Vektoren fuer Eingangs-,
	 *  Ausgangsspannungen und Logiklevel werden entsprechend der in der
	 *  uebergebenen Gatelogik hinterlegten Anzahlen an Inputs und Outputs
	 *  angepasst und die hinterlegten Werte mit 0V bzw. false initialisiert
	 *  \param a_name Name
	 *  \param a_pLogic Pointer auf ein vordefiniertes Gatelogik-Objekt
	 *  \param a_propagationDelay Gatterlaufzeit
	 */
	Gate(
		std::string a_name,
		const GateLogic* a_pLogic,
		sgnl::Nanoseconds a_propagationDelay);

	/*! \brief Setzt den Input mit dem angegeben Index auf den uebergebenen
	 *  Spannungswert
	 *  \param a_idx Nummer (Index) des Inputs
	 *  \param a_voltage Zu setzende Spannung an diesem Input
	 */
	void setInput(int a_idx, sgnl::Volt a_voltage);

	/*! \brief Fuehrt einen Simulationsschritt durch (einen Schritt von
	 *  der einen Nanosekunde auf die naechste).
	 *  \details Ablauf:
	 *  - Eingangs-Logiklevel aus Eingangsspannungen ermitteln
	 *  - Neue Ausgangs-Logiklevel entsprechend der Gatelogik ermitteln
	 *  - Ausgangsspannungen aus Ausgangs-Logikleveln ermitteln
	 *  - Ausgangsspannungen mit Verzoegerung durch Gatterlaufzeit und
	 *  mit Rauschen beaufschlagen
	 *  - Neue Werte fuer naechsten Simulationsschritt merken
	 */
	void simulationStep();

	/*! \brief Gibt die Ausgangsspannungen im aktuellen Simulationsschritt
	 * zurueck
	 *  \return Ausgangsspannungen im aktuellen Simulationsschritt
	 */
	std::vector<sgnl::Volt> outputs() const;

	/*! \brief Gibt die Anzahl der Inputs zurueck
	 *  \return Anzahl der Inputs
	 */
	int nInputs()const;

	/*! \brief Gibt die Anzahl der Outputs zurueck
	 *  \return Anzahl der Outputs
	 */
	int nOutputs() const;

	/*! \brief Gibt den Namen der Gatelogik zurueck
	 *  \return Name der Gatelogik
	 */
	std::string logicName() const;

private:
	const GateLogic* pLogic_;  /**< Pointer auf ein vordefiniertes
		Gatelogik-Objekt. Dieses bestimmt, welche Logikfunktion dieses Gate
		erfuellt */
	sgnl::Nanoseconds propagationDelay_;  /**< Gatterlaufzeit */
	std::string name_;  /**< Name */

	std::vector<sgnl::Volt> prevInputs_;   /**< Eingangsspannungen
		im vorherigen Simulationsschritt */
	std::vector<sgnl::Volt> inputs_;       /**< Eingangsspannungen
		im aktuellen Simulationsschritt */
	std::vector<bool> prevInputs_logical_;     /**< Eingangs-Logiklevel
			im vorherigen Simulationssschritt */
	std::vector<sgnl::Volt> prevOutputs_;  /**< Ausgangsspannungen
		im vorherigen Simulationssschritt */
	std::vector<bool> prevOutputs_logical_;     /**< Ausgangs-Logiklevel
		im vorherigen Simulationssschritt */
	std::vector<sgnl::Volt> outputs_;      /**< Ausgansspannungen
		im aktuellen Simulationsschritt */

	/*! \brief Beaufschlagt die uebergebenen Ausgangsspannungen mit der
	 *  Verzoegerung infolge der Gatterlaufzeit und gibt sie zurueck. Dafuer
	 *  werden die vorherigen Werte der Ausgangsspannungen herangezogen
	 *  \param a_outputs Ausgangsspannungen
	 *  \return Ausgangsspannungen, verzoegert infolge der Gatterlaufzeit
	 */
	std::vector<sgnl::Volt> applyPropagationDelay(
		std::vector<sgnl::Volt> a_outputs);
};

#endif /* GATE_H_ */
