cmake_minimum_required(VERSION 3.20)
project(OpenGLApp LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DEPS_DIR ${CMAKE_SOURCE_DIR}/deps)

# GLAD 2 – OpenGL loader
set(GLAD_INCLUDE_DIR ${DEPS_DIR}/glad/include)
set(GLAD_SRC         ${DEPS_DIR}/glad/src/gl.c)

# GLM – header-only math library
set(GLM_INCLUDE_DIR ${DEPS_DIR})

# --- GLFW (platform-specific) ------------------------------------------------

if(APPLE)
    find_package(glfw3 3.3 REQUIRED)
    set(GLFW_TARGET glfw)
    set(OPENGL_LIBS "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")

elseif(WIN32)
    set(GLFW_INCLUDE_DIR ${DEPS_DIR}/glfw/include)
    set(GLFW_LIB_DIR     ${DEPS_DIR}/glfw/lib)

    add_library(glfw_win STATIC IMPORTED)
    set_target_properties(glfw_win PROPERTIES
        IMPORTED_LOCATION             ${GLFW_LIB_DIR}/libglfw3.a
        INTERFACE_INCLUDE_DIRECTORIES ${GLFW_INCLUDE_DIR}
    )
    set(GLFW_TARGET glfw_win)
    set(OPENGL_LIBS opengl32 gdi32 user32 shell32)

else()
    find_package(glfw3 3.3 REQUIRED)
    set(GLFW_TARGET glfw)
    set(OPENGL_LIBS GL dl m)
endif()

# --- Libraries ----------------------------------------------------------------

add_library(glad STATIC ${GLAD_SRC})
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})

# --- Application --------------------------------------------------------------

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    glad
    ${GLFW_TARGET}
    ${OPENGL_LIBS}
)

# --- Copy runtime DLLs (Windows only) ----------------------------------------

if(WIN32)
    foreach(_dll
            ${GLFW_LIB_DIR}/glfw3.dll
            ${MINGW64_DLL_DIR}/libstdc++-6.dll
            ${MINGW64_DLL_DIR}/libgcc_s_seh-1.dll
            ${MINGW64_DLL_DIR}/libwinpthread-1.dll)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${_dll}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach()
endif()

