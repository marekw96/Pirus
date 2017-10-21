#include "Fragment.h"

Pirus::Fragment::Fragment() :
	value{},
	level{0},
	type{}
{}

Pirus::Fragment::Fragment(const text & value, size_t level, FRAGMENT_TYPE type) :
	value(value),
	level(level),
	type(type)
{}

bool Pirus::Fragment::operator==(const Fragment & f) const
{
	return this->level == f.level && this->type == f.type && this->value == f.value;
}
