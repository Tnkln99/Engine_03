#pragma once

#include "ZtDevice.h"

#define GLM_FORCE_RADIANTS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace Zt {
	class ZtModel
	{
	public:
		struct Vertex {
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		ZtModel(ZtDevice& device, const std::vector<Vertex>& vertices);
		~ZtModel();

		ZtModel(const ZtModel&) = delete;
		ZtModel& operator=(const ZtModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);

		ZtDevice& ztDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}

