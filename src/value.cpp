#include "value.h"
#include "operators.h"
#include "utility.h"
#include <compare>
#include <typeinfo>

namespace reverSilly::PythonInterpreter 
{
	Value::Type Value::common_type(Type a, Type b)
	{
		if(a==Type::Float||b==Type::Float)
			return Type::Float;
		return Type::Integer;
	}
	bool Value::is_numeric(Type a)
	{
		return a==Type::Boolean||a==Type::Float||a==Type::Integer;
	}
	Value Value::to(Type t)const
	{
		switch(t)
		{
			case Type::None:return {to<None>()};
			case Type::Boolean:return {to<Boolean>()};
			case Type::Float:return {to<Float>()};
			case Type::Integer:return {to<Integer>()};
			case Type::String:return {to<String>()};
			case Type::Tuple:return {to<Tuple>()};
		}
		throw std::bad_cast{};
	}

	std::partial_ordering operator<=>(Value const &a,Value const &b)
	{
		if(a.is_numeric()&&b.is_numeric())
			return do_promoted<std::partial_ordering>(a,b,[](auto const &a,auto const &b){return a<=>b;});
		if(a.type()==b.type())
		{
			if(a.is<Value::None>())
				throw_not_defined();
			return a<=>b;
		}
		throw_not_defined();
	}
	bool operator==(Value const &a,Value const &b)
	{
		if(a.is_numeric()&&b.is_numeric())
			return do_promoted<bool>(a,b,[](auto const &a,auto const &b){return a==b;});
		return a==b;
	}
}