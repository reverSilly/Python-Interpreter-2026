#pragma once
#include "value.h"
#include "utility.h"
#include <stdexcept>

namespace reverSilly::PythonInterpreter
{
	template<typename R>
	R do_promoted(Value const &a,Value const &b,auto F)
	{
		if(!a.is_numeric()||!b.is_numeric())
			throw std::logic_error{"not compatible types for arithmetic operation."};
		Type t=common_type(a.type(),b.type());
		if(t==Type::Integer)
			return R{F(a.to<Integer>(),b.to<Integer>())};
		if(t==Type::Float)
			return R{F(a.to<Float>(),b.to<Float>())};
		throw_not_implemented();
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