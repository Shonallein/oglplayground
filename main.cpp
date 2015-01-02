#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

#include "api.h"
#include <GLFW/glfw3.h>

#include "res_path.h"
#include "program.h"

void InitializeProgram();

namespace
{
void error_callback(int error, const char* description)
{
  std::cerr << "[ERROR] " << description << std::endl;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);

  if (key == GLFW_KEY_F5 && action == GLFW_PRESS)
    InitializeProgram();
}

void read_resource_content_(const char* filename, std::string& content)
{
  const std::string path =
      std::string(OglPlayground::RESOURCES_ROOT_PATH)+'/'+filename;
  std::ifstream file(path, std::ios::in);
  if(!file.is_open()) {
    std::cerr << "Can't open " << path << std::endl;
    return;
  }

  file.seekg(0, std::ios::end);
  content.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(&content[0], content.size());
  file.close();
}

// Globals
std::string root_path_;
std::unique_ptr<OglPlayground::Program> program_ = NULL;
}

void InitializeProgram()
{
  std::cout << "Load the shaders." << std::endl;
  program_.reset();
  OglPlayground::AttributeBinds attributes;
  std::string vs,ps;
  read_resource_content_("simple.vert", vs);
  read_resource_content_("simple.frag", ps);
  program_.reset(
      new OglPlayground::Program(
          vs.c_str(),
          ps.c_str(),
          attributes));
  if(!program_->isValid()) {
    std::cout << program_->errorString() << std::endl;
  }
}

const float vertexData[] = {
  0.0f,    0.5f, 0.0f, 1.0f,
  0.5f, -0.366f, 0.0f, 1.0f,
  -0.5f, -0.366f, 0.0f, 1.0f,
  1.0f,    0.0f, 0.0f, 1.0f,
  0.0f,    1.0f, 0.0f, 1.0f,
  0.0f,    0.0f, 1.0f, 1.0f,
};

GLuint positionBufferObject;
GLuint vao;


void InitializeVertexBuffer()
{
	glGenBuffers(1, &positionBufferObject);

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
	InitializeProgram();
	InitializeVertexBuffer();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

    OglPlayground::ProgramBinder binder(program_.get());

	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)48);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glUseProgram(0);
}

int main(int argc, char *argv[])
{
  root_path_ = argv[0];
  glfwSetErrorCallback(error_callback);

  if (!glfwInit())
	exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  GLFWwindow* window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
  if (!window) {
	glfwTerminate();
	exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);  
  glfwSetKeyCallback(window, key_callback);

  if(!OglPlayground::initApi()) {
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_FAILURE);
  }

  init();
  while (!glfwWindowShouldClose(window))
  {
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);
	display();
	glfwSwapBuffers(window);
	glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  exit(EXIT_SUCCESS);
}
