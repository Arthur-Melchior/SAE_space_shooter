cmake_minimum_required(VERSION 4.0)
project(space_shooter)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "/_deps/sfml-build")


include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

# only copies if files are different
file(COPY ${CMAKE_SOURCE_DIR}/_assets DESTINATION ${CMAKE_BINARY_DIR}/_assets)

add_custom_target(copy_resources
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/_assets ${CMAKE_BINARY_DIR}/_assets
        COMMENT "🔄 Copying all resources to build directory"
)

add_executable(space_shooter main.cpp
        player.h
        entity.h
        entity.cpp
        player.cpp)
add_dependencies(space_shooter copy_resources)

target_compile_features(space_shooter PRIVATE cxx_std_20)
target_link_libraries(space_shooter PRIVATE SFML::Graphics SFML::Audio)

target_compile_options(space_shooter PRIVATE
        $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od /Zi>
        $<$<AND:$<CONFIG:Debug>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-g -O0>
)

