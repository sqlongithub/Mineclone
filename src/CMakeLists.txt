add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

target_include_directories(mineclone PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
