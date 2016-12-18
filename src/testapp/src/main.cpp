#include <iostream>
#include <map>

#include <docopt.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "application.h"
#include "gameoflifebehavior.h"
#include "testbehavior.h"

namespace
{
static const char USAGE[] =
R"(TestApp

    Usage:
      testapp test
      testapp gol
      testapp (-h | --help)
      testapp --version

    Options:
      -h --help     Show this screen.
      --version     Show version.
)";
}

int main(int argc, const char** argv)
{
  std::map<std::string, docopt::value> args = 
      docopt::docopt(
          USAGE,
          { argv + 1, argv + argc },
          true,
          "TestApp 1.0");

  std::unique_ptr<TestApp::Behavior> behavior;
  if(args["test"].asBool()) {
    behavior.reset(new TestBehavior());
  } else if(args["gol"].asBool()) {
    behavior.reset(new GameOfLifeBehavior());
  }
  if(behavior == nullptr) return 1;
  
  TestApp::Application application;
  application.run(behavior.get());

  return 0;
}
