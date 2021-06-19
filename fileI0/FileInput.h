/*! \file FileInput.h
 *  \brief Definition der Klasse FileInput
 *  \author cpr
 *  \date 2020
 */

#ifndef FILEIO_FILEINPUT_H_
#define FILEIO_FILEINPUT_H_

#include "../network/Network.h"
#include <string>
#include <vector>
#include "Block.h"


/*! \class FileInput
 *  \brief Stellt Funktionalitaet zum Laden von Networks aus Save-Files bereit
 */
class FileInput
{
public:
	/*! \brief Laedt das unter dem angegebenen Dateinamen gespeicherte Network
	 *  und gibt es zurueck
	 *  \param a_fileName Dateiname des zu ladenden Save-Files, OHNE Endung .nw
	 *  und OHNE Angabe des Ordners: Es wird davon ausgegangen, dass das
	 *  Save-File im Ordner Saves/ liegt
	 *  \return Das aus dem angegebenen Save-File geladene Network
	 */
	Network load(const std::string& a_fileName);

private:

    std::vector<std::string> fileLines_;  /**< Eingelesene Zeilen
        des Save-Files */

	/*! \brief Liest einen Block ab der uebergebenen Zeile ein und gibt ihn
	 *  zurueck. Die Zeile muss eine Block-Beginn-Zeile sein "{". Die Methode
	 *  ruft sich selbst auf (Rekursion), falls der Block selbst untergeordnete
	 *  Bloecke enthaelt. So entsteht eine Baumstruktur von Bloecken, die das zu
	 *  ladende Network definieren
	 *  \param a_lineNumber Zeilennummer, ab der der einzulesende Block beginnt.
	 *  Diese Zeile muss ein Block-Beginn "{" sein. Die Zeilennummer wird mit
	 *  jeder eingelesenen Zeile hochgezaehlt. Wenn die Methode zurueckkehrt,
	 *  steht darin die Zeilennummer des zum Block gehoerenden Endes "}"
	 *  \return Der fertig eingelesene Block
	 */
	Block parse(int& a_lineNumber) const;

	/*! \brief Moegliche Events beim Einlesen des Save-Files in Blocks
	 */
	enum class ReadEvent
	{
		OPENER = 0,	 /**< "{"-Zeile angetroffen (Block-Beginn)              */
		CLOSER = 1,	 /**< "}"-Zeile angetroffen (Block-Ende)                */
		NAME = 2,	 /**< Zeile angetroffen, die auf ":" endet (Block-Name) */
		VALUE = 3	 /**< Zeile angetroffen, die einen Block-Wert enthaelt  */
	};

	/*! \brief Moegliche Zustaende beim Einlesen des Save-Files in Blocks
	 */
	enum class ReadState
	{
		EXPECTING_OPENER = 0,		   /**< Erwarte "{"-Zeile (Block-Beginn)  */
		EXPECTING_VALUE_OR_NAME = 1,   /**< Erwarte Wert oder Block-Name      */
		EXPECTING_VALUE_OR_CLOSER = 2, /**< Erwarte Wert oder Block-Ende "}"  */
		EXPECTING_NAME_OR_CLOSER = 3   /**< Erwarte Block-Name oder -Ende "}" */
	};

	/*! \brief Gibt eine Parser-Fehlermeldung aus, die die uebergebenen
	 *  Parameter aktueller Zustand, erhaltenes Event und Zeilennummer enthaelt
	 *  \param a_state Parserzustand, in dem der Fehler aufgetreten ist
	 *  \param a_event Erhaltenes Event, bei dem der Fehler aufgetreten ist
	 *  \param a_lineNumber Zeilennummer, in der der Fehler aufgetreten ist
	 */
	void error(ReadState a_state, ReadEvent a_event, int a_lineNumber) const;

	/*! \brief Interpretiert den Inhalt der uebergebenen Zeile als ReadEvent
	 *  und gibt es zurueck
	 *  \param a_line Die als Event zu interpretierende Zeile
	 *  \return Das ReadEvent entsprechend dem Inhalt der uebergebenen Zeile
	 */
	ReadEvent getEvent(const std::string& a_line) const;

	/*! \brief Gibt den Block-Namen zurueck, der in der uebergebenen Zeile steht
	 *  (Zeile ohne den nachgestellten Doppelpunkt ":")
	 *  \param a_line Die Save-File-Zeile, die den Block-Namen enthaelt
	 *  \return Der Block-Name (ohne nachgestellten Doppelpunkt ":")
	 */
	std::string getBlockName(const std::string& a_line) const;

	/*! \brief Konvertiert die uebergebene Block-Baumstruktur in das darin
	 *  hinterlegte Network
	 *  \param a_networkBlock Die kompletten Network-Daten (Verbindungen, Gates
	 *  etc.) als Block-Baumstruktur
	 *  \return Das Network
	 */
	Network blockToNetwork(const Block& a_networkBlock) const;

	/*! \brief Konvertiert den Inhalt des Save-Files mit dem angegebenen Namen
	 *  in einen Vektor der einzelnen Zeilen. Leerzeilen werden uebersprungen,
	 *  vor- und nachgestellte Leerzeichen entfernt
	 *  \param a_fileName Dateiname des zu ladenden Save-Files, OHNE Endung .nw
	 *  und OHNE Angabe des Ordners: Es wird davon ausgegangen, dass das
	 *  Save-File im Ordner Saves/ liegt
	 *  \return Der Inhalt des Save-Files als Vektor der einzelnen Zeilen, ohne
	 *  Leerzeilen und ohne vor- und nachgestellte Leerzeichen
	 */
	bool fileContentsToLines(const std::string& a_fileName);

	/*! \brief Entfernt vor- und nachgestellte Leerzeichen aus der uebergebenen
	 *  Zeile
	 *  \param a_line Zeile, aus der die vor- und nachgestellten Leerzeichen
	 *  entfernt werden sollen
	 */
	void trimLine(std::string& a_line) const;

	/*! \brief Interpretiert den uebergebenen string als positive Ganzzahl
	 *  \param a_value string, der als positive Ganzzahl interpretiert werden soll
	 *  \return Die positive Ganzzahl entsprechend dem uebergebenen string oder
	 *  -1, falls der uebergebene string nicht als positive Ganzzahl
	 *  interpretiert werden kann. In diesem Fall wird auch eine Fehlermeldung
	 *  ausgegeben
	 */
	int toPositiveInteger(const std::string& a_value) const;

	/*! \brief Interpretiert den uebergebenen string als GateLogic
	 *  \param a_str string, der als GateLogic interpretiert werden soll
	 *  \return Diejenige GateLogic, die dem uebergebenen string entspricht oder
	 *  die BUF1-GateLogic, falls der uebergebene string nicht als GateLogic
	 *  interpretiert werden kann
	 */
	const GateLogic& logicFromString(const std::string& a_str) const;
};


#endif /* FILEIO_FILEINPUT_H_ */
