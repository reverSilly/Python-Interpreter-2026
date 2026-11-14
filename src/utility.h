
namespace reverSilly
{
	template<class...T>
	struct overloaded:T...{ using T::operator()...; };
}