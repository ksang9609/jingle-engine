// Object/SphereComp.cpp
#include "Object/SphereComp.h"

USphereComp::USphereComp()
	: UPrimitiveComponent()
{
	SetPrimitiveType(EPrimitiveType::Sphere);
}