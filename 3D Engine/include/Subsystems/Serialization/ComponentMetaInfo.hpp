#ifndef SATURN_COMPONENT_META_INFO_HPP_
#define SATURN_COMPONENT_META_INFO_HPP_

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>

#include "Subsystems/ECS/component_index.hpp"

namespace Saturn {

namespace Meta {

struct ComponentInfo {
    // This has a space behind it to avoid problems with UI.
    // #DirtyFixPleaseFixBetter
    std::string name;

    using type_name_t = std::string;
    std::unordered_map<std::string, type_name_t> fields;
};

namespace detail {
void init_component_meta_data(std::vector<ComponentInfo>& data);
} // namespace detail

template<typename... Cs>
class ComponentsMeta {
public:
    static void init() { detail::init_component_meta_data(data); }

    template<typename C>
    static ComponentInfo const& get_component_meta_info() {
        const std::size_t index = index_table.template get<C>();
        assert(index != index_table.not_found);
        // Return the data
        return data[index];
    }

private:
    static inline std::vector<ComponentInfo> data = {};
    static inline component_index_table<Cs...> index_table = {};
};

} // namespace Meta

} // namespace Saturn

#endif
