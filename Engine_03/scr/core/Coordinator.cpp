#include "Coordinator.h"

namespace zt::core
{
	void Coordinator::init()
	{
		// Create pointers to each manager
		componentManager = std::make_unique<ComponentManager>();
		entityManager = std::make_unique<EntityManager>();
		systemManager = std::make_unique<SystemManager>();
	}

	Entity Coordinator::createEntity() const
	{
		return entityManager->createEntity();
	}

	void Coordinator::destroyEntity(const Entity entity) const
	{
		entityManager->destroyEntity(entity);

		componentManager->entityDestroyed(entity);

		systemManager->entityDestroyed(entity);
	}

	template <typename T>
	void Coordinator::registerComponent() const
	{
		componentManager->registerComponent<T>();
	}

	template <typename T>
	void Coordinator::addComponent(Entity entity, T component)
	{
		componentManager->addComponent<T>(entity, component);

		auto signature = entityManager->getSignature(entity);
		signature.set(componentManager->getComponentType<T>(), true);
		entityManager->setSignature(entity, signature);

		systemManager->entitySignatureChanged(entity, signature);
	}

	template <typename T>
	void Coordinator::removeComponent(const Entity entity) const
	{
		componentManager->removeComponent<T>(entity);

		auto signature = entityManager->getSignature(entity);
		signature.set(componentManager->getComponentType<T>(), false);
		entityManager->setSignature(entity, signature);

		systemManager->entitySignatureChanged(entity, signature);
	}

	template <typename T>
	T& Coordinator::getComponent(const Entity entity)
	{
		return componentManager->getComponent<T>(entity);
	}

	template <typename T>
	ComponentType Coordinator::getComponentType() const
	{
		return componentManager->getComponentType<T>();
	}

	template <typename T>
	std::shared_ptr<T> Coordinator::registerSystem()
	{
		return systemManager->registerSystem<T>();
	}

	template <typename T>
	void Coordinator::SetSystemSignature(const Signature signature) const
	{
		systemManager->setSignature<T>(signature);
	}
}
