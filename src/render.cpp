// Created by lonelyinnovator on 2023/2/24 9:00.

#include "render.h"

Render::Render() {
  InitRender();
}

Render::Render(int multi_sample_num) {
  multi_sample_num_ = multi_sample_num;
  InitRender();
}

Render::Render(int width, int height, int multi_sample_num) {
  screen_width_ = width;
  screen_height_ = height;
  multi_sample_num_ = multi_sample_num;

  last_x_ = static_cast<float>(screen_width_) / 2.0f;
  last_y_ = static_cast<float>(screen_height_) / 2.0f;

  InitRender();
}

Render::~Render() {

}

void Render::OneCameraRender() {

  glfwSetCursorPosCallback(window_, OnMouseMove);
  glfwSetScrollCallback(window_, OnMouseScroll);

  // set glfw to capture mouse
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // shader
  Shader model_shader = Shader(R"(shader\shaded.vs)", R"(shader\shaded.fs)");

  // load model by model name
  std::string model_name[] = {"21"};
  std::vector<Model> all_models;
  for (auto &name: model_name) {
    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off"));
  }
//    for (auto &name: PsbDataset::GetAllModelName()) {
//        allModels.emplace_back(Model(PsbDataset::ModelDir + name + ".off"));
//    }

  // traverse all the models
  for (int i = 0; i < all_models.size(); ++i) {
    while (!glfwWindowShouldClose(window_)) {

      // per frame time logic
      auto current_frame = static_cast<float>(glfwGetTime());
      delta_time_ = current_frame - last_frame_;
      last_frame_ = current_frame;

      ProcessInput(window_);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      model_shader.use();

      // set material
      glm::vec3 material_color = Color::White;
      model_shader.setVec3("material.ambient", material_color);
      model_shader.setVec3("material.diffuse", material_color);
      model_shader.setVec3("material.specular", material_color);
      model_shader.setFloat("material.shininess", 32.0f);
      // set light
      model_shader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
      model_shader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
      model_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
      model_shader.setVec3("dirLight.direction", camera_.GetFront());
      // set view position
      model_shader.setVec3("viewPos", camera_.GetPosition());

      // view/projection transformations
      glm::mat4 projection = glm::perspective(glm::radians(camera_.GetZoom()),
                                              screen_width_ * 1.0f / screen_height_,
                                              0.1f, 100.0f);
      glm::mat4 view = camera_.GetViewMatrix();
      model_shader.setMat4("projection", projection);
      model_shader.setMat4("view", view);

      // render the loaded model
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      model_shader.setMat4("model", model);
      all_models[i].Draw(model_shader);
      // normal matrix
      glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));
      model_shader.setMat3("normalMatrix", normalMatrix);

      glfwSwapBuffers(window_);
      glfwPollEvents();
    }
  }

  glDeleteProgram(model_shader.ID);
}

void Render::MeshProjectionRender(bool shaded) {

//    glfwSetCursorPosCallback(window, mouse_move_callback);
//    glfwSetScrollCallback(window, mouse_scroll_callback);

  // set glfw to capture mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // shader
  Shader model_shader = Shader(R"(shader\shaded.vs)", R"(shader\shaded.fs)");
  if (!shaded) {
    model_shader = Shader(R"(shader\depth_map.vs)", R"(shader\depth_map.fs)");
  }

  // load model by model name
  std::string model_name[] = {"21"};
  std::vector<Model> all_models;
  for (auto &name: model_name) {
    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off"));
  }
//    for (auto &name: PsbDataset::GetAllModelName()) {
//        allModels.emplace_back(Model(PsbDataset::ModelDir + name + ".off"));
//    }

  MeshProjection meshProjection;

  // traverse all the models
  for (int i = 0; i < all_models.size(); ++i) {
    // repeat render every camera repeat_num times
    int cnt = 1, repeat_num = 2;
    while (!glfwWindowShouldClose(window_)) {
      // get mesh projection camera
      if (cnt % (repeat_num + 1) == 0) {
        cnt = 1;
        auto success = meshProjection.GetNextProjCamera(camera_);
        if (!success) {
          meshProjection.ResetCurrentCameraIndex();
//                glfwSetWindowShouldClose(window, true);
          break;
        }
      } else {
        cnt++;
      }

      // per frame time logic
      auto current_frame = static_cast<float>(glfwGetTime());
      delta_time_ = current_frame - last_frame_;
      last_frame_ = current_frame;

//            process_input(window);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      model_shader.use();

      // set material
      glm::vec3 material_color = Color::White;
      model_shader.setVec3("material.ambient", material_color);
      model_shader.setVec3("material.diffuse", material_color);
      model_shader.setVec3("material.specular", material_color);
      model_shader.setFloat("material.shininess", 32.0f);
      // set light
      model_shader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
      model_shader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
      model_shader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
      model_shader.setVec3("dirLight.direction", camera_.GetFront());
      // set view position
      model_shader.setVec3("viewPos", camera_.GetPosition());

      // view/projection transformations
      glm::mat4 projection = glm::perspective(glm::radians(camera_.GetZoom()),
                                              screen_width_ * 1.0f / screen_height_,
                                              0.1f, 100.0f);
      glm::mat4 view = camera_.GetViewMatrix();
      model_shader.setMat4("projection", projection);
      model_shader.setMat4("view", view);

      // render the loaded model
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      model_shader.setMat4("model", model);
      all_models[i].Draw(model_shader);
      // normal matrix
      glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));
      model_shader.setMat3("normalMatrix", normalMatrix);

      // compute depth min max when use DepthMap shader
      if (!shaded) {
        ComputeDepthMinMax(model_shader, all_models[i], projection, view, model);
      }

      glfwSwapBuffers(window_);
      glfwPollEvents();

      // snapshot
      if (shaded) {
        meshProjection.ProjSnapshot(PsbDataset::GetImageDir() + all_models[i].GetName(), "shaded",
                                    screen_width_, screen_height_, false);
      } else {
        meshProjection.ProjSnapshot(PsbDataset::GetImageDir() + all_models[i].GetName(), "depth",
                                    screen_width_, screen_height_, false);
      }
    }
  }

  glDeleteProgram(model_shader.ID);
}

void Render::ShadedSegRender() {
  // shader
  Shader modelShader = Shader(R"(shader\shaded_seg.vs)",
                              R"(shader\shaded_seg.fs)");

  // load model and seg
  int seg_num = 0;
//    std::string model_name[] = {"180"};
  std::vector<Model> allModels;
//    for (auto &name: model_name) {
//        std::string model_path = PsbDataset::ModelDir + name + ".off";
//        std::string seg_path = std::format(R"({0}{1}\{1}_{2}.seg)", PsbDataset::SegDir, name, seg_num);
//        allModels.emplace_back(Model(model_path, seg_path));
//    }
  for (auto &name: PsbDataset::GetAllModelName()) {
    std::string model_path = PsbDataset::GetModelDir() + name + ".off";
    std::string seg_path = std::format(R"({0}{1}\{1}_{2}.seg)", PsbDataset::GetSegDir(), name, seg_num);
    allModels.emplace_back(Model(model_path, seg_path));
  }

  MeshProjection meshProjection;

  for (int i = 0; i < allModels.size(); ++i) {
//        glfwSetWindowShouldClose(window, false);
    int cnt = 1, repeat_num = 2;
    while (!glfwWindowShouldClose(window_)) {
      // get mesh projection camera
      if (cnt % (repeat_num + 1) == 0) {
        cnt = 1;
        auto success = meshProjection.GetNextProjCamera(camera_);
        if (!success) {
          meshProjection.ResetCurrentCameraIndex();
          break;
        }
      } else {
        cnt++;
      }

      // per frame time logic
      auto current_frame = static_cast<float>(glfwGetTime());
      delta_time_ = current_frame - last_frame_;
      last_frame_ = current_frame;
//            process_input(window);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      modelShader.use();

      // set all colors
      for (int j = 0; j < Color::GetAllColorsSize(); ++j) {
        glm::vec3 tmp(Color::AllColors[j]);
        modelShader.setVec3(std::format("allColors[{}]", j), tmp);
      }

      // set material
//            glm::vec3 material_color = Color::Maroon;
      glm::vec3 material_color = Color::White;
      modelShader.setVec3("material.ambient", material_color);
      modelShader.setVec3("material.diffuse", material_color);
      modelShader.setVec3("material.specular", material_color);
      modelShader.setFloat("material.shininess", 32.0f);
      // set light
      modelShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
      modelShader.setVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
      modelShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
      modelShader.setVec3("dirLight.direction", camera_.GetFront());
      // set view position
      modelShader.setVec3("viewPos", camera_.GetPosition());

      // view/projection transformations
      glm::mat4 projection = glm::perspective(glm::radians(camera_.GetZoom()), kScreenWidth * 1.0f / kScreenHeight,
                                              0.1f, 100.0f);
      glm::mat4 view = camera_.GetViewMatrix();
      modelShader.setMat4("projection", projection);
      modelShader.setMat4("view", view);

      // render the loaded model
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      modelShader.setMat4("model", model);
      allModels[i].Draw(modelShader);
      // normal matrix
      glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));
      modelShader.setMat3("normalMatrix", normalMatrix);

      // set max seg class
      modelShader.setInt("maxSegClass", allModels[i].GetMaxSegClass());

      glfwSwapBuffers(window_);
      glfwPollEvents();

      meshProjection.ProjSnapshot(PsbDataset::GetImageDir() + allModels[i].GetName(), "gt",
                                  kScreenWidth, kScreenHeight, true);
    }
  }
}

const Camera &Render::GetCamera() {
  return camera_;
}

void Render::SetCamera(const Camera &camera) {
  camera_ = camera;
}

int Render::GetScreenWidth() const {
  return screen_width_;
}

int Render::GetScreenHeight() const {
  return screen_height_;
}

GLFWwindow *Render::GetWindow() const {
  return window_;
}

int Render::GetMultiSampleNum() const {
  return multi_sample_num_;
}


void Render::OnFramebufferSizeChange(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void Render::ProcessInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera_.ProcessKeyboard(CameraMovement::kForward, delta_time_);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera_.ProcessKeyboard(CameraMovement::kBackward, delta_time_);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera_.ProcessKeyboard(CameraMovement::kLeft, delta_time_);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera_.ProcessKeyboard(CameraMovement::kRight, delta_time_);
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    camera_.ProcessKeyboard(CameraMovement::kUp, delta_time_);
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera_.ProcessKeyboard(CameraMovement::kDown, delta_time_);
}

void Render::OnMouseMove(GLFWwindow *window, double pos_x_in, double pos_y_in) {
  auto pos_x = static_cast<float>(pos_x_in);
  auto pos_y = static_cast<float>(pos_y_in);

  if (first_mouse_) {
    last_x_ = pos_x;
    last_y_ = pos_y;
    first_mouse_ = false;
  }

  float offset_x = pos_x - last_x_;
  float offset_y = last_y_ - pos_y;

  last_x_ = pos_x;
  last_y_ = pos_y;

  camera_.ProcessMouseMovement(offset_x, offset_y);
}

void Render::OnMouseScroll(GLFWwindow *window, double offset_x, double offset_y) {
  // offset_y: mouse scroll vertically
  camera_.ProcessMouseScroll(static_cast<float>(offset_y));
}

void Render::InitRender() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // N multi sample
  glfwWindowHint(GLFW_SAMPLES, multi_sample_num_);

  // create glfw window
  // -----------------------------
  window_ = glfwCreateWindow(screen_width_, screen_height_, "mesh", nullptr, nullptr);
  if (window_ == nullptr) {
    std::cout << "Failed to create window" << std::endl;
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window_);

  glfwSetFramebufferSizeCallback(window_, OnFramebufferSizeChange);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "Failed to init glad" << std::endl;
    return;
  }

  // set opengl viewport
  glViewport(0, 0, screen_width_, screen_height_);

  // enable z-buffer
  glEnable(GL_DEPTH_TEST);

  // enable msaa
  if (multi_sample_num_ != 0) {
    glEnable(GL_MULTISAMPLE);
  }
}


void Render::ComputeDepthMinMax(Shader &shader, Model &ourModel, glm::mat4 &projection, glm::mat4 &view,
                                glm::mat4 &model) {
  float zMin = 0.0f, zMax = 0.0f;
  for (int i = 0; i < ourModel.GetMeshes()[0].GetVertices().size(); ++i) {
    auto &v = ourModel.GetMeshes()[0].GetVertices()[i].position;
    auto p = projection * view * model * glm::vec4(v, 1.0f);
    auto z = p.z / p.w;
    if (i == 0) {
      zMin = z;
      zMax = z;
    } else {
      zMin = std::min(zMin, z);
      zMax = std::max(zMax, z);
    }
  }
  shader.setFloat("zMin", zMin);
  shader.setFloat("zMax", zMax);
  shader.setFloat("grayMin", 0.2f);
  shader.setFloat("grayMax", 0.8f);
}


float Render::delta_time_ = 0.0f;
float Render::last_frame_ = 0.0f;

bool Render::first_mouse_ = true;
float Render::last_x_ = kScreenWidth / 2.0f;
float Render::last_y_ = kScreenHeight / 2.0f;

Camera Render::camera_{};