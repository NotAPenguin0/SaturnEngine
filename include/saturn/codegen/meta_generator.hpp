#ifndef SATURN_CODEGEN_META_GENERATOR_HPP_
#define SATURN_CODEGEN_META_GENERATOR_HPP_

#include <saturn/codegen/ast_visitor.hpp>

namespace saturn::codegen {

void generate_meta_files(VisitResult const& data);

}

#endif