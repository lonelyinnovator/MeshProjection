#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "render.h"
#include "user_exception.hpp"

int main(int argc, char *argv[]) {

//    if (std::filesystem::exists(image_dir)) {
//        std::filesystem::remove_all(image_dir);
//    }

  try {
    Render render;
    render.OneCameraRender();

//    render shaded
//    render.MeshProjectionRender(true);
//    render depth map
//    render.MeshProjectionRender(false);

//    render.ShadedSegRender();
  }
  catch (const UserException &e) {
    std::cout << "render error!" << e.what() << std::endl;
  }
  catch (const std::exception &e) {
    std::cout << "render unknown error! " << e.what() << std::endl;
  }

  glfwTerminate();
  return 0;
}

