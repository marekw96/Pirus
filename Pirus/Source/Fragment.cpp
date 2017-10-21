#include "Fragment.h"

Pirus::Fragment::Fragment()
{}

Pirus::Fragment::Fragment(const text & value, size_t level, FRAGMENT_TYPE type)
{}

bool Pirus::Fragment::operator==(const Fragment & f) const
{
	return this->level == f.level && this->type == f.type && this->value == f.value;
}
