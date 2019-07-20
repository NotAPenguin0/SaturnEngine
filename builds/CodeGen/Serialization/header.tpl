#ifndef MVG_COMPONENT_SERIALIZERS_HPP_
#define MVG_COMPONENT_SERIALIZERS_HPP_

/*	This file was generated using the serialization code generation program 
 *	for SaturnEngine. Do NOT modify this file manually.
 */

#include <nlohmann/json.hpp>
        
namespace Saturn::Components {
        
{{#ForwardDeclarations}}  
struct {{ComponentName}};
{{/ForwardDeclarations}}

{{#FromJsonDecl}}
void from_json(nlohmann::json const& json, {{ComponentName}}& component);
{{/FromJsonDecl}}

{{#ToJsonDecl}}
void to_json(nlohmann::json& json, {{ComponentName}} const& component);
{{/ToJsonDecl}}

} // namespace Saturn::Components
        
#endif