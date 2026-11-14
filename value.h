#include<variant>
#include<string>
#include<vector>
#include<int2048.h>

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
		};
	}
}