#include "operators.h"
#include "int2048.h"
#include "utility.h"
#include "value.h"
#include <cmath>
#include <format>
#include <stdexcept>
#include <type_traits>

namespace reverSilly::PythonInterpreter
{
	Value operator+(Value const &a,Value const &b)
	{
		if(a.is<String>()&&b.is<String>())
			return {a.as<String>()+b.as<String>()};
		return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a+b;},"+");
	}
	Value operator-(Value const &a,Value const &b)
	{
		return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a-b;},"-");
	}
	std::string repeat(std::string s,int2048::Integer const &n)
	{
		std::string res;
		for(u32 i=0,len=n.len();i<len;++i,s+=s)
			if(n.get_bit(i))
				res+=s;
		return res;
	}
	bool bool_or_int(Type t)
	{
		return t==Type::Boolean||t==Type::Integer;
	};
	Value operator*(Value const &a,Value const &b)
	{
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
		return do_promoted<Value>(a,b,[](auto const &a,auto const &b){return a*b;},"*");
	}
	Float check_fzero(Float f)
	{
		if(f==0)
			throw ProgramError{"divided by 0"};
		return f;
	}
	Value operator/(Value const &a,Value const &b)
	{
		return {a.to<Float>()/check_fzero(b.to<Float>())};
	}
	auto rethrow(auto F)
	{
		try{ return F(); }
		catch(std::invalid_argument &e)
		{ throw ProgramError(e.what()); }
	}
	Value floor_division(const Value &a,const Value &b)
	{
		return do_promoted<Value>(a,b,overloaded{[](Integer const &a,Integer const &b)
		{
			return rethrow([&]()
			{
				return a/b;
			});
		},[](Float a,Float b)
		{
			return std::floor(a/check_fzero(b));
		}},"//");
	}
	Value operator%(const Value &a,const Value &b)
	{
		auto fmod=[](Float a,Float b)
		{
			check_fzero(b);
			Float r=std::fmod(a, b);
			if (r!=0&&((r<0)!=(b<0)))
				r+=b;
			return r;
		};
		return do_promoted<Value>(a,b,overloaded{[](Integer const &a,Integer const &b)
		{
			return rethrow([&]()
			{
				return a%b;
			});
		},[fmod](Float a,Float b)
		{
			return fmod(a,b);
		}},"%");
	}
	Value operator+(const Value &a)
	{
		if(a.type()==Type::Boolean)
			return {a.to<Integer>()};
		if(a.is_numeric())
			return a;
		throw_not_defined(std::format("unary operator+ not defined for type: {}",a.type_name()));
	}
	Value operator-(const Value &a)
	{
		if(a.type()==Type::Boolean)
			return {-a.to<Integer>()};
		return a.visit<Value>
		(
			[](Integer const &a)->Value{return {-a};},
			[](Float a)->Value{return {-a};},
			[](auto &&v)->Value
			{
				throw ProgramError{std::format
				(
					"unary operator- not defined for type: {}",
					type_name<std::remove_cvref_t<decltype(v)>>()
				)};
			}
		);
	}
}