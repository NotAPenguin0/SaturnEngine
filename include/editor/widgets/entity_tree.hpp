#ifndef SATURN_EDITOR_ENTITY_TREE_HPP_
#define SATURN_EDITOR_ENTITY_TREE_HPP_

#include <editor/widgets/widget.hpp>

namespace editor {

struct EntityDragDropData {
    saturn::ecs::entity_t entity = 0;
    saturn::ecs::registry* registry = nullptr;
};  

class EntityTree : public Widget {
public:
    EntityTree(const char* name, saturn::ecs::registry& ecs);
    void show(saturn::FrameContext& ctx) override;

    saturn::ecs::entity_t selected_entity = 0;
    
private:
    std::string name;
    saturn::ecs::registry* ecs;
    EntityDragDropData current_payload;
};
    
}

#endif