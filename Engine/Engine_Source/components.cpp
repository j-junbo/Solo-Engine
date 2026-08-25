#include <rttr/registration>
#include "components.h"

#define RTTR_REGISTER_CLASS_(x) rttr::registration::class_<x>(#x)
#define RTTR_REGISTER_PROPERTY_(x,y) .property(#y, & x :: y)


RTTR_REGISTRATION
{
	RTTR_REGISTER_CLASS_(NameComponent)
	RTTR_REGISTER_PROPERTY_(NameComponent, name);


	RTTR_REGISTER_CLASS_(PositionComponent)
	RTTR_REGISTER_PROPERTY_(PositionComponent, pos);

}