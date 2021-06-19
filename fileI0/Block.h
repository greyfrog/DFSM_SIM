/*! \file Block.h
 *  \brief Definition der Klasse Block
 *  \author cpr
 *  \date 2020
 */

#ifndef FILEIO_BLOCK_H_
#define FILEIO_BLOCK_H_

#include <string>
#include <vector>
#include <map>


/*! \class Block
 * \brief Hinterlegt den Inhalt eines Save-Files so, dass ein strukturierter
 * Zugriff auf die einzelnen Save-File-Bloecke und ihre Werte ermoeglicht wird
 */
class Block
{
public:
	/*! \brief Fuegt dem Block einen Wert hinzu. Das ist nur erlaubt, wenn der
	 *  Block nicht schon untergeordnete Bloecke enthaelt
	 *  \param a_value Der hinzuzufuegende Wert
	 */
	void addValue(const std::string& a_value);

	/*! \brief Gibt die Anzahl der vom Block gespeicherten Werte zurueck
	 *  \return Anzahl der vom Block gespeicherten Werte
	 */
	int nValues() const;

	/*! \brief Gibt den Wert mit dem angegebenen Index (Nummer) zurueck
	 *  \param a_idx Index (Nummer) des Werts
	 *  \return Wert mit dem angegebenen Index (Nummer)
	 */
	std::string value(int a_idx) const;

	/*! \brief Fuegt dem Block einen untergeordneten Block mit dem angegebenen
	 *  Namen hinzu. Das ist nur erlaubt, wenn der Block nicht schon Werte
	 *  enthaelt
	 *  \param a_name Name des hinzuzufuegenden untergeordneten Blocks
	 *  \param a_block Der hinzuzufuegende untergeordnete Block
	 */
	void addChild(const std::string& a_name, const Block& a_block);

	/*! \brief Gibt die Anzahl der vom Block gespeicherten untergeordneten
	 *  Bloecke zurueck
	 *  \return Anzahl der vom Block gespeicherten untergeordneten Bloecke
	 */
	int nChildren() const;

	/*! \brief Gibt zurueck, ob der Block einen untergeordneten Block mit dem
	 *  angegebenen Namen enthaelt
	 *  \param a_name Name des untergeordneten Blocks
	 *  \return true, falls der Block einen untergeordneten Block mit dem
	 *  angegebenen Namen enthält, false sonst
	 */
	bool hasChild(const std::string& a_name) const;

	/*! \brief Gibt den untergeordneten Block mit dem angegebenen Namen zurueck
	 *  \param a_name Name des untergeordneten Blocks
	 *  \return Der untergeordnete Block mit dem angegebenen Namen
	 */
	Block child(const std::string& a_name) const;

	/*! \brief Gibt den untergeordneten Block mit dem angegebenen Index (Nummer)
	 *  zurueck
	 *  \param a_idx Index (Nummer) des untergeordneten Blocks
	 *  \return Der untergeordnete Block mit dem Index (Nummer)
	 */
	Block child(int a_idx) const;

	/*! \brief Gibt den Inhalt des Blocks in gut lesbarer Form auf der Konsole
	 *  aus
	 */
	void prettyPrint() const;

private:
	std::vector<std::string> values_;  			/**< Werte */
	std::map<std::string, Block> children_;		/**< Untergeordnete Bloecke */

	/*! \brief Gibt den Inhalt des Blocks in gut lesbarer Form mit der
	 *  angegebenen Einrueckungstiefe auf der Konsole aus
	 *  \param a_indent Einrueckungstiefe / Leerzeichen
	 */
	void printIndented(int a_indent) const;
};


#endif /* FILEIO_BLOCK_H_ */
