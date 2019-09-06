#ifndef SATURN_EDITOR_ENTITY_TREE_HPP_
#define SATURN_EDITOR_ENTITY_TREE_HPP_

#ifdef WITH_EDITOR

#    include "EditorWidget.hpp"

#    include <vector>

namespace Saturn {
class Scene;
class SceneObject;
} // namespace Saturn

namespace Saturn::Editor {

class EntityTree : public EditorWidget {
public:
    EntityTree();
    using tree_t = std::vector<SceneObject*>;

    SceneObject* get_selected_entity() const;
    void reset_selected_entity();

    void show(Scene& scene);
private:
    SceneObject* selected_entity = nullptr;

	tree_t build_entity_tree(Scene& scene);
	void show_entity_tree(tree_t& enttree, Scene& scene);
    bool has_child(tree_t& tree, tree_t::iterator entity);
	tree_t::iterator show_self_and_children(Scene& scene, tree_t& tree, tree_t::iterator entity);

	void show_entity_details(SceneObject* entity, Scene& scene);
};

} // namespace Saturn::Editor

#endif

#endif
