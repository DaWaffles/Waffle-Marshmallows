#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <sstream>
#include <fstream>
#include <string>


//cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
//cmake --build build
//./build/my_project

void framebuffer_size_callback(GLFWwindow* window, int width, int height);  
void processInput(GLFWwindow *window);

const float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};  

 GLuint compileShader(GLenum type, const std::string& filename) {
    const char* src = nullptr;
    std::string content;

    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cout << "ERROR::SHADER::FILE_NOT_FOUND: " << filename << "\n";
      return 0;
    }

    std::stringstream buffer;

    buffer << file.rdbuf();
    content = buffer.str();

    file.close();
    src = content.c_str();

    if (src==nullptr) return 0; // Safety check

    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    // Error checking
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << filename << "\n" << infoLog << "\n";
        return 0;
    }

    return shader;
}

 bool loadShaders(GLuint& program) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, "vertex_core.glsl");
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, "fragment_core.glsl");

    if (vertexShader == 0 || fragmentShader == 0) return false;

    // Link Program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);


    // Check Linking
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
        return false;
    }

    return true;
}

GLFWwindow* initWindow(int width, int height, const char* title) {
    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // 2. Set context and callbacks
    glfwMakeContextCurrent(window);

    // 3. Initialize GLAD
    // Note: GLAD must be loaded after the context is made current
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }
    return window;
}

int main() {
  glfwInit();
  
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = initWindow(800, 600, "WaffleMallows");
  
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << '\n';
    return -1;
  }

  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  GLuint shaderProgram = glCreateProgram();
  if (!loadShaders(shaderProgram)) {
      std::cout << "Failed to load shaders" << '\n';
      glfwTerminate();
      return -1;
  }

  // --- Vertex Data Setup ---
  GLuint VAO, VBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // VAO
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    glClearColor(0.392f, 0.584f, 0.929f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();

  std::cout << "Program End" << '\n';

  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
