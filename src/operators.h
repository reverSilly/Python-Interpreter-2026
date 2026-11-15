#pragma once
#include "value.h"
#include <stdexcept>

namespace reverSilly::PythonInterpreter
{
	auto do_promoted(Value const &a,Value const &b,auto F)
	{
		using Type=Value::Type;
		if(!a.is_numeric()||!b.is_numeric())
			throw std::logic_error{"not compatible types for arithmetic operation."};
		Type t=Value::common_type(a.type(),b.type());
		if(t==Type::Integer)
			return F(a.to<Value::Integer>(),b.to<Value::Integer>());
		if(t==Type::Float)
			return F(a.to<Value::Float>(),b.to<Value::Float>());
		throw std::logic_error{"not implemented."};
	}

	Value operator+(Value const&,Value const&);
	Value operator-(Value const&,Value const&);
	Value operator*(Value const&,Value const&);
	Value operator/(Value const&,Value const&);
	Value floor_division(Value const&,Value const&);
	Value operator%(Value const&,Value const&);
	Value operator+(Value const&);
	Value operator-(Value const&);

}