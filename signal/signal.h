/*! \file Signal.h
 *  \brief Deklarationen im namespace signal
 *  \author cpr
 *  \date 2020
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include <random>
#include <vector>

const float HIGH_THRESH_VOLT = 3.3f;  /**< Logiklevel-Schwellspannung */
const float DEADBAND_VOLT = 0.2f;  /**< Totzone fuer Logiklevel-Wechsel */
const float HIGH_LEVEL_VOLT = 5.0f;  /**< Nominalspannung fuer Logiklevel true
	(HIGH, 1) */
const float LOW_LEVEL_VOLT = 0.0f;  /**< Nominalspannung fuer Logiklevel false
	(LOW, 0) */
const float NOISE_ABS_MAX_VOLT = 0.02f;  /**< Maximalbetrag Rauschspannung */


/*! \namespace sgnl
 *  \brief Stellt Funktionen, Typdefinitionen und Objekte (Zufallsgenerator)
 *  zum Umgang mit Signalen bereit
 */
namespace sgnl
{

	typedef double Volt;		/**< Spannung / V */
	typedef int Nanoseconds;	/**< Zeit / Nanosekunden */

	extern std::random_device rd_;	/**< Seed fuer Zufallsgenerator */
	extern std::mt19937 gen_;		/**< Zufallsgenerator */
	extern std::uniform_int_distribution<> distr_;  /**< Intervall der
		Zufallswerte */

	/*! \brief Beaufschlagt den uebergebenen Spannungswert mit einer Stoerung
	 *  und gibt ihn anschliessend zurueck
	 *  \param a_voltage Spannungswert
	 *  \return Gestoerter Spannungswert
	 */
	Volt applyNoise(sgnl::Volt a_voltage);

	/*! \brief Beaufschlagt die uebergebenen Spannungswerte mit einer Stoerung
	 *  und gibt sie anschliessend zurueck
	 *  \param a_voltages Spannungswerte
	 *  \return Gestoerte Spannungswerte
	 */
	std::vector<Volt> applyNoise(std::vector<sgnl::Volt> a_voltages);

	/*! \brief Gibt die den uebergebenen Spannungswerten zuzuordnenden
	 *  Logiklevels zurueck.
	 *  \details Es sind auch die Logiklevels des vorherigen
	 *  Simulationsschritts zu uebergeben, da im Fall einer Spannung innerhalb
	 *  der Totzone kein Wechsel des vorherigen Logiklevels stattfinden soll
	 *  (wichtig fuer Flankenerkennung bei verrauschtem Signal)
	 *  \image html deadband.png
	 *  \param a_voltages Spannungswerte im aktuellen Simulationsschritt
	 *  \param a_prevLogicLevels Logiklevels im vorherigen Simulationsschritt
	 *  \return Sich ergebende Logiklevels im aktuellen Simulationsschritt
	 */
	std::vector<bool> voltageToLogical(
		std::vector<sgnl::Volt> a_voltages,
		std::vector<bool> a_prevLogicLevels);

	/*! \brief Gibt die zugehoerigen Nominalspannungswerte fuer die uebergebenen
	 *  Logiklevels zurueck. Die Konvertierung ist true->HIGH_LEVEL_VOLT,
	 *  false->LOW_LEVEL_VOLT
	 *  \param a_logicLevels Logiklevels
	 *  \return Zu den Logikleveln gehoerige Nominalspannungswerte
	 */
	std::vector<Volt> logicalToVolt(std::vector<bool> a_logicLevels);

} // namespace sgnl

#endif /* SIGNAL_H_ */
