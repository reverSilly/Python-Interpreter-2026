#include "visitor.h"
#include "Python3Parser.h"
#include "Python3ParserVisitor.h"
#include "parse_number.h"
#include "tree/ParseTreeType.h"
#include "tree/TerminalNode.h"
#include "utility.h"
#include "value.h"
#include "escape.h"
#include <any>
#include <format>

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
	std::any Visitor::visitChildren(ParseTree *t)
	{
		throw_not_implemented(std::format
		(
			"the visitor is not implemented on '{}'",
			typeid(*t).name()
		));
	}
	std::any Visitor::visitAtom(Python3Parser::AtomContext *ctx)
	{
		if(ctx->NUMBER())
			return parse_number(ctx->NUMBER()->getText());
		if(ctx->NAME())
		{
			return rethrow([&]
			{
				return stk.find(ctx->NAME()->getText());
			});
		}
		if(!ctx->STRING().empty())
		{
			String res;
			for(auto &i:ctx->STRING())
				res+=parse_string(i->getText(),default_escape);
			return Value{res};
		}
		if(ctx->NONE())
			return Value{};
		if(ctx->TRUE())
			return Value{Boolean{true}};
		if(ctx->FALSE())
			return Value{Boolean{false}};
		if(ctx->test())
			return evaluate(ctx->test());
		if(ctx->format_string())
			return format(ctx->format_string());
		throw_not_implemented("not implemented");
	}
	Value Visitor::format(Python3Parser::Format_stringContext *ctx)
	{
		auto f_escape=[](std::string_view &str)->std::string
		{
			if(char ch=str.front();str.starts_with("{{")||str.starts_with("}}"))
			{
				str.remove_prefix(2);
				return {ch};
			}
			return default_escape(str);
		};
		String res;
		for(auto child:ctx->children)
		{
			if(auto *tl=dynamic_cast<Python3Parser::TestlistContext*>(child))
				res+=evaluate(tl).to<String>();
			else if(auto term=dynamic_cast<antlr4::tree::TerminalNode*>(child))
			{
				if(term->getSymbol()->getType()==Python3Parser::FORMAT_STRING_LITERAL)
					res+=escape(term->getText(),f_escape);
			}
		}
		return {res};
	}
	std::any Visitor::visitTestlist(Python3Parser::TestlistContext *ctx)
	{
		Tuple res;
		for(auto i:ctx->test())
			res.emplace_back(evaluate(i));
		if(res.size()==1)
			return res.front();
		return Value{res};
	}
}