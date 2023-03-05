#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define FMT_HEADER_ONLY

#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION

#include "render.h"
#include "model.h"
#include "user_exception.hpp"

int main(int argc, char *argv[]) {
  try {
    Render render(512, 512);

    // load model by model name
    int model_class = 0;
    std::vector<Model> all_models;
//  std::string model_name[] = {"1"};
//  std::vector<std::string> model_name;
//  for (int i = 1; i<= 20; ++i) model_name.push_back(std::to_string(i));
//  for (auto &name: model_name) {
//    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off"));
//  }
//  for (auto &name: PsbDataset::GetAllModelName()) {
//    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off"));
//  }

//    for (auto &name: {4}) {
    for (auto &name: ShapeNetCoreDataset::GetModelNamesPerClass()[model_class]) {
      all_models.emplace_back(Model(fmt::format(R"({}{}\{}.obj)",
                                                ShapeNetCoreDataset::GetDataDir(),
                                                ShapeNetCoreDataset::GetModelClasses()[model_class], name)));
    }


    render.MeshProjectionRender(all_models, true);

//    render.MeshProjectionRender(all_models,false);

//    render.OneCameraRender();
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

