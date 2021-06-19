/*! \file main.cpp
 * \brief Enthaelt Programmeinstiegspunkt main()
 * \author  Nguyen, Phan Anh Thai
 * Tran, Ngoc Phuong Dung
 * \date 2020
 */

#include "gate/Gate.h"
#include <iostream>
#include "Tests.h"
#include "network/Network.h"
using namespace pgl;
using namespace sgnl;
using namespace std;
using namespace Test;
#include "fileIO/FileOutput.h"
#include "fileIO/FileInput.h"
/*! \brief Hauptprogramm
 */
int main()
{
	std::cout << "DFSM-SIM Testprogramm gestartet." << std::endl;

//	//	// Vor2
//	Test::DemuxSimulationStep();
//	Test::DemuxConsistent();
//	//1.3.2 Weiteres Schlechtfälle
//	cout << "1. Fehlende Gatter Verbindungen." << endl;
//	Test::missingGatesConnectionDetected();
//	cout << endl << "2. Ein Gatter 2 mal verbinden." << endl;
//	Test::Gates2TimesConnectionDetected();
//	cout << endl
//			<< "3. Ein Gatter mit einem nicht vorhandenen Gatter verbinden."
//			<< endl;
//	Test::GateWithNoneExitedGateConnectionDetected();
//	cout << endl << "4. Fehlende-Input-Network." << endl;
//	Test::missingInputDetected();
//	cout << endl << "5. Fehlende-Output-Network." << endl;
//	Test::missingOutputDetected();
//	cout << endl << "6. Ein Input 2 mal verbinden." << endl;
//	Test::Input2TimeConnectedDetected();
//	cout << endl << "7. Fehlende Input Verbindung." << endl;
//	Test::missingInputConnectionDetected();
//	cout << endl << "8. Output 2mal verbinden." << endl;
//	Test::Output2TimeConnectedDetected();
//	cout << endl << "9. Fehlende Output Verbindung." << endl;
//	Test::missingOutputConnectionDetected();
//	cout << endl << "10. InputName Duplication" << endl;
//	Test::InputNamesDuplicationDetected();
//	cout << endl << "11. Gatterlaufzeit negativ." << endl;
//	Test::negativePropagationsDelayDetected();
//	cout << endl << "12. gleiche Namen von Gattern." << endl;
//	Test::SameGateNameDetected();
//	cout << endl << "13. Network ist leer." << endl;
//	Test::EmptyNetworkDetected();
//	cout << endl << "14. Verbindung zwischen Input mit non-existierte Gate."
//			<< endl;
//	Test::InputwithNonExitGateConnectionDetected();
//	cout << endl << "15. Verbindung zwischen Output mit non-existierte Gate."
//			<< endl;
//	Test::OutputwithNonExitGateConnectionDetected();
//	//1.4
//	Test::ZustandsautomatenCheck();
//	// Durchführung
//	Test::networkConsistentC0();

	//Vor3
	Network testing = Test::make4BitShiftRegister();
	FileOutput::save(testing, "Vor3_Test");
	//Durch3
	FileInput fileTest;
	Network exampleTest = fileTest.load("example");
	std::cout << endl << "DFSM-SIM Testprogramm beendet." << std::endl;
	return 0;
}

/*! \mainpage Labor Software Engineering: Dokumentation
 *  Erstellt von: <br>
 * Tran, Ngoc Phuong Dung / Matrikelnummer: 759489 <br>
 * Nguyen, Phan Anh Thai / Matrikelnummer: 759861<br>
 * <br>
 * <a href="anforderungen.html"><b>Anforderungsdokument</b></a>,
 * <a href="tests.html"><b>Testdokument</b></a> und
 * <a href="textantworten.txt">
 * <b>Textantworten auf Fragen aus Vorbereitung und Durchfuehrung</b></a>
 *  \image html Cover.png
 */

/*! \page Klassendiagramm Klassendiagramm
 *  \image html Klassendiagramm.png
 *  \brief Klassendiagramm
 */
/*! \page Aktivitaetsdiagramm Aktivitaetsdiagramm
 *  \image html Aktivitaetsdiagramm.png
 *  \brief Aktivitaetsdiagramm  beschreibt den Programmablauf von main()in natuerlicher Sprache von Start bis zum Ende
 */
/*! \page D-FlipFlopSimu D-FlipFlopSimu
 *  \image html D-FlipFlop.png
 *  \brief D-Flipflop mit dem mit folgendem Szenario:
 *   D-Eingang soll mit einer Periode von 133 Nanosekunden sein.
 *   CLK-Eingang soll mit einer Periode von 100 Nanosekunden sein.
 *   D-Flipflop soll eine Gatterlaufzeit von 10 Nanosekunden haben.
 *   Ausgang = D-Eingang, falls positive Flanke an Clock-Eingang erkannt, andernfalls neuer Ausgang = alter Ausgang (Halten)
 */
/*! \page Aktivitaetsdiagramm_SimulationStep Aktivitaetsdiagramm_SimulationStep
 *  \image html Aktivitaetsdiagramm_SimulationStep.png
 *  \brief Aktivitaetsdiagramm  beschreibt den Programmablauf der Methode Network::simulationStep(std::map<std::string, sgnl::Volt> a_inputs)
 */
/*! \page Usecasediagramm Usecasediagramm
 *  \image html Usecasediagramm.png
 *  \brief Usecasediagramm  stellt das erwartete Verhalten eines Systems dar und wird deshalb dafuer eingesetzt, die Anforderungen an ein System zu spezifizieren.
 */
/*! \page 12DemuxSimu 12DemuxSimu
 *  \image html 12DemuxSimu.png
 *  \brief einen 1-zu-2-Demultiplexer (Demux). Dieser hat zwei Eingaenge: Den Dateneingang E und den Steuereingang S. Er
 *  hat zwei Ausgaenge: Q0 und Q1. Ist der Steuereingang LOW, wird E auf Q0 durchgeschaltet, ist er HIGH, wird E auf Q1
 *  durchgeschaltet.
 */
/*! \page Zustandsautomat Zustandsautomat
 *  \image html Zustandsautomat.png
 *  \brief Zustandsautomat  ine kompliziertere (weil zustandsabhaengige) Schaltung, naemlich ein synchrones
 *  4-Bit-Schieberegister, aufgebaut aus vier D-Flipflops, mit seriellem Eingang D, Takteingang CLK und parallelem
 *  Ausgang Q0..Q3
 */
/*! \page CoverageReports CoverageReports
 *  \image html CoverageReports.png
 *  \brief Testen consistent 100% Coverage Reports
 */
/*! \page ConsistentC0 ConsistentC0
 *  \image html ConsistentC0.png
 *  \brief Testenwege ConsistentC0
 */
/*! \page SequenzDiagramm SequenzDiagramm
 *  \image html SequenzDiagramm.png
 *  \brief Sequenz Diagramm  fuer FileOutput::save(...)
 */

/*! \page Zustandsdiagramm Zustandsdiagramm
 *  \image html parse-ZustandsDiagramm.png
 *  \brief Zustandsdiagramm  der Funktion Block parse(int& a_lineNumber)
 */
