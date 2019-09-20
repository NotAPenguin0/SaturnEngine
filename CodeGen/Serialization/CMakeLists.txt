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
)

include_directories(${LIBCLANG_INCLUDE_DIR})
include_directories(${MUSTACHE_INCLUDE_DIR})


# Add targets
add_executable(SerializationCodeGen
	${SERIALIZATION_HEADER_FILES}
	${SERIALIZATION_SOURCE_FILES}
)

target_include_directories(SerializationCodeGen PRIVATE
	${LIBCLANG_INCLUDE_DIR}
	${MUSTACHE_INCLUDE_DIR}
)


target_link_libraries(SerializationCodeGen 
	${CMAKE_CURRENT_SOURCE_DIR}/libclang/lib/libclang.lib)