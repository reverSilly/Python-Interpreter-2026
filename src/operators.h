#include "value.h"
#include <compare>

namespace reverSilly::Python3Interpreter
{
	Value do_promoted(Value const&,Value const&,auto F);
	Value operator+(Value const&,Value const&);
	Value operator-(Value const&,Value const&);
	Value operator*(Value const&,Value const&);
	Value operator/(Value const&,Value const&);
	Value floor_division(Value const&,Value const&);
	Value operator%(Value const&,Value const&);
	Value operator+(Value const&);
	Value operator-(Value const&);

	std::strong_ordering operator<=>(Value const&,Value const&);
	bool operator==(Value const&,Value const&);
	bool operator!=(Value const&,Value const&)=default;
	bool operator<(Value const&,Value const&)=default;
	bool operator<=(Value const&,Value const&)=default;
	bool operator>(Value const&,Value const&)=default;
	bool operator>=(Value const&,Value const&)=default;
}