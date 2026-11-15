#include "visitor.h"
#include <any>

namespace reverSilly::PythonInterpreter
{
	Value Visitor::evaluate(ParseTree *p)
	{
		return std::any_cast<Value>(visit(p));
	}
	ControlFlow Visitor::execute(ParseTree *p)
	{
		return std::any_cast<ControlFlow>(visit(p));
	}
}