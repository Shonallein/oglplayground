#include <cassert>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "geometry.h"
#include "program.h"
#include "resources_path.h"

namespace
{

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

const char* vertSrc = R"src(
#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    TexCoord = texCoord;
}
)src";

const char* fragSrc = R"src(
#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, TexCoord);
} 
)src";

}

int main()
{
    // Init GLFW
    glfwInit();
    
    // Setup GLFW flags
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "OglPlayground", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Setup OpenGL Loader
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    // Setup GLFW callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    GLfloat vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    uint32_t indices[] = {
        // front
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
      10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
      20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
      30, 31, 32, 33, 34, 35
    };

    // Load texture
    int width, height, nbChannel;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* image = stbi_load(OglPlayground::resource_path("container.jpg"), &width, &height, &nbChannel, STBI_rgb);
    assert(nbChannel == 3);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);

    {
      // geomtery 
      const OglPlayground::VertexDesc vertDesc = {
        {OglPlayground::AttributeUsage::Position, 3},
        {OglPlayground::AttributeUsage::UV0, 2}
      };
      const size_t stride = strideFromVertexDesc(vertDesc);
      OglPlayground::Geometry geom(vertices, sizeof(vertices)/stride, indices, sizeof(indices)/sizeof(uint32_t), vertDesc);

      const OglPlayground::AttributeBindDesc attribDesc = {
        {OglPlayground::AttributeUsage::Position, 0},
        {OglPlayground::AttributeUsage::UV0, 1}
      };
      OglPlayground::GeometryBinder geomBinder(geom, attribDesc);

      // shader
      OglPlayground::Program program(vertSrc, fragSrc, [](const char* msg) { std::cerr << msg; });
      assert(program.isValid());
      for(const auto& desc : program.descs())
      {
        std::cout << "Uniform " << desc.name << " located at " << desc.location << std::endl;
      }

      // Game loop
      while (!glfwWindowShouldClose(window))
      {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);  
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program.use();

        glm::mat4 view;
        GLfloat radius = 10.0f;
        GLfloat camX = (GLfloat)sin(glfwGetTime()) * radius;
        GLfloat camZ = (GLfloat)cos(glfwGetTime()) * radius;
        view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 projection;
        int screenWidth, screenHeight;
        glfwGetWindowSize(window, &screenWidth, &screenHeight);
        projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

        std::vector<glm::vec3> cubePositions = {
          glm::vec3( 0.0f,  0.0f,  0.0f), 
          glm::vec3( 2.0f,  5.0f, -15.0f), 
          glm::vec3(-1.5f, -2.2f, -2.5f),  
          glm::vec3(-3.8f, -2.0f, -12.3f),  
          glm::vec3( 2.4f, -0.4f, -3.5f),  
          glm::vec3(-1.7f,  3.0f, -7.5f),  
          glm::vec3( 1.3f, -2.0f, -2.5f),  
          glm::vec3( 1.5f,  2.0f, -2.5f), 
          glm::vec3( 1.5f,  0.2f, -1.5f), 
          glm::vec3(-1.3f,  1.0f, -1.5f)  
        };

        glBindTexture(GL_TEXTURE_2D, texture);
        geomBinder.bind();
        for(int i = 0; i < cubePositions.size(); ++i)
        {
          glm::mat4 model;
          model = glm::translate(model, cubePositions[i]);
          model = glm::rotate(model, (GLfloat)glfwGetTime()*glm::radians(20.0f * (i+1)), glm::vec3(1.0f, 0.3f, 0.5f));
          glm::mat4 mvp = projection*view*model;
          glUniformMatrix4fv(program.desc("transform")->location, 1, GL_FALSE, glm::value_ptr(mvp));
          glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }
        geomBinder.unbind();

        // Swap the screen buffers
        glfwSwapBuffers(window);
      }
    }

    // Terminates GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}
