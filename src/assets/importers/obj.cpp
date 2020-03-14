#include <saturn/assets/importers/obj.hpp>
#include <saturn/assets/assets.hpp>
#include <saturn/scene/scene.hpp>

#include <saturn/components/static_mesh.hpp>

#include <phobos/renderer/mesh.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <stl/vector.hpp>
#include <stl/types.hpp>

namespace saturn::assets::importers {

static void add_mesh(Context& ctx, Model model, aiMesh const* mesh, aiScene const* scene) {
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
}

static void process_node(Context& ctx, Model model, aiNode const* node, aiScene const* scene) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        add_mesh(ctx, model, mesh, scene);
        // NOTE: We only support a single mesh for each model at the moment since entity parenting isn't implemented yet
        break;
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        process_node(ctx, model, node->mChildren[i], scene);
    }
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

    process_node(ctx, model, scene->mRootNode, scene);

    return model;
}

}