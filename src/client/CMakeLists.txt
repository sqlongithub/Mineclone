target_sources(${PROJECT_NAME} PRIVATE
        Game.cpp
)

add_subdirectory(physics)
add_subdirectory(rendering)
add_subdirectory(input)
add_subdirectory(core)
add_subdirectory(world)
