add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(game)
add_subdirectory(renderer)

target_include_directories(mineclone PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
