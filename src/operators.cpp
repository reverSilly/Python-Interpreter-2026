#include "operators.h"
#include "utility.h"

namespace reverSilly::PythonInterpreter
{
	Value operator+(Value const &a,Value const &b)
	{
		if(a.is_numeric()&&b.is_numeric())
			return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a+b;});
		if(a.is<Value::String>()&&b.is<Value::String>())
			return {a.as<Value::String>()+b.as<Value::String>()};
		throw_not_defined();
	}
}