#include "ComponentArray.h"

#include <cassert>

namespace zt::core
{
	template <typename T>
	void ComponentArray<T>::insertData(const Entity entity, T component)
	{
		assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once.");

		// Put new entry at end and update the maps
		size_t newIndex = size;
		entityToIndexMap[entity] = newIndex;
		indexToEntityMap[newIndex] = entity;
		componentArray[newIndex] = component;
		++size;

	}

	template <typename T>
	void ComponentArray<T>::removeData(const Entity entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density
		size_t indexOfRemovedEntity = entityToIndexMap[entity];
		size_t indexOfLastElement = size - 1;
		componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

		// Update map to point to moved spot
		const Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
		entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		entityToIndexMap.erase(entity);
		indexToEntityMap.erase(indexOfLastElement);

		--size;
	}

	template <typename T>
	T& ComponentArray<T>::getData(const Entity entity)
	{
		assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component.");

		// Return a reference to the entity's component
		return componentArray[entityToIndexMap[entity]];

	}

	template <typename T>
	void ComponentArray<T>::entityDestroyed(const Entity entity)
	{
		if (entityToIndexMap.find(entity) != entityToIndexMap.end())
		{
			// Remove the entity's component if it existed
			removeData(entity);
		}
	}
}
