#include "escape.h"
#include <cctype>
#include <charconv>
#include <string_view>
#include <system_error>

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
			else if(unsigned int ch=0;++it!=str.end())
			{
				switch(*it)
				{
					case 'a':ch='\a';break;
					case 'b':ch='\b';break;
					case 'f':ch='\f';break;
					case 'n':ch='\n';break;
					case 'r':ch='\r';break;
					case 't':ch='\t';break;
					case 'v':ch='\v';break;
					case 'x':
					{
						auto[jt,ec]=std::from_chars(it+1,str.end(),ch,16);
						if(ec!=std::errc{})
							ch='x';
						it=jt;
					}
					break;
					default:
					if('0'<=*it&&*it<'8')
						it=std::from_chars(it,std::min(it+3,str.end()),ch,8).ptr;
					else ch=*it;
				}
				res+=ch;
				if(it==str.end())
					break;
			}
			else break;
		}
		return res;
	}
}