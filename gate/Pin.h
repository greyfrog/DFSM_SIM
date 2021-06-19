/*! \file Pin.h
 *  \brief Definition der Klasse Pin
 *  \author cpr
 *  \date 2020
 */

#ifndef PIN_H_
#define PIN_H_

#include <string>


/*! \class Pin
 * \brief Fasst einen Gateinput oder -output in Form von Gatename und
 * Index (Nummer) zusammen.
 */
class Pin
{
public:
	/*! \brief Parameterloser Konstruktor. Dieser wird benoetigt, um
	 *  std::map::operator[] mit Pin als value-type verwendbar zu machen.
	 *  Es wird ein leerer Pin erzeugt. Kann verwendet werden, um eine noch
	 *  nicht vorhandene Verbindung anzuzeigen
	 */
	Pin();

	/*! \brief Konstruktor. Initialisiert den Pin mit dem uebergebenen
	 *  Gatenamen und dem Input- bzw. Output-Index (Nummer)
	 *  \param a_gateName Name des Gates
	 *  \param a_idx Index (Nummer) des Input oder Output
	 */
	Pin(std::string a_gateName, int a_idx);

	/*! \brief Gibt den Namen des Gates zurueck
	 *  \return Name des Gates
	 */
	std::string gateName() const;

	/*! \brief Gibt den Index (Nummer) des Input oder Output zurueck
	 *  \return Index (Nummer) des Input oder Output
	 */
	int idx() const;

	/*! \brief Gibt zurueck, ob es sich um einen leeren Pin handelt
	 *  \return true, falls Pin leer, false sonst
	 */
	bool empty() const;

	/*! \brief Prueft den uebergebenen Pin auf Gleichheit mit diesem
	 *  \return true, falls Gatename und Index (Nummer) gleich, false sonst
	 */
	bool operator==(const Pin& a_other) const;

	/*! \brief Wird benoetigt, um Pin als key-type einer std::map verwendbar zu
	 *  machen. Die Sortierung erfolgt erst alphabetisch nach Gatename, dann
	 *  nach Index (Nummer)
	 *  \return true, falls dieser Pin "kleiner" als der uebergebene ist, false
	 *  sonst
	 */
	bool operator<(const Pin& a_other) const;

private:
	std::string gateName_;  /**< Name des Gates */
	int idx_;				/**< Index (Nummer) des Input oder Output */
};

#endif /* PIN_H_ */
