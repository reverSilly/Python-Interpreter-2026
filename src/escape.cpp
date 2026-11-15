#include "escape.h"
#include <cctype>
#include <charconv>
#include <string_view>

namespace reverSilly::PythonInterpreter
{
	std::string escape(std::string_view str)//quoted string literal
	{
		bool raw=false;
		auto front_is=[](std::string_view str,std::string_view sv)
		{
			if(str.empty())
				return false;
			return sv.find(str.front())!=std::string_view::npos;
		};
		if(front_is(str,"rRuU"))
		{
			if(front_is(str,"rR"))
				raw=true;
			str.remove_prefix(1);
		}
		if(str.starts_with("'''")||str.starts_with("\"\"\""))
			str.remove_prefix(3);
		else if(front_is(str,"\'\""))
			str.remove_prefix(1);
		if(str.ends_with("'''")||str.ends_with("\"\"\""))
			str.remove_suffix(3);
		else if(!str.empty()&&(str.back()=='\''||str.back()=='"'))
			str.remove_suffix(1);
		if(raw)
			return std::string(str);
		std::string res;
		for(auto it=str.begin();it!=str.end();++it)
		{
			if(*it!='\\')
				res+=*it;
			else if(char app=0;++it!=str.end())
			{
				switch(*it)
				{
					case 'a':app='\a';break;
					case 'b':app='\b';break;
					case 'f':app='\f';break;
					case 'n':app='\n';break;
					case 'r':app='\r';break;
					case 't':app='\t';break;
					case 'v':app='\v';break;
					case 'x':
						it=std::from_chars(it,str.end(),app,16).ptr;
					break;
					default:
					if('0'<=*it&&*it<'8')
						it=std::from_chars(it,std::min(it+3,str.end()),app,8).ptr;
					else app=*it;
				}
				res+=app;
				if(it==str.end())
					break;
			}
			else break;
		}
		return res;
	}
}