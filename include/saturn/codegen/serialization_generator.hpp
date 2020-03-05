#ifndef SATURN_CODEGEN_SERIALIZATION_GENERATOR_HPP_
#define SATURN_CODEGEN_SERIALIZATION_GENERATOR_HPP_

#include <saturn/codegen/ast_visitor.hpp>

namespace saturn::codegen {

void generate_serialization_files(VisitResult const& data);

}

#endif