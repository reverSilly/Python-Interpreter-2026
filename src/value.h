#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <variant>
#include <string>
#include <vector>
#include <int2048.h>

namespace reverSilly
{
	namespace PythonInterpreter
	{
		struct Value
		{
			using None=std::monostate;
			using Boolean=bool;
			using Float=double;
			using Integer=sjtu::int2048;
			using String=std::string;
			using Tuple=std::vector<Value>;
			std::variant<None,Boolean,Float,Integer,String,Tuple>data;
			template<typename T>
			bool is()const
			{
				return (bool)std::get<T>(&data);
			}
			template<typename T>
			T &as()
			{
				return std::get<T>(data);
			}
			template<typename T>
			T const&as()const
			{
				return std::get<T>(data);
			}
			template<typename T>
			T to()const
			{
				T res;
				std::visit([&res](auto const &i)
				{
					if constexpr(std::is_convertible_v<decltype(i),T>)
						res=static_cast<T>(i);
					else
					 	throw std::bad_cast{};
				},data);
				return res;
			}
			template<>
			String to<String>()const
			{
				String res;
				std::visit([&res](auto const &i)
				{
					using T=decltype(i);
					if constexpr(std::is_same_v<T,None>)
						return "None";
					else if constexpr(std::is_same_v<T,Boolean>)
						return i?"True":"False";
					else if constexpr(std::is_same_v<T,Float>)
						return std::to_string(i);
					else if constexpr(std::is_same_v<T,Integer>)
						return i.str();
					else if constexpr(std::is_same_v<T,String>)
						return i;
					else if constexpr(std::is_same_v<T,Tuple>)
						return "Tuple["+std::to_string(i.size())+"]";
					else static_assert(false,"type not implemented");
				},data);
				return res;
			}
		};
	}
}