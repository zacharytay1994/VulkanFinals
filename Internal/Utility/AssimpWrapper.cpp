#include "AssimpWrapper.h"

#include <fstream>
#include <stdexcept>
#include <unordered_map>

#include <assimp/include/assimp/Importer.hpp>
#include <assimp/include/assimp/scene.h>
#include <assimp/include/assimp/postprocess.h>

namespace UTILITY
{
    AssimpMesh ProcessMesh ( aiMesh* mesh , aiScene const* scene );
    std::vector<AssimpMaterial::Texture> LoadMaterialTextures ( aiMaterial* mat , aiTextureType type , std::string typeName );

    AssimpModel::AssimpModel ( std::string const& fbx )
    {
        Assimp::Importer importer;

        importer.SetPropertyBool ( AI_CONFIG_PP_PTV_NORMALIZE , true ); 
        auto process_flags = aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace;
        
        // read into scene
        const aiScene* scene = importer.ReadFile ( fbx.c_str() , process_flags );
        if ( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
        {
            throw std::runtime_error ( importer.GetErrorString () );
        }

        ProcessNode ( scene->mRootNode , scene , m_meshes );
    }

    std::vector<char> AssimpModel::ReadFile ( std::string const& filename )
	{
        std::ifstream file ( filename , std::ios::ate | std::ios::binary ); //start from end of file

        if ( !file.is_open () )
        {
            throw std::runtime_error ( "Failed to open file!" );
        }

        size_t fileSize = ( size_t ) file.tellg ();
        std::vector<char> buffer ( fileSize );

        file.seekg ( 0 );
        file.read ( buffer.data () , fileSize );

        file.close ();
        return buffer;
	}

    void AssimpModel::ProcessNode ( aiNode* node , const aiScene* scene , std::vector<AssimpMesh>& meshes )
    {
        // process all the node's meshes (if any)
        for ( unsigned int i = 0; i < node->mNumMeshes; i++ )
        {
            aiMesh* mesh = scene->mMeshes[ node->mMeshes[ i ] ];
            meshes.push_back ( ProcessMesh ( mesh , scene ) );
        }
        // then do the same for each of its children
        for ( unsigned int i = 0; i < node->mNumChildren; i++ )
        {
            ProcessNode ( node->mChildren[ i ] , scene , meshes );
        }
    }

    AssimpMesh ProcessMesh ( aiMesh* mesh , aiScene const* scene )
    {
        std::vector<AssimpVertex> vertices;
        std::vector<unsigned int> indices;

        for ( unsigned int i = 0; i < mesh->mNumVertices; i++ )
        {
            AssimpVertex vertex;

            glm::vec3 vector;
            vector.x = mesh->mVertices[ i ].x;
            vector.y = mesh->mVertices[ i ].y;
            vector.z = mesh->mVertices[ i ].z;
            vertex.m_position = vector;

            vertex.m_normal.x = mesh->mNormals[ i ].x;
            vertex.m_normal.y = mesh->mNormals[ i ].y;
            vertex.m_normal.z = mesh->mNormals[ i ].z;

            vertex.m_tangent.x = mesh->mTangents[ i ].x;
            vertex.m_tangent.y = mesh->mTangents[ i ].y;
            vertex.m_tangent.z = mesh->mTangents[ i ].z;

            if ( mesh->mColors[ 0 ] )
            {
                vertex.m_colour.r = mesh->mColors[ i ]->r;
                vertex.m_colour.g = mesh->mColors[ i ]->g;
                vertex.m_colour.b = mesh->mColors[ i ]->b;
                vertex.m_colour.a = mesh->mColors[ i ]->a;
            }
            else
                vertex.m_colour = glm::vec4 ( 1.0f );

            if ( mesh->mTextureCoords[ 0 ] ) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[ 0 ][ i ].x;
                vec.y = mesh->mTextureCoords[ 0 ][ i ].y;
                vertex.m_uv = vec;
            }
            else
                vertex.m_uv = glm::vec2 ( 0.0f , 0.0f );

            if ( mesh->mMaterialIndex >= 0 )
            {
                AssimpMaterial material;
                aiMaterial* ai_material = scene->mMaterials[ mesh->mMaterialIndex ];

                //unused for now
                material.m_diffuse_maps = LoadMaterialTextures ( ai_material , aiTextureType_DIFFUSE , "texture_diffuse" );
                material.m_specular_maps = LoadMaterialTextures ( ai_material , aiTextureType_SPECULAR , "texture_specular" );
            }

            vertices.push_back ( vertex );
        }

        for ( unsigned int j = 0; j < mesh->mNumFaces; j++ )
        {
            aiFace face = mesh->mFaces[ j ];
            for ( unsigned int k = 0; k < face.mNumIndices; k++ )
            {
                indices.push_back ( face.mIndices[ k ] );
            }

        }

        return { vertices, indices };
    }

    std::vector<AssimpMaterial::Texture> LoadMaterialTextures ( aiMaterial* mat , aiTextureType type , std::string typeName )
    {
        std::unordered_map<std::string , AssimpMaterial::Texture> textureMap;
        std::vector<AssimpMaterial::Texture> textures;

        for ( unsigned int i = 0; i < mat->GetTextureCount ( type ); i++ )
        {
            aiString str;
            mat->GetTexture ( type , i , &str );
            AssimpMaterial::Texture texture;

            if ( textureMap.count ( str.C_Str () ) )
                continue;

            // no load texture with assimp, load with dds instead
            texture.m_id = 0;
            texture.m_type = typeName;
            texture.m_path = str.C_Str ();
            textureMap.insert ( { texture.m_path, texture } );
        }

        for ( auto& entry : textureMap )
        {
            auto& texture = entry.second;
            textures.push_back ( texture );
        }

        return textures;
    }
}