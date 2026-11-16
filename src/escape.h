#pragma once
#include <string>
#include <string_view>

namespace reverSilly::PythonInterpreter
{
	std::string escape(std::string_view str);
}