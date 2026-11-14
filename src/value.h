#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <variant>
#include <string>
#include <vector>
#include <int2048.h>
#include <utility.h>

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
				return (bool)std::get_if<T>(&data);
			}
			bool is_numeric()const
			{
				return is<Boolean>()||is<Float>()||is<Integer>();
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
				return visit<String>
				(
					[](None){return "None";},
					[](Boolean i){return i?"True":"False";},
					[](Float i){return std::to_string(i);},
					[](Integer const &i){return i.str();},
					[](String const &i){return i;},
					[](Tuple const &i){return "Tuple["+std::to_string(i.size())+"]";}
				);
			}
			template<>
			Boolean to<Boolean>()const
			{
				return visit<Boolean>
				(
					[](None){return false;},
					[](Boolean i){return i;},
					[](Float i){return i;},
					[](Integer const &i){return i!=0;},
					[](String const &i){return !i.empty();},
					[](Tuple const &i){return !i.empty();}
				);
			}
			template<typename R>
			R visit(auto ...F)const
			{
				return std::visit<R>(overloaded{F...},data);
			}
			template<typename R>
			R visit(auto ...F)
			{
				return std::visit<R>(overloaded{F...},data);
			}
			template<typename R>
			R visit_with_default(auto ...F)const
			{
				return std::visit<R>(overloaded
				{
					F...,
					[](auto &i){throw std::logic_error{"not implemented."};}
				},data);
			}
			template<typename R>
			R visit_with_default(auto ...F)
			{
				return std::visit<R>(overloaded
				{
					F...,
					[](auto &i){throw std::logic_error{"not implemented."};}
				},data);
			}
		};
	}
}