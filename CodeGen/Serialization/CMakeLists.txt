project(Serialization)

set(CMAKE_CXX_STANDARD 17)

set(LIBCLANG_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/libclang/include
)

set(MUSTACHE_INCLUDE_DIR
	${CMAKE_CURRENT_SOURCE_DIR}/../dependencies
)

set(SERIALIZATION_SOURCE_FILES
	${SERIALIZATION_SOURCE_FILES}
	"${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp"
)

set(SERIALIZATION_HEADER_FILES
	${SERIALIZATION_HEADER_FILES}
	${CMAKE_CURRENT_SOURCE_DIR}/Parser.hpp
)

add_subdirectory(cppast)

# Add targets
add_executable(SerializationCodeGen
	${SERIALIZATION_HEADER_FILES}
	${SERIALIZATION_SOURCE_FILES}
)

target_include_directories(SerializationCodeGen PRIVATE
	${MUSTACHE_INCLUDE_DIR}
)

# ${CMAKE_CURRENT_SOURCE_DIR}/libclang/lib/libclang.lib 
target_link_libraries(SerializationCodeGen cppast)