/*
 * Network.h
 *
 *  Created on: Apr 27, 2021
 *      Author: Nutzer
 */

#ifndef GATE_NETWORK_H_
#define GATE_NETWORK_H_
#include <string>
#include "../signal/Signal.h"
#include "../gate/Gate.h"
#include <map>
#include "../gate/Pin.h"
using namespace sgnl;
class Network
{
private:
	std::map<std::string, Gate> gates_; /**<Gate des Networks, man kann per Angabe des Namens auf Gate zugreifen*/
	std::map<std::string, std::vector<Pin> > inputConnections_;/**<die Verbindungen zwischen den Network-Inputs und den Gate-Input-Pins*/
	std::map<std::string, Pin> outputConnections_;/**<die Verbindungen zwischen den Gate-Output-Pins und den Network-Outputs*/
	std::map<Pin, std::vector<Pin> > gate2GateConnections_; /**<die Verbindungen zwischen den Gates*/
	std::string name_; /**< Name des Networks */
	sgnl::Nanoseconds propagationDelay_; /**< Gatterlaufzeit des Networks*/

public:
	//get Methode
	/*! \brief Gibt die Gatterlaufzeit des Networks aus
	 *  \return die Gatterlaufzeit in Nanosecs
	 */
     sgnl::Nanoseconds getPropagationDelay_() const ;
 	/*! \brief Gibt die Gate des Networks aus
 	 *  \return die Gate des Networks
 	 */
	std::map<std::string, Gate> getGates_() const ;
 	/*! \brief Gibt die Verbindungen zwischen den Network-Inputs und den Gate-Input-Pins aus
 	 *  \return die die Verbindungen zwischen den Network-Inputs und den Gate-Input-Pins
 	 */
	std::map<std::string, std::vector<Pin> > getInputConnections_() const;
 	/*! \brief Gibt die Verbindungen zwischen den Gate-Output-Pins und den Network-Outputs aus
 	 *  \return die Verbindungen zwischen den Gate-Output-Pins und den Network-Outputs
 	 */
	std::map<std::string, Pin> getOutputConnections_()const ;
 	/*! \brief Gibt die Verbindungen zwischen den Gates aus
 	 *  \return die Verbindungen zwischen den Gates
 	 */
	 std::map<Pin, std::vector<Pin> > getGate2GateConnections_() const ;
	/*! \brief Wird ein neues Network erstellt
	 *  \param a_name Name des Networks
	 * \param a_propagationDelay Gatterlaufzeit in Nanosekunden
	 */
	Network(std::string a_name, sgnl::Nanoseconds a_propagationDelay);
	/*! \brief Dem Network muss ein Input hinzugefuegt werden koennen.
	 *  \param a_name der gewuenschte Name des Inputs
	 *  \return bool
	 */
	bool addInput(const std::string &a_name);
	/*! \brief Dem Network muss ein Output hinzugefuegt werden koennen
	 *  \param a_name der gewuenschte Name des Outputs
	 *  \return bool
	 */
	bool addOutput(const std::string &a_name);
	/*! \brief Dem Network muss ein Gate hinzugefuegt werden koennen
	 *  \param a_name der gewuenschte Name des Gates
	 *  \param *a_plogic ein Zeiger auf die vom Gate zu nutzende vordefinierte GateLogic
	 *  \return bool
	 */
	bool addGate(const std::string &a_name, const GateLogic &a_plogic);
	/*! \brief Ein Network-Input muss mit einem Gate-Input-Pin verbunden werden koennen.
	 *  \param a_inputName der Name des Network-Inputs
	 *  \param a_inGateName der Name des Gates
	 *  \param a_inPinIdx der Index (die Nummer) des Gate-Input-Pins
	 *  \return bool
	 */
	bool connectInput(std::string a_inputName, std::string a_inGateName,
			int a_inPinIdx);
	/*! \brief Ein Network-Output muss mit einem Gate-Output-Pin verbunden werden koennen.
	 *  \param a_outputName der Name des Network- Outputs
	 *  \param a_outGateName der Name des Gates
	 *  \param a_outPinIdx der Index (die Nummer) des Gate-Output-Pins
	 *  \return bool
	 */
	bool connectOutput(std::string a_outputName, std::string a_outGateName,
			int a_outPinIdx);
	/*! \brief Ein Gate-Output muss mit einem Gate-Input verbunden werden koennen.
	 *  \param a_outGateName der Name des Gates, dessen Output verbunden werden soll
	 *  \param a_outPinIdx der Index (die Nummer) des Gate-Output-Pins
	 *  \param a_inGateName der Name des Gates mit dessen Input verbunden werden soll
	 *  \param a_inPinIdx der Index (die Nummer) des Gate-Input-Pins
	 *  \return bool
	 */
	bool connectGates(std::string a_outGateName, int a_outPinIdx,
			std::string a_inGateName, int a_inPinIdx);

	/*! \brief Das Network muss einen Simulationsschritt durchfuehren koennen.
	 * geschieht Folgendes:
	 * - Pruefen, ob alle Network-Inputs uebergeben wurden (sonst return). Dafuer muss ueber alle inputConnections_
	 * iteriert werden.
	 * - Die uebergebenen Network-Inputspannungen auf die verbundenen Gate-Input-Pins legen (per Gate::setInput(...)).
	 * Dafuer muss ueber alle inputConnections_ iteriert werden.
	 * - Die aktuellen Gate-Outputspannungen auf die verbundenen Gate-Input-Pins schalten (per Gate::setInput(...)).
	 * Dafuer muss ueber alle gate2GateConnections_ iteriert werden.
	 * - Alle Gates einen Simulationsschritt ausfuehren lassen (per Gate::simulationStep()). Dafuer muss ueber alle
	 * gates_ iteriert werden.
	 * \param a_inputs Map des Inputs , der Zugriff auf die Spannungen per Angabe des Network-Input-Namens
	 */

	void simulationStep(const std::map<std::string, sgnl::Volt> &a_inputs);
	/*! \brief Das Network muss auf Konsistenz geprueft werden koennen
	 * - Pruepft :ob ein Gate-Input Pin mit mehr als einem Output verbunden?
	 * - Ob kein Output oder kein einziges Gate hatte
	 * - Ob einen Network-Input mit Gate-Inputs verbindet/ einen Network-Output mit einem Gate-Output verbindet?
	 * - Ob einen Gate-Output mit Gate-Inputs verbindet?
	 * \return bool
	 */
	bool consistent() const;
	/*! \brief Das Network muss die aktuellen Spannungen an seinen Outputs zurueckgeben koennen.
	 * \return std::map der zurueckgegebenen Spannungen zu welchem Network-Output gehoert,
	 * geschieht der Zugriff auf die Spannungen per Angabe des Network-Output-Namens.
	 */
	std::map<std::string, sgnl::Volt> outputs() const;
	/*! \brief sollen alle Attribute des Networks und der enthaltenen Gates gut lesbar auf der Konsole
	 * ausgegeben werden.
	 * \return void
	 */
	void prettyPrint() const;
	/*! \brief Gibt den Name des Networks aus
	 * \return std::string Name des Networks
	 */
	std::string name() const;
	/*! \brief Gibt den Outputs-Namen des Networks aus
	 * \return std::vector<std::string> Vector-Name des outputs-Networks
	 */
	std::vector<std::string> outputNames() const;
	/*! \brief Gibt den Inputs-Namen des Networks aus
	 * \return std::vector<std::string> Vector-Name des Inputs-Networks
	 */
	std::vector<std::string> inputNames() const;
	/*! \brief Check ob Name gueltig ist
	 * \return bool true wenn Name gueltig, sonst false
	 */
	bool assert_NameValid(std::string a_name) const;
	/*! \brief Check ob Pin verbunden oder nicht
	 * \return bool true wenn schon verbunden, sonst false
	 */
	bool inPinAlreadyConnected(Pin a_inPin) const;
};

#endif /* GATE_NETWORK_H_ */
