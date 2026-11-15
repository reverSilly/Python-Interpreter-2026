#include "operators.h"
#include "int2048.h"
#include "utility.h"
#include "value.h"
#include <cmath>

namespace reverSilly::PythonInterpreter
{
	Value operator+(Value const &a,Value const &b)
	{
		if(a.is<Value::String>()&&b.is<Value::String>())
			return {a.as<Value::String>()+b.as<Value::String>()};
		return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a+b;});
	}
	Value operator-(Value const &a,Value const &b)
	{
		return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a-b;});
	}
	std::string repeat(std::string s,int2048::Integer const &n)
	{
		std::string res;
		for(u32 i=0,len=n.len();i<len;++i,s+=s)
			if(n.get_bit(i))
				res+=s;
		return res;
	}
	bool bool_or_int(Value::Type t)
	{
		return t==Value::Type::Boolean||t==Value::Type::Integer;
	};
	Value operator*(Value const &a,Value const &b)
	{
		using String=Value::String;
		using Integer=Value::Integer;
		using Type=Value::Type;
		auto str=[](Value const &a,Value const &b)->Value
		{
			Integer bb=b.to<Integer>();
			if(bb<0)
				return {String("")};
			return {repeat(a.as<String>(),bb.data())};
		};
		if(a.type()==Type::String&&bool_or_int(b.type()))
			return str(a,b);
		if(bool_or_int(a.type())&&b.type()==Type::String)
			return str(b,a);
		return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a*b;});
	}
	Value operator/(Value const &a,Value const &b)
	{
		return {a.to<Value::Float>()/b.to<Value::Float>()};
	}
	Value floor_division(const Value &a,const Value &b)
	{
		using Integer=Value::Integer;
		using Float=Value::Float;
		return do_promoted<Value>(a,b,overloaded{[](Integer const &a,Integer const &b)
		{
			return a/b;
		},[](Float a,Float b)
		{
			return std::floor(a/b);
		}});
	}
	Value operator%(const Value &a,const Value &b)
	{
		using Integer=Value::Integer;
		using Float=Value::Float;
		auto fmod=[](Value::Float a,Value::Float b)
		{
			Float r=std::fmod(a, b);
			if (r!=0&&((r<0)!=(b<0)))
				r+=b;
			return r;
		};
		return do_promoted<Value>(a,b,overloaded{[](Integer const &a,Integer const &b)
		{
			return a%b;
		},[fmod](Float a,Float b)
		{
			return fmod(a,b);
		}});
	}
	Value operator+(const Value &a)
	{
		if(a.type()==Value::Type::Boolean)
			return {a.to<Value::Integer>()};
		if(a.is_numeric())
			return a;
		throw_not_defined();
	}
	Value operator-(const Value &a)
	{
		if(a.type()==Value::Type::Boolean)
			return {-a.to<Value::Integer>()};
		return a.visit_with_default<Value>
		(
			[](Value::Integer const &a)->Value{return {-a};},
			[](Value::Float a)->Value{return {-a};}
		);
	}
}