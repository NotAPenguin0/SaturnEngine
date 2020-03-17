#ifndef SATURN_CODEGEN_DISPLAY_FIELDS_GENERATOR_HPP_
#define SATURN_CODEGEN_DISPLAY_FIELDS_GENERATOR_HPP_

#include <saturn/codegen/ast_visitor.hpp>

namespace saturn::codegen {

void generate_field_display_dispatcher(VisitResult const& data);

}

#endif