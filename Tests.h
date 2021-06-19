/*
 * Tests.h
 *
 *  Created on: May 10, 2021
 *      Author: Nutzer
 */

#ifndef TESTS_H_
#define TESTS_H_
#include"network/Network.h"
namespace Test
{

/*! \brief 1-zu-2-Demultiplexer (Demux) mit 2 Eingaenge und 2 Ausgaenge
 */
Network make12Demux()
{
	Network test_network("12Demux", 7);
	test_network.addGate("test_AND2_1", pgl::and2);
	test_network.addGate("test_AND2_2", pgl::and2);
	test_network.addGate("test_NOT1", pgl::not1);
	test_network.addInput("S");
	test_network.addInput("E");
	test_network.addOutput("Q0");
	test_network.addOutput("Q1");
	test_network.connectInput("S", "test_NOT1", 0);
	test_network.connectInput("E", "test_AND2_2", 0);
	test_network.connectInput("S", "test_AND2_2", 1);
	test_network.connectInput("E", "test_AND2_1", 0);
	test_network.connectGates("test_NOT1", 0, "test_AND2_1", 1);
	test_network.connectOutput("Q0", "test_AND2_1", 0);
	test_network.connectOutput("Q1", "test_AND2_2", 0);
	return test_network;
}
/*! \brief 2-zu-4-SchiebeRegister mit 2 Eingaenge und 4 Ausgaenge
 */
Network make4BitShiftRegister()
{
	Network shiftRegister("4-Bit_Shift_Register", 5);
	shiftRegister.addInput("D");
	shiftRegister.addInput("CLK");
	shiftRegister.addGate("dff0", pgl::dff);
	shiftRegister.addGate("dff1", pgl::dff);
	shiftRegister.addGate("dff2", pgl::dff);
	shiftRegister.addGate("dff3", pgl::dff);
	shiftRegister.addOutput("Q0");
	shiftRegister.addOutput("Q1");
	shiftRegister.addOutput("Q2");
	shiftRegister.addOutput("Q3");
	shiftRegister.connectInput("D", "dff0", 0);
	shiftRegister.connectInput("CLK", "dff0", 1);
	shiftRegister.connectInput("CLK", "dff1", 1);
	shiftRegister.connectInput("CLK", "dff2", 1);
	shiftRegister.connectInput("CLK", "dff3", 1);
	shiftRegister.connectOutput("Q0", "dff0", 0);
	shiftRegister.connectOutput("Q1", "dff1", 0);
	shiftRegister.connectOutput("Q2", "dff2", 0);
	shiftRegister.connectOutput("Q3", "dff3", 0);
	shiftRegister.connectGates("dff0", 0, "dff1", 0);
	shiftRegister.connectGates("dff1", 0, "dff2", 0);
	shiftRegister.connectGates("dff2", 0, "dff3", 0);
	return shiftRegister;
}
/*! \brief 12-Demux Simulation
 */
void DemuxSimulationStep()
{
	sgnl::Nanoseconds maxNanosecs = 1000;
	std::map<std::string, sgnl::Volt> inputs;
	inputs["S"] = 0;
	inputs["E"] = 0;
	std::map<std::string, sgnl::Volt> outputs;
	Network test = make12Demux();
	std::cout << "t,S,E,Q0,Q1" << std::endl;
	for (sgnl::Nanoseconds i = 0; i <= maxNanosecs; i++)
	{

		if (i % 120 < 60)
		{
			inputs["E"] = 0;
		}
		else
		{
			inputs["E"] = 5;
		}
		if (i % 500 < 250)
		{
			inputs["S"] = 0;
		}
		else
		{
			inputs["S"] = 5;
		}
		test.simulationStep(inputs);
		outputs = test.outputs();

		std::cout << i << ", " << inputs["S"] << ", " << inputs["E"] << ", ";
		std::cout << outputs["Q0"] << ", " << outputs["Q1"] << std::endl;

	}
}
/*! \brief Prueft Konsistenz von 12 Demux. Ergebnis soll OK sein (Gutfall)
 * Gepruef wird : make12Demux
 */
bool DemuxConsistent()
{
	Network test12Demux = Test::make12Demux();
	std::cout << "erwartetes Ergebnis dieses Consistent Test : OK" << std::endl;
	if (test12Demux.consistent() == true)
	{
		std::cout << "=> consistent() liefert das richtige Ergebnis ";
		return true;
	}
	else
	{
		std::cout << "=> consistent() liefert das false Ergebnis";
	}
	return false;
}
/*! \brief Gibt Struktur des Networks aus. Man kann hier selbst einer Sichtpruefung
 * unterziehen
 * return true
 */
bool DemuxPrettyPrint()
{
	Network test12Demux = Test::make12Demux();
	std::cout
			<< "erwartetes Ergebnis des PrettyPrint : gut strukturierte Ausgabe "
			<< std::endl;
	test12Demux.prettyPrint();
	return true;
}

/*Weiteres Test und Schlechtfall*/

/*! \brief Prüft ob Namen der Inputs dupliziert werden.
 */
bool InputNamesDuplicationDetected()
{
	Network test("testNetzwerk", 100);
	test.addInput("aIn");
	test.addInput("aIn");
	test.addOutput("aOut");
	if (test.assert_NameValid("aIn") == true)
	{
		std::cout << "=>duplizierte Inputsnamen wurde gefunden";
		return false;
	}
	else
	{
		std::cout << "=>duplizierte Inputsnamen wurde erkennt";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Input 2mal verbunden wurde
 * und false zurueckgibt, sonst true
 */
bool Input2TimeConnectedDetected()
{
	bool result = false;
	Network test("testNetzwerk", 100);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("bGate", pgl::not1);
	test.addGate("aGate", pgl::not1);
	result = test.connectInput("aIn", "aGate", 0);
	test.connectOutput("aOut", "bGate", 0);
	result = test.connectInput("aIn", "aGate", 0);
	if (result == true)
	{
		std::cout
				<< "=> Fehler des 2 mal verbundenen Inputs wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler des 2 mal verbundenen Inputs wurde gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Gatterlaufzeit negativ ist
 * return true
 */
bool negativePropagationsDelayDetected()
{
	Network test("negativeDelayNetwork", -100);
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Network kein Input hat
 * und false zurueckgibt, sonst true
 */
bool missingInputDetected()
{
	Network test("NoInputNetwork", 7);
	test.addOutput("aOut");
	test.addGate("aGate", pgl::not1);
	test.connectOutput("aOut", "aGate", 0);
	if (test.consistent() == true)
	{
		std::cout << "=>Fehler beim fehlenden Input wurde nicht gefunden ";
		return false;
	}
	else
	{
		std::cout << "=>Fehler beim fehlenden Input wurde gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Network kein Output hat
 * und false zurueckgibt, sonst true
 */
bool missingOutputDetected()
{
	Network test("NoOutputNetwork", 7);
	test.addInput("aIn");
	test.addGate("aGate", pgl::not1);
	if (test.consistent() == true)
	{
		std::cout << "=>Fehler beim fehlenden Output wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler beim fehlenden Output wurde gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Network kein Gatter hat
 * und false zurueckgibt, sonst true
 */
bool missingGateDetected()
{
	Network test("NoGateNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	if (test.consistent() == true)
	{
		std::cout << "Fehler beim fehlenden Gatter wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "Fehler beim fehlenden Gatter wurde gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn kein Input verbunden wurde
 * und false zurueckgibt, sonst true
 */
bool missingInputConnectionDetected()
{
	Network test("NoInputConnectionNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("aGate", pgl::not1);
	test.connectOutput("aOut", "aGate", 0);
	if (test.consistent() == true)
	{
		std::cerr
				<< "Fehler beim fehlende Input-Verbindung wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "Fehler beim fehlenden Input-Verbindung wurde gefunden";
	}
	return true;
}
/*! \brief Konsistenz-Pruefen, ob Fehler bei fehlender Output-Verbindung gefunden wurde
 */
bool missingOutputConnectionDetected()
{
	Network test("NoOutputConnectionNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("aGate", pgl::not1);
	test.connectInput("aIn", "aGate", 0);
	if (test.consistent() == true)
	{
		std::cout
				<< "=>Fehler bei fehlender Output-Verbindung wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler bei fehlender Output Verbindung wurde gefunden";
	}
	return true;
}

/*! \brief Gibt eine passende Fehlermeldung aus, wenn Input mit einem nicht
 * vorhandenen Gatter verbunden wurde
 * und false zurueckgibt, sonst true
 */
bool InputwithNonExitGateConnectionDetected()
{
	Network test("WrongGateNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("aGate", pgl::not1);
	test.connectOutput("aOut", "aGate", 0);
	if (test.connectInput("aIn", "bGate", 0) == true)
	{
		std::cout
				<< "Fehler beim Gatterverbindung mit einem nicht vorhandenen Gatter wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout
				<< "Fehler beim Gatterverbindung mit einem nicht vorhandenen Gatter wurde gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Output mit einem
 * nicht vorhandenem Gatter verbunden wurde
 * und false zurueckgibt, sonst true
 */
bool OutputwithNonExitGateConnectionDetected()
{
	Network test("WrongGateNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("bGate", pgl::not1);
	test.connectInput("aIn", "bGate", 0);
	if (test.connectOutput("aOut", "aGate", 0) == true)
	{
		std::cout
				<< "Fehler beim Output Verbindung mit einem nicht vorhandenen Gatter wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout
				<< "Fehler beim Output Verbindung mit einem nicht vorhandenen Gatter wurde gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Output 2mal verbunden wurde
 * und false zurueckgibt, sonst true
 */
bool Output2TimeConnectedDetected()
{
	bool result;
	Network test("WrongGateNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("aGate", pgl::not1);
	result = test.connectOutput("aOut", "aGate", 0);
	result = test.connectOutput("aOut", "aGate", 0);
	if (result == true)
	{
		std::cout << "=>Fehler bei Output 2 mal verbinden nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler bei Output 2 mal verbinden gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn keine Verbindung zwischen Gates erstellt wurde
 * und false zurueckgibt, sonst true
 */
bool missingGatesConnectionDetected()
{
	Network test("NoGatesConnection", 7);
	test.addInput("A");
	test.addOutput("B");
	test.addGate("aGate", pgl::not1);
	test.addGate("bGate", pgl::not1);
	test.connectInput("A", "aGate", 0);
	test.connectOutput("B", "bGate", 0);
	if (test.consistent() == true)
	{
		std::cout << "=>Fehler bei fehlender Gatterverbindungen nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler bei fehlender Gatterverbindungen nicht gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Gattern 2 mal verbunden wurde
 * und false zurueckgibt, sonst true
 */
bool Gates2TimesConnectionDetected()
{
	bool result;
	Network test("GatesConnection", 7);
	test.addInput("A");
	test.addOutput("B");
	test.addGate("aGate", pgl::not1);
	test.addGate("bGate", pgl::not1);
	test.connectInput("A", "aGate", 0);
	test.connectOutput("B", "bGate", 0);
	result = test.connectGates("aGate", 0, "bGate", 0);
	result = test.connectGates("aGate", 0, "bGate", 0);
	if (result == true)
	{
		std::cout << "=>Fehler bei Gatterverbindungen nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler bei Gatterverbindungen gefunden";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Verbindung mit einem
 * nicht vorhandenen Gatter erstellt wurde
 * und false zurueckgibt, sonst true
 */
bool GateWithNoneExitedGateConnectionDetected()
{
	bool result;
	Network test("GatesConnection", 7);
	test.addInput("A");
	test.addOutput("B");
	test.addGate("aGate", pgl::not1);
	test.addGate("bGate", pgl::not1);
	test.connectInput("A", "aGate", 0);
	test.connectOutput("B", "bGate", 0);
	result = test.connectGates("aG", 0, "bGate", 0);
	if (result == true)
	{
		std::cout << "=>Fehler bei Gatterverbindungen wurde nicht erkennt";
		return false;
	}
	else
	{
		std::cout << "=>Fehler bei Gatterverbindungen wurde erkennt";
	}
	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn es im Network leer ist
 * false zurueck gibt, sonst true
 */
bool EmptyNetworkDetected()
{
	Network test("EmptyNetwork", 7);
	if (test.consistent() == true)
	{
		std::cout << "=>Leeres Network";
		return false;
	}
	else
	{
		std::cout << "=>Keine Fehler wurde gefunden";
	}

	return true;
}
/*! \brief Gibt eine passende Fehlermeldung aus, wenn Gattern derselben Namen haben
 * und false zurueckgibt, sonst true
 */
bool SameGateNameDetected()
{
	Network test("SameGateNetwork", 7);
	test.addInput("aIn");
	test.addOutput("aOut");
	test.addGate("aGate", pgl::not1);
	test.addGate("aGate", pgl::tff);
	if (test.assert_NameValid("aGate") == true)
	{
		std::cerr << "=>Fehler bei Namen des Gattern wurde nicht gefunden";
		return false;
	}
	else
	{
		std::cout << "=>Fehler bei Namen des Gattern wurde gefunden";
	}
	return true;
}
/*! \brief Test eines Zustandautomaten, gibt sein Signalverlaeufe aus.
 */
/*! \brief Test eines Zustandautomaten, gibt sein Signalverlaeufe aus.
 */
bool ZustandsautomatenCheck()
{
	std::cout << std::endl << "Zusandautomaten" << std::endl;
	Network test_24shiftRegister = Test::make4BitShiftRegister();
	if (test_24shiftRegister.consistent() == true)
	{
		std::cout << "=> consistent() liefert das richtige Ergebnis ";
	}
	else
	{
		std::cout << "=> consistent() liefert das false Ergebnis";
		return false;
	}
	test_24shiftRegister.prettyPrint();
	sgnl::Nanoseconds maxNanosecs = 1000;
	std::map<std::string, sgnl::Volt> inputs;
	inputs["D"] = 0;
	inputs["CLK"] = 0;
	std::map<std::string, sgnl::Volt> outputs;
	std::cout << "t,CLK,D,Q0,Q1,Q2,Q3" << std::endl;
	for (sgnl::Nanoseconds i = 0; i <= maxNanosecs; i++)
	{

		if (i % 100 < 50)
		{
			inputs["CLK"] = 0;
		}
		else
		{
			inputs["CLK"] = 5;
		}
		if (i % 133 < 67)
		{
			inputs["D"] = 0;
		}
		else
		{
			inputs["D"] = 5;
		}
		test_24shiftRegister.simulationStep(inputs);
		outputs = test_24shiftRegister.outputs();
		std::cout << i << ", " << inputs["CLK"] << ", " << inputs["D"] << ", ";
		std::cout << outputs["Q0"] << ", " << outputs["Q1"] << ", "
				<< outputs["Q2"] << ", " << outputs["Q3"] << std::endl;
	}
	return true;
}

//
/*! \brief eine C0-Abdeckung von 100% für die Methode Network::consistent() erreichen
 */
//CO-Testfunktion
bool networkConsistentC0()
{
	//leeres Network blau
	Network network0("CO_Netzwerk", 7);
	if (network0.consistent() == false)
	{
		std::cout
				<< "Konsistenz Methode war erfolgreich, ein leeres Network zu erkennen"
				<< std::endl;
	}
	else
	{
		std::cout
				<< "Konsistenz Methode war nicht erfolgreich, ein leeres Network zu erkennen"
				<< std::endl;
		return false;
	}
	//Add inputs outputs and Gates
	network0.addInput("aIn");
	network0.addOutput("aOut");
	network0.addGate("aGate", pgl::not1);
	if (network0.consistent() == false)
	{
		std::cout << "Die Fehler bei fehlenden Verbindungen wurde gefunden"
				<< std::endl;
	}
	else
	{
		std::cout
				<< "Die Fehler bei fehlenden Verbindungen wurde nicht gefunden"
				<< std::endl;
		return false;
	}
	//vollständig fehlerfrei Network gelb
	Network test_24shiftRegister = Test::make4BitShiftRegister();
	if (test_24shiftRegister.consistent() == false)
	{
		std::cout << "Konsistenz Prüfung liefert falsches Ergebnis"
				<< std::endl;
		return false;
	}
	return true;
}

}

#endif /* TESTS_H_ */
