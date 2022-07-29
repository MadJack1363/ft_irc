#include "ft.hpp"
#include <sstream>

/**
 * @brief	Convert an int to a string.
 * 
 * @param	nb The int to convert.
 * 
 * @return	The string representation of the int.
 */
std::string	ft::toString(int const nb)
{
	std::stringstream	ss;

	ss << nb;
	return ss.str();
}
