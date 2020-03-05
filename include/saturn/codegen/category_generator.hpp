#ifndef SATURN_CODEGEN_CATEGORY_GENERATOR_HPP_
#define SATURN_CODEGEN_CATEGORY_GENERATOR_HPP_

#include <saturn/codegen/ast_visitor.hpp>

namespace saturn::codegen {

void generate_type_category_file(VisitResult const& data);

}

#endif