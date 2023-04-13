#pragma once
#include <memory>
#include <unordered_map>

#include "Component.h"
#include "ComponentArray.h"
#include "Entity.h"

namespace zt::core
{
	class ComponentManager
	{
	public:
		template<typename T>
		void registerComponent();

		template<typename T>
		ComponentType getComponentType();

		template<typename T>
		void addComponent(Entity entity, T component);

		template<typename T>
		void removeComponent(Entity entity);

		template<typename T>
		T& getComponent(Entity entity);

		void entityDestroyed(Entity entity) const;

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> componentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType nextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray();
	};
}
