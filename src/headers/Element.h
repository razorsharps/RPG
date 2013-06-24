#pragma once

#include "Visitor.h"
class Element
{
public :
	virtual void accept(class Visitor &v) = 0;
};