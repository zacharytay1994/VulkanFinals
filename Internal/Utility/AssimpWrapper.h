#pragma once

#include "../VulkanObjects/vk_vertex.h"

#include <glm/glm/glm.hpp>

#include <vector>
#include <string>

struct aiNode;
struct aiScene;

namespace UTILITY
{
	using AssimpVertex = VK_OBJECT::Vertex;

	struct AssimpMaterial
	{
		struct Texture
		{
			unsigned int	m_id {};
			std::string		m_type;
			std::string		m_path;
		};

		std::vector<Texture> m_diffuse_maps;
		std::vector<Texture> m_specular_maps;
	};

	struct AssimpMesh
	{
		std::vector<AssimpVertex>	m_vertices;
		std::vector<uint32_t>		m_indices;
	};

	struct AssimpModel
	{
		std::vector<AssimpMesh> m_meshes;

		AssimpModel ( std::string const& fbx );
	private:
		std::vector<char> ReadFile ( std::string const& filename );

		void ProcessNode ( aiNode* node , const aiScene* scene , std::vector<AssimpMesh>& meshes );
	};
}