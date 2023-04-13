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
		void init()
		{
			// Create pointers to each manager
			componentManager = std::make_unique<ComponentManager>();
			entityManager = std::make_unique<EntityManager>();
			systemManager = std::make_unique<SystemManager>();
		}

		[[nodiscard]] Entity createEntity() const
		{
			return entityManager->createEntity();
		}

		void destroyEntity(const Entity entity) const
		{
			entityManager->destroyEntity(entity);

			componentManager->entityDestroyed(entity);

			systemManager->entityDestroyed(entity);
		}

		// Component methods
		template<typename T>
		void registerComponent() const
		{
			componentManager->registerComponent<T>();
		}

		template<typename T>
		void addComponent(Entity entity, T component)
		{
			componentManager->addComponent<T>(entity, component);

			auto signature = entityManager->getSignature(entity);
			signature.set(componentManager->getComponentType<T>(), true);
			entityManager->setSignature(entity, signature);

			systemManager->entitySignatureChanged(entity, signature);
		}


		template<typename T>
		void removeComponent(Entity entity) const
		{
			componentManager->removeComponent<T>(entity);

			auto signature = entityManager->getSignature(entity);
			signature.set(componentManager->getComponentType<T>(), false);
			entityManager->setSignature(entity, signature);

			systemManager->entitySignatureChanged(entity, signature);
		}


		template <typename T>
		T& getComponent(const Entity entity)
		{
			return componentManager->getComponent<T>(entity);
		}

		template <typename T>
		[[nodiscard]] ComponentType getComponentType() const
		{
			return componentManager->getComponentType<T>();
		}

		template <typename T>
		std::shared_ptr<T> registerSystem()
		{
			return systemManager->registerSystem<T>();
		}

		template <typename T>
		void setSystemSignature(const Signature signature) const
		{
			systemManager->setSignature<T>(signature);
		}

	private:
		std::unique_ptr<ComponentManager> componentManager;
		std::unique_ptr<EntityManager> entityManager;
		std::unique_ptr<SystemManager> systemManager;
	};
}
