cmake_minimum_required(VERSION 3.5)

project(Application)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Copy the resources to the build directory.
file(COPY ./res DESTINATION "${CMAKE_BINARY_DIR}")

# Define all of the source files and executable.
set(SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Application.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Gui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Renderer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Window.cpp
    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageLoader.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Texture2D.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mesh.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Transform.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Scene.cpp
)
add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:SHELL:-g -Wall -Wextra -Wpedantic -Werror>"
    "$<$<CONFIG:Release>:SHELL:-O2 -DNDEBUG>" -std=c++20)

# Get the OpenGL library.
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# Set GLFW library option; no docs and no install.
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL "Generate installation target")
add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# Add GLAD.
add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

# Add GLM.
add_subdirectory(external/glm)
target_link_libraries(${PROJECT_NAME} glm)

# Add Assimp.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Generation of shared libs")
set(ASSIMP_INSTALL OFF CACHE BOOL "Install Assimp library.")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "Build test suite for Assimp.")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "Treat all warnings as errors." FORCE)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "Build all exporters.")
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "Build all importers.")
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "Build glTF importer.")
add_subdirectory(external/assimp)
target_link_libraries(${PROJECT_NAME} assimp::assimp)

# Add stb_image.
add_subdirectory(external/stb_image)
target_link_libraries(${PROJECT_NAME} stb_image)

# Add ImGUI as a static library.
set(IMGUI_PATH ${CMAKE_CURRENT_LIST_DIR}/external/imgui)
file(GLOB IMGUI_GLOB
    ${IMGUI_PATH}/imgui.h
    ${IMGUI_PATH}/imgui.cpp
    ${IMGUI_PATH}/imconfig.h
    ${IMGUI_PATH}/imgui_demo.cpp
    ${IMGUI_PATH}/imgui_draw.cpp
    ${IMGUI_PATH}/imgui_internal.h
    ${IMGUI_PATH}/imstb_rectpack.h
    ${IMGUI_PATH}/imstb_textedit.h
    ${IMGUI_PATH}/imstb_truetype.h
    ${IMGUI_PATH}/imgui_tables.cpp
    ${IMGUI_PATH}/imgui_widgets.cpp

    # specific bindings...
    ${IMGUI_PATH}/backends/imgui_impl_glfw.h
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.h
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3_loader.cpp)
add_library("imgui" STATIC ${IMGUI_GLOB})
target_include_directories("imgui" PUBLIC ${IMGUI_PATH})
target_link_libraries("imgui" PRIVATE glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI_PATH} ${IMGUI_PATH}/backends)
target_link_libraries(${PROJECT_NAME} imgui)

# Allows the use of the standard main() as entry point on Windows.
if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

if(MSVC)
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
        message("\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set '${PROJECT_NAME}' as StartUp Project in Visual Studio.\n")
    else()
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
    endif()
endif()
