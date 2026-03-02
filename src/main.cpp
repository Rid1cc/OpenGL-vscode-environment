#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <cstdlib>
#include <iostream>

// Shaders (GLSL 4.1 — highest version supported on macOS)

static constexpr const char* VERTEX_SHADER = R"(
#version 410 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
out vec3 vColor;
void main() {
    vColor      = aColor;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

static constexpr const char* FRAGMENT_SHADER = R"(
#version 410 core
in  vec3 vColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(vColor, 1.0);
}
)";

// GL debug callback — only active in debug builds

static void GLAPIENTRY debugCallback(GLenum, GLenum type, GLuint, GLenum severity,
                                     GLsizei, const GLchar* msg, const void*) {
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
    std::cerr << "[GL 0x" << std::hex << type << "] " << msg << std::dec << '\n';
}

// Shader compilation + linking

static GLuint compileShader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);

    int ok;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        std::cerr << "Shader error: " << log << '\n';
    }
    return s;
}

static GLuint createProgram() {
    GLuint v = compileShader(GL_VERTEX_SHADER,   VERTEX_SHADER);
    GLuint f = compileShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);

    int ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(prog, sizeof(log), nullptr, log);
        std::cerr << "Link error: " << log << '\n';
    }
    glDeleteShader(v);
    glDeleteShader(f);
    return prog;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return EXIT_FAILURE;
    }

    // Request an OpenGL core profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int w, int h) {
        glViewport(0, 0, w, h);
    });

    // Load OpenGL functions
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version) {
        std::cerr << "Failed to load OpenGL\n";
        return EXIT_FAILURE;
    }
    std::cout << "OpenGL " << GLAD_VERSION_MAJOR(version) << '.'
              << GLAD_VERSION_MINOR(version)
              << " | " << glGetString(GL_RENDERER) << '\n';

#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(debugCallback, nullptr);
#endif

    // Triangle: position (x,y) + color (r,g,b) per vertex
    float vertices[] = {
         0.0f,  0.5f,   1.0f, 0.0f, 0.0f,   // top    — red
        -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   // left   — green
         0.5f, -0.5f,   0.0f, 0.0f, 1.0f,   // right  — blue
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    GLuint program = createProgram();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
