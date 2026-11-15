#include "value.h"
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
}