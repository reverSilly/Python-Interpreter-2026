#pragma once
namespace reverSilly
{
	template<class...T>
	struct overloaded:T...{ using T::operator()...; };
	[[noreturn]]void throw_not_implemented();
	[[noreturn]]void throw_not_defined();
}