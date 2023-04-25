// Include standard headers
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <vector>
// Include GLEW, GLFW, GLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// Include shader
#include <common/Shader.hpp>
#include <common/stb_image.hpp>
#define STB_IMAGE_IMPLEMENTATION
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
vector<unsigned int> loadTexturesFromFile(const vector<string> &paths);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 45.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// ****************************************
// ****************************************
// **               MAIN                 **
// ****************************************
// ****************************************
int main(void) {
  // ========================================
  //           INITIALIZE OPENGL
  // ========================================
  // Initialise GLFW
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    return -1;
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

  // Open a window and create its OpenGL context
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Grass Simulation!", NULL, NULL);

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    return -1;
  }

  // background
  glClearColor(0.215f, 0.215f, 0.215f, 1.0f);
  glfwSwapInterval(0);
  glEnable(GL_DEPTH_TEST);

  // ========================================
  //            CREATE SHADER
  // ========================================
  // Create and compile our GLSL program from the shaders
  GLuint shader = load_shaders("../../assets/shaders/grass.vs", "../../assets/shaders/grass.fs",
                               "../../assets/shaders/grass.gs");
  glUseProgram(shader);
  // projection matrix
  glm::mat4 projection =
      glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
  glUniformMatrix4fv(glGetUniformLocation(shader, "u_projection"), 1, GL_FALSE, &projection[0][0]);

  // view transformation
  glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  glUniformMatrix4fv(glGetUniformLocation(shader, "u_view"), 1, GL_FALSE, &view[0][0]);

  // ========================================
  //            CREATE POSITIONS
  // ========================================
  std::vector<glm::vec3> positions;
  for (float x = -10.0f; x < 10.0f; x += 0.06f) {
    for (float z = -10.0f; z < 10.0f; z += 0.06f) {
      positions.push_back(glm::vec3(x, 0, z));
    }
  }

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glUseProgram(shader);

  // ========================================
  //            GENERATE TEXTURE
  // ========================================
  vector<std::string> paths;
  paths.push_back("../../assets/textures/grass.png");
  paths.push_back("../../assets/textures/land2.png");
  vector<unsigned int> texture_ids = loadTexturesFromFile(paths);

  GLuint landShader = load_shaders("../../assets/shaders/land.vs", "../../assets/shaders/land.fs");
  glUseProgram(landShader);
  glUniformMatrix4fv(glGetUniformLocation(landShader, "u_projection"), 1, GL_FALSE,
                     &projection[0][0]);
  glUniformMatrix4fv(glGetUniformLocation(landShader, "u_view"), 1, GL_FALSE, &view[0][0]);

  float landVertices[] = {
      // Vertices           // Textures
      -10.0f, 0.0f, -10.0f, 0.0f, 0.0f, // down left
      10.0f,  0.0f, -10.0f, 1.0f, 0.0f, // down right
      -10.0f, 0.0f, 10.0f,  0.0f, 1.0f, // up left
      10.0f,  0.0f, 10.0f,  1.0f, 1.0f  // up right
  };

  float repeatFactor = 100.0f;
  for (int i = 0; i < 4; i++) {
    landVertices[i * 5 + 3] *= repeatFactor;
    landVertices[i * 5 + 4] *= repeatFactor;
  }

  unsigned int landVAO, landVBO;
  glGenVertexArrays(1, &landVAO);
  glBindVertexArray(landVAO);

  glGenBuffers(1, &landVBO);
  glBindBuffer(GL_ARRAY_BUFFER, landVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(landVertices), landVertices, GL_STATIC_DRAW);

  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // ========================================
  //            RENDER LOOP
  // ========================================
  glPointSize(5.0f);
  do {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    // input
    processInput(window);

    // reset color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind textures
    int count = 0;
    for (const auto &texture : texture_ids) {
      glActiveTexture(GL_TEXTURE0 + count);
      glBindTexture(GL_TEXTURE_2D, texture);
      if (count < texture_ids.size() - 1) {
        glUniform1i(glGetUniformLocation(shader, ("texture" + to_string(count)).c_str()), count);
      } else {
        glUniform1i(glGetUniformLocation(landShader, ("texture" + to_string(count)).c_str()),
                    count);
      }
      count += 1;
    }

    // update view
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // draw
    glUseProgram(landShader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "u_view"), 1, GL_FALSE, &view[0][0]);
    glUniform3fv(glGetUniformLocation(shader, "u_cameraPosition"), 1, &cameraPos[0]);
    glUniformMatrix4fv(glGetUniformLocation(landShader, "u_view"), 1, GL_FALSE, &view[0][0]);
    glUniform3fv(glGetUniformLocation(landShader, "u_cameraPosition"), 1, &cameraPos[0]);
    glBindVertexArray(landVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "u_view"), 1, GL_FALSE, &view[0][0]);
    glUniform3fv(glGetUniformLocation(shader, "u_cameraPosition"), 1, &cameraPos[0]);
    glUniformMatrix4fv(glGetUniformLocation(landShader, "u_view"), 1, GL_FALSE, &view[0][0]);
    glUniform3fv(glGetUniformLocation(landShader, "u_cameraPosition"), 1, &cameraPos[0]);
    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, positions.size());

    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    // it's dirty but it's ok to keep it simple
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  } while (!glfwWindowShouldClose(window));

  // ========================================
  //              CLEAN UP
  // ========================================
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader);

  glDeleteVertexArrays(1, &landVAO);
  glDeleteBuffers(1, &landVBO);
  glDeleteProgram(landShader);

  glfwTerminate();
  return 0;
}

// ****************************************
// ****************************************
// **            FUNCTIONS               **
// ****************************************
// ****************************************
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 2.5 * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    cameraPos += cameraUp * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    cameraPos -= cameraUp * cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
  fov -= (float)yoffset;
  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;
}

vector<unsigned int> loadTexturesFromFile(const vector<string> &paths) {
  vector<unsigned int> textures;

  for (const auto &path : paths) {
    std::string filename = path;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    stbi_set_flip_vertically_on_load(false);
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
      textures.push_back(texture);
    } else {
      std::cout << "Texture failed to load at path: " << path << std::endl;
    }

    stbi_image_free(data);
  }

  return textures;
}
