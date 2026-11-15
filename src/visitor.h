#pragma once
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "value.h"
#include "ControlFlow.h"
#include "Python3ParserBaseVisitor.h"

namespace reverSilly::PythonInterpreter
{
	class Visitor:public ::Python3ParserBaseVisitor 
	{
		// TODO: override all methods of Python3ParserBaseVisitor
		using Value=reverSilly::PythonInterpreter::Value;
		using ParseTree=antlr4::tree::ParseTree;
		Value evaluate(ParseTree*);
		ControlFlow execute(ParseTree*);
	};
}


#endif//PYTHON_INTERPRETER_EVALVISITOR_H
