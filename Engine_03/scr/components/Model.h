#pragma once
#include "../graphics/Model.h"
namespace zt::component
{
	struct Model
	{
		std::shared_ptr<graphics::Model> model{};
	};
}
