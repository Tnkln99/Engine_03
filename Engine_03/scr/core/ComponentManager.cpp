#include"ComponentManager.h"

#include <cassert>

namespace zt::core
{
	template <typename T>
	void ComponentManager::registerComponent()
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once.");

		// Add this component type to the component type map
		componentTypes.insert({ typeName, nextComponentType });

		// Create a ComponentArray pointer and add it to the component arrays map
		componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		// Increment the value so that the next component registered will be different
		++nextComponentType;
	}

	template <typename T>
	ComponentType ComponentManager::getComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		// Return this component's type - used for creating signatures
		return componentTypes[typeName];
	}

	template <typename T>
	void ComponentManager::addComponent(Entity entity, T component)
	{
		// Add a component to the array for an entity
		getComponentArray<T>()->InsertData(entity, component);
	}

	template <typename T>
	void ComponentManager::removeComponent(Entity entity)
	{
		// Remove a component from the array for an entity
		getComponentArray<T>()->RemoveData(entity);
	}

	template <typename T>
	T& ComponentManager::getComponent(Entity entity)
	{
		// Get a reference to a component from the array for an entity
		return getComponentArray<T>()->GetData(entity);
	}

	inline void ComponentManager::entityDestroyed(const Entity entity) const
	{
		// Notify each component array that an entity has been destroyed
// If it has a component for that entity, it will remove it
		for (auto const& pair : componentArrays)
		{
			auto const& component = pair.second;

			component->entityDestroyed(entity);
		}
	}

	template <typename T>
	std::shared_ptr<ComponentArray<T>> ComponentManager::getComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
	}
}