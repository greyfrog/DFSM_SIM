/*! \file FileOutput.h
 *  \brief Definition der Klasse FileOutput
 *  \author cpr
 *  \date 2020
 */

#ifndef FILEIO_FILEOUTPUT_H_
#define FILEIO_FILEOUTPUT_H_

#include "../network/Network.h"
#include <string>
#include <vector>


/*! \class FileOutput
 * \brief Stellt Funktionalitaet zum Speichern von Networks als Save-Files
 * bereit
 */
class FileOutput
{
public:
	/*! \brief Speichert das uebergebene Network unter dem angegebenen
	 *  Dateinamen im Ordner ./Saves
	 *  \param a_network Zu speicherndes Network
	 *  \param a_fileName Dateiname, unter dem das Network im Ordner ./Saves
	 *  gespeichert werden soll, OHNE Endung .nw
	 *  \return true, falls das Speichern erfolgreich war, false sonst
	 */
	static bool save(const Network& a_network, const std::string& a_fileName);

private:
	/*! \brief Gibt den Block "name" zum Schreiben ins Save-File zurueck
	 *  \param a_name Network-Name
	 *  \return name-Block
	 */
	static std::string makeNameBlock(
		const std::string& a_name);

	/*! \brief Gibt den Block "propagationDelay" zum Schreiben ins Save-File
     *  zurueck
	 *  \param a_propagationDelay Network-Gatterlaufzeit / Nanosekunden
	 *  \return propagationDelay-Block
	 */
	static std::string makePropagationDelayBlock(
		int a_propagationDelay);

	/*! \brief Gibt den Block "inputs" zum Schreiben ins Save-File zurueck
	 *  \param a_inputNames Network-Inputnamen
	 *  \return inputs-Block
	 */
	static std::string makeInputsBlock(
		const std::vector<std::string>& a_inputNames);

	/*! \brief Gibt den Block "outputs" zum Schreiben ins Save-File zurueck
	 *  \param a_outputNames Network-Outputnamen
	 *  \return outputs-Block
	 */
	static std::string makeOutputsBlock(
		const std::vector<std::string>& a_outputNames);

    /*! \brief Gibt den Block "gates" zum Schreiben ins Save-File zurueck
     *  \param a_gates Im Network enthaltene Gates, nach Namen
     *  \return gates-Block
     */
	static std::string makeGatesBlock(
		const std::map<std::string, Gate>& a_gates);

	/*! \brief Gibt einen untergeordneten Block vom connections-Block des
     *  Save-Files zum Schreiben ins Save-File zurueck. Fuer Verbindungstyp
     *  Network-Input -> Gate-Input verwenden
	 *  \param a_inputName Network-Inputname ("from", Quelle)
     *  \param a_gateInputPins Gate-Input-Pins ("to", Ziele)
	 *  \return Untergeordneter Block vom connections-Block
	 */
	static std::string makeInputConnectionBlock(
		const std::string& a_inputName,
		const std::vector<Pin>& a_gateInputPins);

    /*! \brief Gibt einen untergeordneten Block vom connections-Block des
     *  Save-Files zum Schreiben ins Save-File zurueck. Fuer Verbindungstyp
     *  Gate-Output -> Network-Output verwenden
     *  \param a_outputName Network-Outputname ("to", Ziel)
     *  \param a_gateOutputPin Gate-Output-Pin ("from", Quelle)
     *  \return Untergeordneter Block vom connections-Block
     */
	static std::string makeOutputConnectionBlock(
		const std::string& a_outputName,
		const Pin& a_gateOutputPin);

    /*! \brief Gibt einen untergeordneten Block vom connections-Block des
     *  Save-Files zum Schreiben ins Save-File zurueck. Fuer Verbindungstyp
     *  Gate-Output -> Gate-Inputs verwenden
     *  \param a_gateOutputPin Gate-Outputname ("from", Quelle)
     *  \param a_gateInputPins Gate-Input-Pins ("to", Ziele)
     *  \return Untergeordneter Block vom connections-Block
     */
	static std::string makeGate2GateConnectionBlock(
		const Pin& a_gateOutputPin,
		const std::vector<Pin>& a_gateInputPins);

};


#endif /* FILEIO_FILEOUTPUT_H_ */
