#include <saturn/assets/importers/obj.hpp>
#include <saturn/assets/assets.hpp>
#include <saturn/scene/scene.hpp>

#include <saturn/components/static_mesh.hpp>
#include <saturn/components/mesh_renderer.hpp>

#include <phobos/renderer/mesh.hpp>
#include <phobos/renderer/material.hpp>
#include <phobos/renderer/texture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stl/vector.hpp>
#include <stl/types.hpp>

#include <iostream>

namespace saturn::assets::importers {

using ModelMaterials = stl::vector<Handle<ph::Material>>;

static void add_mesh(Context& ctx, ModelMaterials const& materials, Model model, aiMesh const* mesh, aiScene const* scene) {
    
    ph::Mesh::CreateInfo info;
    info.ctx = ctx.vulkan;
    // Position, Normal, TexCoord
    info.vertex_size = 3 + 3 + 2;
    info.vertex_count = mesh->mNumVertices;
    stl::vector<float> vertices(stl::tags::uninitialized, info.vertex_size * mesh->mNumVertices);
    for (stl::size_t i = 0; i < mesh->mNumVertices; ++i) {
        stl::size_t const index = i * info.vertex_size;
        // Write data

        // Position
        vertices[index] = mesh->mVertices[i].x;
        vertices[index + 1] = mesh->mVertices[i].y;
        vertices[index + 2] = mesh->mVertices[i].z;
        // Normal
        vertices[index + 3] = mesh->mNormals[i].x;
        vertices[index + 4] = mesh->mNormals[i].y;
        vertices[index + 5] = mesh->mNormals[i].z;
        // TexCoord
        vertices[index + 6] = mesh->mTextureCoords[0][i].x;
        vertices[index + 7] = mesh->mTextureCoords[0][i].y;
    }

    info.vertices = vertices.data();

    // Assume each face has 3 indices (aka a triangle) when reserving memory
    stl::vector<stl::uint32_t> indices(stl::tags::reserve, mesh->mNumFaces * 3);
    for (stl::size_t i = 0; i < mesh->mNumFaces; ++i) {
        aiFace const& face = mesh->mFaces[i];
        for (stl::size_t j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    info.index_count = indices.size();
    info.indices = indices.data();
    
    // Create the mesh
    ph::Mesh loaded_mesh(info);
    // Send it to the asset system to store there
    Handle<ph::Mesh> handle = assets::take_mesh(loaded_mesh);

    // Finally add the loaded mesh to the blueprint
    using namespace components;
    ecs::registry& blueprints = ctx.scene->blueprints;
    blueprints.add_component<StaticMesh>(model.blueprint, handle);
    // Add the material for this mesh
    Handle<ph::Material> material = materials[mesh->mMaterialIndex];
    blueprints.add_component<MeshRenderer>(model.blueprint, material);
}

static void process_node(Context& ctx, ModelMaterials const& materials, Model model, aiNode const* node, aiScene const* scene) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        add_mesh(ctx, materials, model, mesh, scene);
        // NOTE: We only support a single mesh for each model at the moment since entity parenting isn't implemented yet
        break;
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        process_node(ctx, materials, model, node->mChildren[i], scene);
    }
}

static ModelMaterials load_materials(Context& ctx, fs::path const& cwd, aiScene const* scene) {
    ModelMaterials materials(stl::tags::reserve, scene->mNumMaterials);

    for (stl::size_t i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* mat = scene->mMaterials[i];
        // Only process the material if there is a diffuse texture. 
        if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            // Get diffuse texture
            aiString texture_path;
            mat->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path);
            fs::path path = cwd / fs::path(texture_path.C_Str());

            // Load the texture
            Handle<ph::Texture> texture = load_texture(ctx, path);
            // Assign it to the material
            ph::Material material;
            material.texture = get_texture(texture);

            // Send the material to the asset system
            Handle<ph::Material> handle = take_material(material);

            materials.push_back(handle);
        } else {
            // To make sure indices match
            materials.push_back(Handle<ph::Material>{-1});
        }
    }

    return materials;
}

Model import_obj_model(Context& ctx, fs::path const& path) {
    // A model is simply a blueprint entity
    ecs::entity_t blueprint = ctx.scene->blueprints.create_entity();

    constexpr int postprocess = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals;
    Assimp::Importer importer;
    aiScene const* scene = importer.ReadFile(path.generic_string(), postprocess);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        throw std::runtime_error("Failed to load model at path " + path.generic_string());
    }

    Model model { blueprint };

    ModelMaterials materials = load_materials(ctx, path.parent_path(), scene);
    process_node(ctx, materials, model, scene->mRootNode, scene);

    return model;
}

}