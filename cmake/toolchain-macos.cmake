# macOS Toolchain (Apple Clang)
#
# Apple deprecated OpenGL in macOS 10.14 but it still works up to 4.1 core.

set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version")
add_compile_definitions(GL_SILENCE_DEPRECATION)
