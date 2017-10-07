#include "Fragment.h"

bool Pirus::Fragment::operator==(const Fragment & f) const
{
	return this->level == f.level && this->type == f.type && this->value == f.value;
}
