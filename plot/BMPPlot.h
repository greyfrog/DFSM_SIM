/*! \file BMPPlot.h
 *  \brief Definition der Klasse BMPPlot
 *  \author cpr
 *  \date 2020
 */

#ifndef BMPPLOT_H_
#define BMPPLOT_H_

#include "simplebmp/simplebmp.h"
#include "../signal/Signal.h"

#include <vector>
#include <string>
#include <utility>


/*! \class BMPPlot
 *  \brief Ermoeglicht die Ausgabe von Simulationsergebnissen als Plot
 *  in einer Bitmap-Datei (.bmp)
 */
class BMPPlot
{
public:
	/*! \brief Konstruktor. Initialisiert ein Plot-Bitmap der Groesse 1280x800,
	 *  laedt das Zeichensatz-Bitmap und plottet den uebergebenen Titel
	 *  \param a_title Plot-Titel
	 */
	BMPPlot(const std::string& a_title);

	/*! \brief Plottet das uebergebene Signal unter das zuvor geplottete Signal.
	 *  \param a_signalName Name des Signals
	 *  \param a_signal Signal-Spannungswerte; Es wird davon ausgegangen, dass
	 *  die Werte im 1-Nanosekunden-Raster vorliegen
	 */
	void add(
		const std::string& a_signalName,
		const std::vector<sgnl::Volt>& a_signal);

	/*! \brief Plottet die Skalenstriche auf der Zeitachse im angegebenen Abstand
	 *  \param a_spacing Abstand zwischen den Skalenstrichen
	 */
	void setTimeTicks(sgnl::Nanoseconds a_spacing);

	/*! \brief Speichert den Plot als Bitmap-Bild. Dateiname ist ein Zeitstempel
	 *  \return true, falls das Bild erfolgreich gespeichert wurde, false sonst
	 */
	bool save() const;

private:
	/*! \brief Pixelkoordinaten
	 */
	struct Coordinate
	{
		int x_;  /**< X-Koordinate */
		int y_;  /**< Y-Koordinate */
	};

	/*! \brief Pixelfarbe
	 */
	struct Color
	{
		unsigned char r_;  	/**< Rot-Anteil, 0-255 */
		unsigned char g_;	/**< Gruen-Anteil, 0-255 */
		unsigned char b_;	/**< Blau-Anteil, 0-255 */
	};

	/*! \brief Plottet die uebergebenen Pixel
	 *  \param a_pixels Pixelkoordinaten
	 *  \param a_color Pixelfarbe
	 */
	void plotPixels(
		const std::vector<Coordinate>& a_pixels,
		Color a_color);

	/*! \brief Plottet den uebergebenen Text
	 *  \param a_where Koordinaten der oberen linken Ecke des ersten Zeichens
	 *  \param a_text Zu plottender Text
	 */
	void plotText(Coordinate a_where, const std::string& a_text);

	/*! \brief Plottet den uebergebenen Namen des Signals
	 *  \param a_signalName Name des Signals
	 */
	void plotSignalName(const std::string& a_signalName);

	/*! \brief Plottet die uebergebenen Signal-Spannungswerte
	 *  \param a_values Signal-Spannungswerte
	 */
	void plotSignalValues(const std::vector<sgnl::Volt>& a_values);

	/*! \brief Plottet das t/U-Koordinatensystem fuer die uebergebene Laenge des
	 *  Signals. Ein Pixel entspricht einer Nanosekunde
	 *  \param a_length Laenge des Signals ( = Anzahl der Werte)
	 */
	void plotAxes(sgnl::Nanoseconds a_length);

	/*! \brief Gibt die Pixelkoordinaten einer Bresenham-Linie der Dicke 1
	 *  zwischen den uebergebenen Start- und Endkoordinaten zurueck
	 *  \param a_start Startkoordinaten der Linie
	 *  \param a_end Endkoordinaten der Linie
	 *  \return Alle Pixelkoordinaten der resultierenden Linie
	 */
	std::vector<BMPPlot::Coordinate> bresenhamLine(
		Coordinate a_start,
		Coordinate a_end) const;

	SimpleBMP plot_;	/**< Plot-Bitmap */
	SimpleBMP font_;	/**< Zeichensatz-Bitmap */
	int fontSizeX_;		/**< Breite eines Zeichens */
	int fontSizeY_;		/**< Hoehe eines Zeichens */
	int nSignals_;		/**< Anzahl bisher geplotteter Signale */

};

#endif /* BMPPLOT_H_ */
