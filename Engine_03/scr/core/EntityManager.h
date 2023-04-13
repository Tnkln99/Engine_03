#pragma once
#include <array>
#include <queue>

#include "Entity.h"
#include "Signature.h"

namespace zt::core
{
	class EntityManager
	{
	public:
		EntityManager();

		Entity createEntity();

		void destroyEntity(Entity entity);

		void setSignature(Entity entity, Signature signature);

		[[nodiscard]] Signature getSignature(Entity entity) const;

	private:
		// Queue of unused entity IDs
		std::queue<Entity> availableEntities{};

		// Array of signatures where the index corresponds to the entity ID
		std::array<Signature, MAX_ENTITIES> signatures{};

		// Total living entities - used to keep limits on how many exist
		uint32_t livingEntityCount{};
	};
}
