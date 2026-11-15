#include "utility.h"
#include <stdexcept>

namespace reverSilly
{
	void throw_not_implemented()
	{
		throw std::logic_error{"Not implemented."};
	}
	void throw_not_defined()
	{
		throw std::logic_error{"Not defined."};
	}
}