#include "AssetManager/ResourceLoaders.hpp"

#include "Editor/EditorLog.hpp"
#include "Logging/LogSystem.hpp"

#include <Renderer/stb_image.h>
#include <fstream>

#include "AssetManager/AssetManager.hpp"
#include "Editor/ProjectFile.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <nlohmann/json.hpp>

namespace Saturn {

LoadResult<Shader> ResourceLoader<Shader>::load(std::string const& path,
                                                std::string const& root_dir) {
    std::ifstream file(path);
    if (!file.good()) {
        log::error("Failed to open shader file at path: {}/{}", root_dir, path);
        return {nullptr, {}};
    }
    Shader::CreateInfo info;

    // The shader file contains two paths: the first is to the vertex shader,
    // the second to the fragment shader

    std::string vtx, frag, geom;
    std::getline(file, vtx);
    std::getline(file, frag);
    if (std::getline(file, geom) && geom != "None") {
        info.geom_path = root_dir + "/" + geom;
    }
    vtx = root_dir + vtx;
    frag = root_dir + frag;
    info.vtx_path = vtx;
    info.frag_path = frag;

    return {std::make_unique<Shader>(info),
            {fs::absolute(vtx), fs::absolute(frag)}};
}

void ResourceLoader<Shader>::reload(std::unique_ptr<Shader>& res,
                                    std::vector<fs::path>& dependent_paths,
                                    std::string const& path,
                                    std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

LoadResult<Mesh> ResourceLoader<Mesh>::load(std::string const& path,
                                            std::string const& root_dir) {
    std::ifstream file(path);
    if (!file.good()) {
        log::error("Failed to open mesh file at path: {}/{}", root_dir, path);
        return {nullptr, {}};
    }

    // A Mesh file looks like this:
    /*
     *attributes: [N]
     *attribute_1_name location_in_shader_1 num_components_1
     *...
     *attribute_n_name location_in_shader_n num_components_n
     *vertices: [N]
     *[List of comma separated vertex data, N vertices]
     *Indices: [N] # Leave zero to auto-generate
     *[List of comma separated indices, N indices]
     **/

    // PUT A COMMA AT THE END OF THE LIST AS WELL OR PARSING FAILS

    Mesh::CreateInfo info;

    std::string dummy;
    file >> dummy;
    assert(dummy == "attributes:" && "Syntax error: Expected /'attributs:/'");
    std::size_t num_attributes;
    file >> num_attributes;

    // We'll need this when reading the vertex data itself
    std::size_t total_vertex_components = 0;

    // Read attribute data
    info.vertices.attributes.resize(num_attributes);
    for (auto& attr : info.vertices.attributes) {
        file >> dummy; // Attribute name
        std::size_t location_in_shader;
        std::size_t num_components;
        file >> location_in_shader >> num_components;

        attr.location_in_shader = location_in_shader;
        attr.num_components = num_components;

        total_vertex_components += num_components;
    }

    // Read vertex data
    file >> dummy;
    assert(dummy == "vertices:" && "Syntax error: Expected /'vertices:/'");
    std::size_t num_vertices;
    file >> num_vertices;
    info.vertices.vertices.resize(num_vertices * total_vertex_components);
    for (auto& vtx : info.vertices.vertices) {
        file >> vtx;
        char comma;
        file >> comma;
    }

    file >> dummy;
    assert(dummy == "indices:" && "Syntax error: Expected /'indices:/'");
    std::size_t num_indices;
    file >> num_indices;
    if (num_indices != 0) {
        info.vertices.indices.resize(num_indices);
        for (auto& idx : info.vertices.indices) {
            file >> idx;
            char comma;
            file >> comma;
        }
    }

    info.vertices.dynamic = false;

    return {std::make_unique<Mesh>(info), {}};
}

void ResourceLoader<Mesh>::reload(std::unique_ptr<Mesh>& res,
                                  std::vector<fs::path>& dependent_paths,
                                  std::string const& path,
                                  std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

static TextureFormat format_from_string(std::string const& str) {
    if (str == "RGB") return TextureFormat::RGB;
    if (str == "RGBA") return TextureFormat::RGBA;
    if (str == "SRGB") return TextureFormat::SRGB;
    if (str == "SRGB_ALPHA") return TextureFormat::SRGBAlpha;

    throw std::runtime_error("Invalid texture format!");
}

static TextureTarget target_from_string(std::string const& str) {
    if (str == "Texture1D") return TextureTarget::Texture1D;
    if (str == "Texture1DArray") return TextureTarget::Texture1DArray;
    if (str == "Texture2D") return TextureTarget::Texture2D;
    if (str == "Texture2DArray") return TextureTarget::Texture2DArray;
    if (str == "Texture2DMultiSample")
        return TextureTarget::Texture2DMultiSample;
    if (str == "Texture2DMultiSampleArray")
        return TextureTarget::Texture2DMultiSampleArray;
    if (str == "Texture3D") return TextureTarget::Texture3D;
    if (str == "CubeMap") return TextureTarget::CubeMap;
    if (str == "CubeMapArray") return TextureTarget::CubeMapArray;
    if (str == "TextureRectangle") return TextureTarget::TextureRectangle;

    throw std::runtime_error("Invalid texture target!");
}

static TextureParameter param_from_string(std::string const& str) {
    if (str == "DepthStencilMode") return TextureParameter::DepthStencilMode;
    if (str == "BaseLevel") return TextureParameter::BaseLevel;
    if (str == "CompareFunc") return TextureParameter::CompareFunc;
    if (str == "CompareMode") return TextureParameter::CompareMode;
    if (str == "LodBias") return TextureParameter::LodBias;
    if (str == "MinFilter") return TextureParameter::MinFilter;
    if (str == "MagFilter") return TextureParameter::MagFilter;
    if (str == "MinLod") return TextureParameter::MinLod;
    if (str == "MaxLod") return TextureParameter::MaxLod;
    if (str == "MaxLevel") return TextureParameter::MaxLevel;
    if (str == "SwizzleR") return TextureParameter::SwizzleR;
    if (str == "SwizzleG") return TextureParameter::SwizzleG;
    if (str == "SwizzleB") return TextureParameter::SwizzleB;
    if (str == "SwizzleA") return TextureParameter::SwizzleA;
    if (str == "WrapS") return TextureParameter::WrapS;
    if (str == "WrapT") return TextureParameter::WrapT;
    if (str == "WrapR") return TextureParameter::WrapR;

    throw std::runtime_error("Invalid texture parameter!");
}

static TextureParameterValue value_from_string(std::string const& str) {
    if (str == "DepthComponent") return TextureParameterValue::DepthComponent;
    if (str == "Nearest") return TextureParameterValue::Nearest;
    if (str == "Linear") return TextureParameterValue::Linear;
    if (str == "NearestMipmapNearest")
        return TextureParameterValue::NearestMipmapNearest;
    if (str == "LinearMipmapLinear")
        return TextureParameterValue::LinearMipmapLinear;
    if (str == "LinearMipmapNearest")
        return TextureParameterValue::LinearMipmapNearest;
    if (str == "NearestMipmapLinear")
        return TextureParameterValue::NearestMipmapLinear;
    if (str == "ClampToEdge") return TextureParameterValue::ClampToEdge;
    if (str == "ClampToBorder") return TextureParameterValue::ClampToBorder;
    if (str == "MirroredRepeat") return TextureParameterValue::MirroredRepeat;
    if (str == "Repeat") return TextureParameterValue::Repeat;

    throw std::runtime_error("Invalid texture parameter value!");
}

LoadResult<Texture> ResourceLoader<Texture>::load(std::string const& path,
                                                  std::string const& root_dir) {
    std::ifstream file(path);
    if (!file.good()) {
        log::error("Failed to open texture file at path: {}/{}", root_dir,
                   path);
        return {nullptr, {}};
    }

    /*
     *A texture file looks like this:
     *texture_target
     *path_to_file
     *texture_unit
     *texture_format
     *internal_texture_format
     *flip_y (bool, true/false)
     *paramter_count
     *param1 = value
     *param2 = value
     *...
     *paramN = value
     **/

    // Read texture target
    std::string str;
    file >> str;
    auto target = target_from_string(str);
    // Read path to image file
    file >> str;
    std::string path_to_file = std::move(str);
    // Read texture unit
    int unit_n;
    file >> unit_n;
    auto unit = (GLenum)(GL_TEXTURE0 + unit_n);
    // Read texture format
    file >> str;
    auto format = format_from_string(str);
    file >> str;
    auto internal_format = format_from_string(str);
    // Read flip
    file >> str;
    bool flip_y = (str == "true");
    // Read param count
    std::size_t param_count;
    file >> param_count;
    // Read parameters
    std::vector<Texture::ParameterInfo> params(param_count);
    for (auto& param : params) {
        file >> str;
        param.parameter = param_from_string(str);
        char dummy;
        file >> dummy; // read '=' sign
        file >> str;
        param.value = value_from_string(str);
    }

    path_to_file = root_dir + path_to_file;

    Texture::CreateInfo info;
    info.flip_y = flip_y;
    info.format = format;
    info.internal_format = internal_format;
    info.image_path = path_to_file;
    info.parameters = std::move(params);
    info.texture_unit = unit;
    info.target = target;

    return {std::make_unique<Texture>(info), {fs::absolute(path_to_file)}};
}

void ResourceLoader<Texture>::reload(std::unique_ptr<Texture>& res,
                                     std::vector<fs::path>& dependent_paths,
                                     std::string const& path,
                                     std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

LoadResult<audeo::SoundSource>
ResourceLoader<audeo::SoundSource>::load(std::string const& path,
                                         std::string const& root_dir) {
    std::ifstream file(path);
    if (!file.good()) {
        log::error("Failed to open sound source file at path: {}/{}", root_dir,
                   path);
        return {nullptr, {}};
    }

    // First line contains the path to the sound source
    // Second line: Effect or Music

    std::string src_path;
    std::getline(file, src_path);
    std::string type_s;
    file >> type_s;

    audeo::AudioType type = audeo::AudioType::Effect;
    if (type_s == "Music") { type = audeo::AudioType::Music; }

    audeo::SoundSource source = audeo::load_source(root_dir + src_path, type);
    if (audeo::is_valid(source)) {
        return {std::make_unique<audeo::SoundSource>(source),
                {fs::absolute(root_dir + src_path)}};
    } else {
        return {nullptr, {}};
    }
}

void ResourceLoader<audeo::SoundSource>::reload(
    std::unique_ptr<audeo::SoundSource>& res,
    std::vector<fs::path>& dependent_paths,
    std::string const& path,
    std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    *res = *new_res.ptr;
    dependent_paths = std::move(new_res.dependent_paths);
}

LoadResult<Font> ResourceLoader<Font>::load(std::string const& path,
                                            std::string const& root_dir) {
    Font::CreateInfo info;
    info.font_path = path;
    return {std::make_unique<Font>(info), {}};
}

void ResourceLoader<Font>::reload(std::unique_ptr<Font>& res,
                                  std::vector<fs::path>& dependent_paths,
                                  std::string const& path,
                                  std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

enum class CubeMapFace {
    Right = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    Left = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    Top = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    Bottom = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    Back = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    Front = GL_TEXTURE_CUBE_MAP_POSITIVE_Z
};

std::ifstream& operator>>(std::ifstream& in, CubeMapFace& fc) {
    std::string s;
    in >> s;
    if (s == "Right") {
        fc = CubeMapFace::Right;
    } else if (s == "Left") {
        fc = CubeMapFace::Left;
    } else if (s == "Top") {
        fc = CubeMapFace::Top;
    } else if (s == "Bottom") {
        fc = CubeMapFace::Bottom;
    } else if (s == "Back") {
        fc = CubeMapFace::Back;
    } else if (s == "Front") {
        fc = CubeMapFace::Front;
    }

    return in;
}

void load_face(CubeMapFace face_id, std::string const& path) {
    int width, height, channels;
    unsigned char* data =
        stbi_load(path.c_str(), &width, &height, &channels, 0);
    glTexImage2D(static_cast<GLenum>(face_id), 0, GL_RGB, width, height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

LoadResult<CubeMap> ResourceLoader<CubeMap>::load(std::string const& path,
                                                  std::string const& root_dir) {
    std::ifstream base_file(path);
    // This file contains all the faces of the cube map, and other data like
    // format

    bool flip = false;
    std::string s;
    base_file >> s;
    flip = (s == "true");
    stbi_set_flip_vertically_on_load(flip);

    std::vector<fs::path> face_paths;
    constexpr size_t face_count = 6;

    // Create the cubemap texture
    unsigned int handle;
    glGenTextures(1, &handle);
    glBindTexture(GL_TEXTURE_CUBE_MAP, handle);

    for (size_t i = 0; i < face_count; ++i) {
        CubeMapFace face_id;
        std::string face_path;

        base_file >> face_id >> face_path;
        face_path = root_dir + face_path;

        load_face(face_id, face_path);

        face_paths.emplace_back(face_path);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return {std::make_unique<CubeMap>(handle), std::move(face_paths)};
}

void ResourceLoader<CubeMap>::reload(std::unique_ptr<CubeMap>& res,
                                     std::vector<fs::path>& dependent_paths,
                                     std::string const& path,
                                     std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

template<typename T>
void json_get_or(nlohmann::json const& j,
                 std::string_view key,
                 T& dest,
                 T const& def = T{}) {
    if (auto json_it = j.find(key); json_it != j.end()) {
        dest = (*json_it).get<T>();
    } else {
        dest = def;
    }
}

template<typename R>
void load_res_if_present(Resource<R>& res,
                         std::string const& path,
                         std::string const& root,
                         std::vector<fs::path>& dependent_paths) {

    if (path != "") {
        res = AssetManager<R>::get_resource(root + path, false, true);
        dependent_paths.emplace_back(root + path);
    }
}

LoadResult<Material>
ResourceLoader<Material>::load(std::string const& path,
                               std::string const& root_dir) {
    Material mat;
    std::vector<fs::path> paths;

    nlohmann::json j;
    std::ifstream file(path);
    file >> j;

    std::string diffuse, specular, normal, shader;
    // get paths to different resources used in this material
    json_get_or(j, "diffuse", diffuse);
    json_get_or(j, "specular", specular);
    json_get_or(j, "normal", normal);
    json_get_or(j, "shader", shader);
    // fix their paths
    load_res_if_present(mat.diffuse_map, diffuse, root_dir, paths);
    load_res_if_present(mat.specular_map, specular, root_dir, paths);
    load_res_if_present(mat.normal_map, normal, root_dir, paths);
    load_res_if_present(mat.shader, shader, root_dir, paths);

    json_get_or(j, "shininess", mat.shininess, 100.0f);
    json_get_or(j, "lit", mat.lit, true);
    json_get_or(j, "reflective", mat.reflective, false);

    return {std::make_unique<Material>(std::move(mat)), paths};
}

void ResourceLoader<Material>::reload(std::unique_ptr<Material>& res,
                                      std::vector<fs::path>& dependent_paths,
                                      std::string const& path,
                                      std::string const& root_dir) {

    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

void add_mesh(Model& model,
              const aiMesh* mesh,
              const aiScene* scene,
              std::vector<Resource<Material>> const& materials) {
    std::vector<float> data;
    std::vector<unsigned int> indices;

    const size_t vertex_size = 11;
    const size_t buf_elems = mesh->mNumVertices * vertex_size;
    data.reserve(buf_elems);

    indices.reserve(mesh->mNumFaces * 3);

    // vertices
    for (size_t i = 0; i < mesh->mNumVertices; ++i) {
        data.push_back(mesh->mVertices[i].x);
        data.push_back(mesh->mVertices[i].y);
        data.push_back(mesh->mVertices[i].z);
        data.push_back(mesh->mTextureCoords[0][i].x);
        data.push_back(mesh->mTextureCoords[0][i].y);
        data.push_back(mesh->mNormals[i].x);
        data.push_back(mesh->mNormals[i].y);
        data.push_back(mesh->mNormals[i].z);
        data.push_back(mesh->mTangents[i].x);
        data.push_back(mesh->mTangents[i].y);
        data.push_back(mesh->mTangents[i].z);
    }

    // indices
    for (size_t i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh::CreateInfo info;
    info.vertices.vertices = std::move(data);
    info.vertices.indices = std::move(indices);
    info.vertices.attributes.push_back({0, 3}); // pos
    info.vertices.attributes.push_back({1, 2}); // tx
    info.vertices.attributes.push_back({2, 3}); // norm
    info.vertices.attributes.push_back({3, 3}); // tan

    model.meshes.emplace_back();
    model.meshes.back().assign(info);

    if (mesh->mMaterialIndex >= 0) {
        model.meshes.back().material = materials[mesh->mMaterialIndex];
    }
}

void process_node(const aiNode* node,
                  const aiScene* scene,
                  Model& model,
                  std::vector<Resource<Material>> const& materials) {
    for (size_t i = 0; i < node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        add_mesh(model, mesh, scene, materials);
    }

    for (size_t i = 0; i < node->mNumChildren; ++i) {
        process_node(node->mChildren[i], scene, model, materials);
    }
}

void create_tex_file(fs::path const& path,
                     fs::path const& img_path,
                     aiTextureType tex_type) {

    size_t unit = 0;
    std::string format = "SRGB";

    if (tex_type == aiTextureType_DIFFUSE) {
        unit = 1;
    } else if (tex_type == aiTextureType_SPECULAR) {
        unit = 0;
        format = "RGBA";
    } else if (tex_type == aiTextureType_HEIGHT) {
        unit = 3;
        format = "RGBA";
    }

    std::ofstream file(path);
    file << "Texture2D\n";
    file << img_path.generic_string() << "\n";
    file << unit << "\n";
    file << "RGBA\n";
    file << format << "\n";
    file << "false\n";
    file << 4 << "\n";
    file << "WrapS = Repeat\n"
            "WrapT = Repeat\nMinFilter = LinearMipmapLinear\nMagFilter = "
            "Linear\n";
}

void create_material_texture(aiMaterial* material,
                             fs::path const& path,
                             std::string const& root_dir,
                             aiTextureType type,
                             std::string_view key,
                             nlohmann::json& j) {
    // Diffuse texture. We only support one of each texture for now
    if (material->GetTextureCount(type) > 0) {
        aiString t_path;

        material->GetTexture(type, 0, &t_path);
        fs::path img_path = fs::relative(root_dir + t_path.C_Str(),
                                         Editor::ProjectFile::root_path());
        fs::path tex_path = Editor::ProjectFile::root_path() /
                            "assets/textures/" /
                            (img_path.stem().string() + ".tex");
        create_tex_file(tex_path, img_path, type);
        j[std::string(key)] =
            fs::relative(tex_path.string(), Editor::ProjectFile::root_path())
                .generic_string();
    }
}
void create_material_file(aiMaterial* material,
                          fs::path const& path,
                          std::string const& root_dir) {
    nlohmann::json j;
	
    create_material_texture(material, path, root_dir, aiTextureType_DIFFUSE,
                            "diffuse", j);
    create_material_texture(material, path, root_dir, aiTextureType_SPECULAR,
                            "specular", j);
	// normal/bump map is apparently stored as height in Assimp
    create_material_texture(material, path, root_dir, aiTextureType_HEIGHT,
                            "normal", j);
    j["shader"] = "assets/shaders/normal_mapping.sh";

    std::ofstream file(path);
    file << j;
}

std::vector<Resource<Material>> load_materials(const aiScene* scene,
                                               std::string const& root_dir) {
    using namespace std::literals::string_literals;
    std::vector<Resource<Material>> materials;

    for (size_t i = 0; i < scene->mNumMaterials; ++i) {
        aiMaterial* material = scene->mMaterials[i];
        // Convert to our own format and load from asset manager
        aiString name = material->GetName();
        std::string material_rel =
            "assets/materials/" + (name.C_Str() + ".mtl"s);
        fs::path material_path =
            Editor::ProjectFile::root_path() / material_rel;
        create_material_file(material, material_path, root_dir);
        materials.push_back(
            AssetManager<Material>::get_resource(material_rel, false, true));
    }

    return materials;
}

LoadResult<Model> ResourceLoader<Model>::load(std::string const& path,
                                              std::string const& root_dir) {

    Model model;

    const int ai_load_flags = aiProcess_Triangulate |
                              aiProcess_CalcTangentSpace | aiProcess_FlipUVs |
                              aiProcess_GenNormals;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, ai_load_flags);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
        return {nullptr, {}};
    }

    std::vector<Resource<Material>> materials =
        load_materials(scene, root_dir + "/assets/models/");

    process_node(scene->mRootNode, scene, model, materials);

    return {std::make_unique<Model>(std::move(model)), {}};
}

void ResourceLoader<Model>::reload(std::unique_ptr<Model>& res,
                                   std::vector<fs::path>& dependent_paths,
                                   std::string const& path,
                                   std::string const& root_dir) {
    auto new_res = load(path, root_dir);
    res->swap(*new_res.ptr);
    dependent_paths = std::move(new_res.dependent_paths);
}

// File types definitions

std::vector<FileType> FileTypes<Shader>::types = {
    {L"Shader file (*.sh)", L"*.sh"}};
std::vector<FileType> FileTypes<Mesh>::types = {
    {L"Mesh file (*.mesh)", L"*.mesh"}};
std::vector<FileType> FileTypes<Texture>::types = {
    {L"Texture file (*.tex)", L"*tex"}};
std::vector<FileType> FileTypes<audeo::SoundSource>::types = {
    {L"SoundSource file (*.sfx)", L"*.sfx"}};
std::vector<FileType> FileTypes<Font>::types = {
    {L"TrueType font file (*.ttf)", L"*.ttf"}};
std::vector<FileType> FileTypes<CubeMap>::types = {
    {L"CubeMap file (*.cm)", L"*.cm"}};
std::vector<FileType> FileTypes<Material>::types = {
    {L"Material file (*.mtl)", L"*.mtl"}};
// #TODO add more model file types here
std::vector<FileType> FileTypes<Model>::types = {
    {L"Obj file (*.obj)", L"*.obj"}};

} // namespace Saturn
