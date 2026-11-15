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
			enum class Type
			{
				None=0,Boolean,Float,Integer,String,Tuple
			};
			
			Type type()const
			{
				return static_cast<Type>(data.index());
			}
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
				return std::visit([](auto const &i)->T
				{
					if constexpr(std::is_convertible_v<decltype(i),T>)
						return static_cast<T>(i);
					else
					 	throw std::bad_cast{};
				},data);
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
			template<>
			Float to<Float>()const
			{
				return visit_with_default<Float>
				(
					[](Boolean a){return a;},
					[](Integer const &a){return a.to<Float>();},
					[](Float a){return a;}
				);
			}
			template<>
			Integer to<Integer>()const
			{
				return visit_with_default<Integer>
				(
					[](Boolean a){return a;},
					[](Integer const &a){return a;}
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
				return visit<R>
				(
					F...,
					[](auto &i)->R{throw std::logic_error{"not implemented."};}
				);
			}
			template<typename R>
			R visit_with_default(auto ...F)
			{
				return visit<R>
				(
					F...,
					[](auto &i)->R{throw std::logic_error{"not implemented."};}
				);
			}
		};
	}
}