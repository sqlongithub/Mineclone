cmake_minimum_required(VERSION 3.25)
project(mineclone VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(BUILD_SHARED_LIBS OFF)



if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "In-source builds not allowed")
endif()

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external/")

if(WIN32)
    set(OPENGL_LIBRARIES opengl32)
else()
    find_package(OpenGL REQUIRED)
    set(OPENGL_LIBRARIES OpenGL::GL)
endif()


set(GLAD_DIR "${EXTERNAL_DIR}/glad")
if(EXISTS "${GLAD_DIR}/src/glad.c")
    add_library(glad STATIC ${GLAD_DIR}/src/glad.c)
    set_source_files_properties(${GLAD_DIR}/src/glad.c PROPERTIES LANGUAGE C)
    target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
else()
    message(FATAL_ERROR "GLAD not found at ${GLAD_DIR}")
endif()


if(EXISTS "${EXTERNAL_DIR}/glfw/CMakeLists.txt")
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(${EXTERNAL_DIR}/glfw)
else()
    include(FetchContent)
    FetchContent_Declare(glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG master
    )
    FetchContent_MakeAvailable(glfw)
endif()


if(EXISTS "${EXTERNAL_DIR}/imgui")
    file(GLOB IMGUI_SOURCES
            ${EXTERNAL_DIR}/imgui/*.cpp
            ${EXTERNAL_DIR}/imgui/backends/imgui_impl_glfw.cpp
            ${EXTERNAL_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    )
    add_library(imgui STATIC ${IMGUI_SOURCES})
    target_include_directories(imgui PUBLIC
            ${EXTERNAL_DIR}/imgui
            ${EXTERNAL_DIR}/imgui/backends
    )
    target_link_libraries(imgui PUBLIC glfw glad)
else()
    message(FATAL_ERROR "ImGui not found at ${EXTERNAL_DIR}/imgui")
endif()


include(FetchContent)
FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG master
)
FetchContent_MakeAvailable(glm)


FetchContent_Declare(glaze
        GIT_REPOSITORY https://github.com/stephenberry/glaze.git
        GIT_TAG main
        GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(glaze)


add_executable(${PROJECT_NAME} src/main.cpp
        src/common/physics/systems/CollisionSystem.cpp
        src/common/physics/systems/CollisionSystem.h
        src/common/physics/components/Collider.h
        src/common/ecs/components/Velocity.h
        src/common/physics/systems/PhysicsMovementSystem.cpp
        src/common/physics/systems/PhysicsMovementSystem.h
        src/common/physics/components/PhysicsProperties.h
        src/common/physics/components/MoveIntent.h)


target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        $<$<CONFIG:Debug>:DEBUG>
)

add_subdirectory(src/)


target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/src"
        "${EXTERNAL_DIR}"
)


target_link_libraries(${PROJECT_NAME} PRIVATE
        ${OPENGL_LIBRARIES}
        glad
        glfw
        imgui
        glm::glm
        glaze::glaze
)


target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLFW_INCLUDE_NONE
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
            NOMINMAX
            WIN32_LEAN_AND_MEAN
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_BUILD)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")