#pragma once
#include <set>

#include "Entity.h"

namespace zt::core
{
	class System
	{
	public:
		std::set<Entity> entities;
	};
}
