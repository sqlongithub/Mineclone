target_sources(${PROJECT_NAME} PRIVATE
        Game.cpp
)

add_subdirectory(world)
add_subdirectory(input)
add_subdirectory(systems)
