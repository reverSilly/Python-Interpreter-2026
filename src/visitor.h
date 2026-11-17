#pragma once
#include "Python3Parser.h"
#include "Python3ParserVisitor.h"
#include "ScopeStack.h"
#include "function.h"
#ifndef PYTHON_INTERPRETER_EVALVISITOR_H
#define PYTHON_INTERPRETER_EVALVISITOR_H

#include "value.h"
#include "ControlFlow.h"
#include "Python3ParserBaseVisitor.h"

namespace reverSilly::PythonInterpreter
{
	class Visitor:public ::Python3ParserBaseVisitor 
	{
		FunctionTable table;
		ScopeStack stk;
		// TODO: override all methods of Python3ParserBaseVisitor
		using Value=reverSilly::PythonInterpreter::Value;
		using ParseTree=antlr4::tree::ParseTree;

		Value evaluate(ParseTree*);
		ControlFlow execute(ParseTree*);
		std::any visitChildren(ParseTree *t)override;
		std::any visitAtom(Python3Parser::AtomContext*)override;
		std::any visitTestlist(Python3Parser::TestlistContext*)override;
		std::any visitTest(Python3Parser::TestContext*)override;
		std::any visitOr_test(Python3Parser::Or_testContext*)override;
		std::any visitAnd_test(Python3Parser::And_testContext*)override;
		std::any visitNot_test(Python3Parser::Not_testContext*)override;
		std::any visitComparison(Python3Parser::ComparisonContext*)override;
		std::any visitArith_expr(Python3Parser::Arith_exprContext*)override;

		Value format(Python3Parser::Format_stringContext*);
	};
}


#endif//PYTHON_INTERPRETER_EVALVISITOR_H
