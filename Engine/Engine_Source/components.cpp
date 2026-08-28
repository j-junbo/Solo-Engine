#include <rttr/registration>
#include "components.h"

#define RTTR_REGISTER_CLASS_(x) rttr::registration::class_<x>(#x)
#define RTTR_REGISTER_PROPERTY_(x,y) .property(#y, & x :: y)

RTTR_REGISTRATION
{
	RTTR_REGISTER_CLASS_(NameComponent)
	RTTR_REGISTER_PROPERTY_(NameComponent, name)
	;

	RTTR_REGISTER_CLASS_(TransformComponent)
	RTTR_REGISTER_PROPERTY_(TransformComponent, pos)
	RTTR_REGISTER_PROPERTY_(TransformComponent, scale)
	RTTR_REGISTER_PROPERTY_(TransformComponent, rotation)
	;

	RTTR_REGISTER_CLASS_(CameraComponent)
	RTTR_REGISTER_PROPERTY_(CameraComponent, target)
	RTTR_REGISTER_PROPERTY_(CameraComponent, fieldOfView)
	RTTR_REGISTER_PROPERTY_(CameraComponent, near)
	RTTR_REGISTER_PROPERTY_(CameraComponent, far)
	;

	RTTR_REGISTER_CLASS_(LightComponent)
	RTTR_REGISTER_PROPERTY_(LightComponent, ambient)
	RTTR_REGISTER_PROPERTY_(LightComponent, diffuse)
	RTTR_REGISTER_PROPERTY_(LightComponent, specular)
	;

	RTTR_REGISTER_CLASS_(TextureComponent)
	RTTR_REGISTER_PROPERTY_(TextureComponent, uv)
	;

}