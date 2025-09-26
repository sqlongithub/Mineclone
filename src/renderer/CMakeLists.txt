add_subdirectory(assets)
add_subdirectory(gpu)
add_subdirectory(materials)
add_subdirectory(mesh)
add_subdirectory(render_queue)

target_sources(${PROJECT_NAME} PRIVATE
    Renderer.cpp
)