#pragma once
#include <memory>
#include <unordered_map>

#include "Signature.h"
#include "System.h"


namespace zt::core
{
	class SystemManager
	{
	public:
		template<typename T>
		std::shared_ptr<T> registerSystem();

		template<typename T>
		void setSignature(Signature signature);

		void entityDestroyed(Entity entity) const;

		void entitySignatureChanged(Entity entity, Signature entitySignature);

	private:
		// Map from system type string pointer to a signature
		std::unordered_map<const char*, Signature> signatures{};

		// Map from system type string pointer to a system pointer
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
}
