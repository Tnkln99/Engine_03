#pragma once
#include <memory>

#include "ComponentManager.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Signature.h"
#include "SystemManager.h"

namespace zt::core
{
	class Coordinator
	{
	public:
		void init();

		// Entity methods
		[[nodiscard]] Entity createEntity() const;

		void destroyEntity(Entity entity) const;

		// Component methods
		template<typename T>
		void registerComponent() const;

		template<typename T>
		void addComponent(Entity entity, T component);

		template<typename T>
		void removeComponent(Entity entity) const;

		template<typename T>
		T& getComponent(Entity entity);

		template<typename T>
		[[nodiscard]] ComponentType getComponentType() const;


		// System methods
		template<typename T>
		std::shared_ptr<T> registerSystem();

		template<typename T>
		void SetSystemSignature(Signature signature) const;
	private:
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;
	};
}
