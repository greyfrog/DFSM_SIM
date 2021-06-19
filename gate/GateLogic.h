/*! \file GateLogic.h
 *  \brief Definition der Klasse GateLogic
 *  \author cpr
 *  \date 2020
 */

#ifndef GATELOGIC_H_
#define GATELOGIC_H_

#include <vector>
#include <string>


/*! \class GateLogic
 *  \brief Eine Logikfunktion fuer ein Gate (Gatterlogik).
 *  Konkrete Gatelogiken erben von dieser Basisklasse und implementieren insb.
 *  die Methode outputsFor(...), welche die boolesche Uebertragungsfunktion
 *  realisiert.
 */
class GateLogic
{
public:
	/*! \brief Konstruktor. Initialisiert die Gatelogik mit den uebergebenen
	 *  Parameterwerten
	 *  \param a_nInputs Anzahl der Eingaenge
	 *  \param a_nOutputs Anzahl der Ausgaenge
	 *  \param a_name Name
	 */
	 GateLogic(
		int a_nInputs,
		int a_nOutputs,
		std::string a_name);

	/*! \brief Destruktor
	 */
	virtual ~GateLogic();

	/*! \brief Boole'sche Uebertragungsfunktion. Gibt die Output-Logiklevel
	 *  zurueck fuer die uebergebenen Input-Logiklevel im aktuellen und
	 *  vorherigen Simulationsschritt sowie Output-Logiklevel im vorherigen
	 *  Simulationsschritt.
	 *  Diese Methode ist durch die vordefinierten Gatelogiken zu
	 *  implementieren.
	 *  \param a_inputs Input-Logiklevel im aktuellen Simulationsschritt
	 *  \param a_prevInputs Input-Logiklevel im vorherigen
	 *  Simulationsschritt
	 *  \param a_prevOutputs Output-Logiklevel im vorherigen
	 *  Simulationsschritt
	 *  \return Output-Logiklevel im aktuellen Simulationsschritt
	 */
	virtual std::vector<bool> outputsFor(const
		std::vector<bool> &a_inputs,
		const std::vector<bool> &a_prevInputs,
		const std::vector<bool> &a_prevOutputs)const = 0  ;

	/*! \brief Gibt die Anzahl der Eingaenge zurueck
	 *  \return Anzahl der Eingaenge
	 */
	 int nInputs() const;

	/*! \brief Gibt die Anzahl der Ausgaenge zurueck
	 *  \return Anzahl der Ausgaenge
	 */
	 int nOutputs() const;

	/*! \brief Gibt den Namen zurueck
	 *  \return Name
	 */
	 std::string name() const;

protected:
	/*! \brief Gibt eine passende Fehlermeldung aus, wenn die Anzahlen der
	 *  uebergebenen Logiklevel nicht zu den Anzahlen der Inputs und Outputs
	 *  passen
	 *  \param a_inputs Input-Logiklevel im aktuellen Simulationsschritt
	 *  \param a_prevInputs Input-Logiklevel im vorherigen Simulationsschritt
	 *  \param a_prevOutputs Output-Logiklevel im vorherigen Simulationsschritt
	 *  \return true, falls die Anzahlen der uebergebenen Logiklevel zu den
	 *  Anzahlen der Inputs und Outputs passen, false sonst
	 */
	bool assertDimensionsMatch(
		const std::vector<bool>& a_inputs,
		const std::vector<bool>& a_prevInputs,
		const std::vector<bool>& a_prevOutputs) const;

	int nInputs_;  		/**< Anzahl der Inputs */
	int nOutputs_;  	/**< Anzahl der Outputs */
	std::string name_;  /**< Name */
};

#endif /* GATELOGIC_H_ */
