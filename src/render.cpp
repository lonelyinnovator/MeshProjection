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

Render::~Render() = default;


void Render::RasterizationFromScratch(const Model &one_model, const glm::mat4 &total_matrix,
                                      const MeshProjection &mp) const {
  std::vector<Vertex> mesh_vertices;
  int min_h, max_h, min_w, max_w;
  float tmp;
  for (auto &mesh: one_model.GetMeshes()) {
    std::vector<std::vector<int>> tri_per_pixel(screen_height_, std::vector<int>(screen_width_, -1));
    std::vector<std::vector<float>> depth(screen_height_, std::vector<float>(screen_width_, 1.0f));
    mesh_vertices = mesh.GetVertices();
    // total transformations
    for (auto &vertex: mesh_vertices) {
      glm::vec4 p = total_matrix * glm::vec4(vertex.position, 1.0f);
      vertex.position = glm::vec3(p / p.w);
      tmp = vertex.position.x;
//      vertex.position.x = static_cast<float>(screen_height_) - round(vertex.position.y);
      vertex.position.x = static_cast<float>(screen_height_) - vertex.position.y - 1;
//      vertex.position.y = round(tmp);
      vertex.position.y = tmp;
//      std::cout << vertex.position.z << std::endl;
    }
    auto mesh_triangles = mesh.GetTriangles();
    // rasterization
    for (int k = 0; k < mesh_triangles.size(); ++k) {
      auto v1 = mesh_vertices[mesh_triangles[k].vertex_idx1].position;
      auto v2 = mesh_vertices[mesh_triangles[k].vertex_idx2].position;
      auto v3 = mesh_vertices[mesh_triangles[k].vertex_idx3].position;
      min_h = static_cast<int>(round(std::min({v1.x, v2.x, v3.x})));
      max_h = static_cast<int>(round(std::max({v1.x, v2.x, v3.x})));
      min_w = static_cast<int>(round(std::min({v1.y, v2.y, v3.y})));
      max_w = static_cast<int>(round(std::max({v1.y, v2.y, v3.y})));
//        std::cout << std::format("{} {} {} {}", min_h, max_h, min_w, max_w) << "; ";
      std::vector<glm::vec2> tv{glm::vec2(v1), glm::vec2(v2), glm::vec2(v3)};
      for (int i = min_h; i <= max_h; ++i) {
        for (int j = min_w; j <= max_w; ++j) {
          glm::vec2 point = glm::vec2(i, j);
          if (GeneralAlgorithm::IsInsideTriangle(point, tv)) {
            auto [alpha, beta, gamma] = GeneralAlgorithm::ComputeBaryCentric2d(point, tv);
            float z = alpha * v1.z + beta * v2.z + gamma * v3.z;
            if (depth[i][j] > z) {
              tri_per_pixel[i][j] = k;
              depth[i][j] = z;
            }
          }
        }
      }
    }
    // write tri_per_pixel to csv file
    std::string file_dir = PsbDataset::GetPixelDir() + one_model.GetName() + R"(\)";
    if (!std::filesystem::exists(file_dir)) {
      std::filesystem::create_directories(file_dir);
    }
    FileProcess file_process(std::format("{}pixel_{}.csv", file_dir, mp.GetCameraViewFilename()), std::ios::out);
    for (int i = 0; i < screen_height_; ++i) {
      for (int j = 0; j < screen_width_; ++j) {
        if (j) file_process.Write(",");
        file_process.Write(std::to_string(tri_per_pixel[i][j]));
      }
      file_process.Write("\n");
    }
    file_process.CloseFile();
  }

}

void Render::OneCameraRender() {

  glfwSetCursorPosCallback(window_, OnMouseMove);
  glfwSetScrollCallback(window_, OnMouseScroll);

  // set glfw to capture mouse
  glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // shader
  Shader model_shader = Shader(R"(shader\shaded.vert)", R"(shader\shaded.frag)");

  // load model by model name
  std::string model_name[] = {"1"};
  std::vector<Model> all_models;
  int seg_num = 0;
  for (auto &name: model_name) {
    std::string seg_path = std::format(R"({0}{1}\{1}_{2}.seg)", PsbDataset::GetSegDir(), name, seg_num);
    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off", seg_path));
  }
//    for (auto &name: PsbDataset::GetAllModelName()) {
//        allModels.emplace_back(Model(PsbDataset::ModelDir + name + ".off"));
//    }

  // mvp matrix
  glm::mat4 projection(1.0f), view(1.0f), model(1.0f), model_view_projection(1.0f);

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

      model_shader.Use();

      // set material
      glm::vec3 material_color = Color::White;
      model_shader.SetVec3("material.ambient", material_color);
      model_shader.SetVec3("material.diffuse", material_color);
      model_shader.SetVec3("material.specular", material_color);
      model_shader.SetFloat("material.shininess", 32.0f);
      // set light
      model_shader.SetVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
      model_shader.SetVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
      model_shader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
      model_shader.SetVec3("dirLight.direction", camera_.GetFront());
      // set view position
      model_shader.SetVec3("viewPos", camera_.GetPosition());

      // view/projection transformations
      projection = glm::perspective(glm::radians(camera_.GetZoom()),
                                    static_cast<float>(screen_width_) / static_cast<float>(screen_height_),
                                    0.1f, 100.0f);
      view = camera_.GetViewMatrix();
      // model transformations
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      // normal matrix
      glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));
      model_shader.SetMat3("normalMatrix", normalMatrix);
      // set mvp matrix
      model_view_projection = projection * view * model;
      model_shader.SetMat4("modelViewProjection", model_view_projection);

      // draw model
      all_models[i].Draw(model_shader);

      float w_min = static_cast<float>(screen_width_), w_max = -1;
      float h_min = static_cast<float>(screen_height_), h_max = -1;
      float x, y;

      FileProcess file_process(PsbDataset::GetDataDir() + "test.csv", std::ios::out);
      file_process.Write("x,y\n");
      for (auto &mesh: all_models[i].GetMeshes()) {
//        Vertex v = mesh.GetVertices()[100];
        for (auto &v: mesh.GetVertices()) {
          glm::vec4 p = viewport_matrix_ * model_view_projection * glm::vec4(v.position, 1.0f);
          p /= p.w;
          x = round(p.x);
          y = round(p.y);
          w_min = std::min(w_min, x);
          h_min = std::min(h_min, x);
          w_max = std::max(w_max, y);
          h_max = std::max(h_max, y);
//          std::cout << std::format("{} {} {}", x, y, p.z) << std::endl;
//          ofs << std::format("{},{}", static_cast<float>(screen_height_) - y, x) << std::endl;
          file_process.Write(std::format("{},{}\n", static_cast<float>(screen_height_) - y, x));
        }
      }
      file_process.CloseFile();

//      std::cout << std::format("w_min: {}, w_max: {}, h_min: {}, h_max:{}", w_min, w_max, h_min, h_max) << std::endl;

      glfwSwapBuffers(window_);
      glfwPollEvents();

      Camera::Snapshot("test.png", PsbDataset::GetDataDir(), screen_width_, screen_height_, true);
//      break;
    }
  }
  for (auto &m: all_models) {
    m.DeleteAllMeshesObject();
  }
  glDeleteProgram(model_shader.GetProgramId());
}

void Render::MeshProjectionRender(bool shaded) {

//    glfwSetCursorPosCallback(window, mouse_move_callback);
//    glfwSetScrollCallback(window, mouse_scroll_callback);

  // set glfw to capture mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // shader
  Shader model_shader = Shader(R"(shader\shaded.vert)", R"(shader\shaded.frag)");
  if (!shaded) {
    model_shader = Shader(R"(shader\depth_map.vert)", R"(shader\depth_map.frag)");
  }

  // load model by model name
  std::vector<Model> all_models;
//  std::string model_name[] = {"1"};
//  for (auto &name: model_name) {
//    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off"));
//  }
  for (auto &name: PsbDataset::GetAllModelName()) {
    all_models.emplace_back(Model(PsbDataset::GetModelDir() + name + ".off"));
  }

  MeshProjection mesh_projection;

  // mvp matrix
  glm::mat4 projection(1.0f), view(1.0f), model(1.0f), model_view_projection(1.0f);

  // traverse all the models
  for (int i = 0; i < all_models.size(); ++i) {
    // repeat render every camera repeat_num times
//    int cnt = 1, repeat_num = 2;
    while (!glfwWindowShouldClose(window_)) {
//      // get mesh projection camera
//      if (cnt % (repeat_num + 1) == 0) {
//        cnt = 1;
//        auto success = mesh_projection.GetNextProjCamera(camera_);
//        if (!success) {
//          mesh_projection.ResetCurrentCameraIndex();
////                glfwSetWindowShouldClose(window, true);
//          break;
//        }
//      } else {
//        cnt++;
//      }
      auto success = mesh_projection.GetNextProjCamera(camera_);
      // render finished
      if (!success) {
        mesh_projection.ResetCurrentCameraIndex();
        break;
      }
      // avoid OpenGL render too quickly
      if (mesh_projection.GetCurrentCameraIndex() == -1) {
        mesh_projection.ResetCurrentCameraIndex();
        continue;
      }

      // per frame time logic
      auto current_frame = static_cast<float>(glfwGetTime());
      delta_time_ = current_frame - last_frame_;
      last_frame_ = current_frame;

//      process_input(window_);

      glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      model_shader.Use();

      // set material
      glm::vec3 material_color = Color::White;
      model_shader.SetVec3("material.ambient", material_color);
      model_shader.SetVec3("material.diffuse", material_color);
      model_shader.SetVec3("material.specular", material_color);
      model_shader.SetFloat("material.shininess", 32.0f);
      // set light
      model_shader.SetVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
      model_shader.SetVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
      model_shader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
      model_shader.SetVec3("dirLight.direction", camera_.GetFront());
      // set view position
      model_shader.SetVec3("viewPos", camera_.GetPosition());

      // view/projection transformations
      projection = glm::perspective(glm::radians(camera_.GetZoom()),
                                    static_cast<float>(screen_width_) / static_cast<float>(screen_height_),
                                    0.1f, 100.0f);
      view = camera_.GetViewMatrix();
      // model transformations
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      // normal matrix
      glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));
      model_shader.SetMat3("normalMatrix", normalMatrix);
      // set mvp matrix
      model_view_projection = projection * view * model;
      model_shader.SetMat4("modelViewProjection", model_view_projection);

      all_models[i].Draw(model_shader);

      // compute depth min max when use DepthMap shader
      if (!shaded) {
        ComputeDepthMinMax(model_shader, all_models[i], projection, view, model);
      }

      glfwSwapBuffers(window_);
      glfwPollEvents();

      // compute and save triangle id on every pixel
      RasterizationFromScratch(all_models[i], viewport_matrix_ * model_view_projection, mesh_projection);

      // snapshot
//      if (shaded) {
//        mesh_projection.ProjSnapshot(PsbDataset::GetImageDir() + all_models[i].GetName(), "shaded",
//                                     screen_width_, screen_height_, false);
//      } else {
//        mesh_projection.ProjSnapshot(PsbDataset::GetImageDir() + all_models[i].GetName(), "depth",
//                                     screen_width_, screen_height_, false);
//      }
    }
  }
  for (auto &m: all_models) {
    m.DeleteAllMeshesObject();
  }
  glDeleteProgram(model_shader.GetProgramId());
}

void Render::ShadedSegRender() {
  // shader
  Shader model_shader = Shader(R"(shader\shaded_seg.vert)",
                               R"(shader\shaded_seg.frag)");

  // load model and seg
  int seg_num = 0;
  std::string model_name[] = {"21"};
  std::vector<Model> all_models;
  for (auto &name: model_name) {
    std::string model_path = PsbDataset::GetModelDir() + name + ".off";
    std::string seg_path = std::format(R"({0}{1}\{1}_{2}.seg)", PsbDataset::GetSegDir(), name, seg_num);
    all_models.emplace_back(Model(model_path, seg_path));
  }
//  for (auto &name: PsbDataset::GetAllModelName()) {
//    std::string model_path = PsbDataset::GetModelDir() + name + ".off";
//    std::string seg_path = std::format(R"({0}{1}\{1}_{2}.seg)", PsbDataset::GetSegDir(), name, seg_num);
//    all_models.emplace_back(Model(model_path, seg_path));
//  }

  MeshProjection mesh_projection;

  // mvp matrix
  glm::mat4 projection(1.0f), view(1.0f), model(1.0f), model_view_projection(1.0f);

  for (int i = 0; i < all_models.size(); ++i) {
    int cnt = 1, repeat_num = 2;
    while (!glfwWindowShouldClose(window_)) {
      // get mesh projection camera
      if (cnt % (repeat_num + 1) == 0) {
        cnt = 1;
        auto success = mesh_projection.GetNextProjCamera(camera_);
        if (!success) {
          mesh_projection.ResetCurrentCameraIndex();
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

      model_shader.Use();

      // set all colors
      for (int j = 0; j < Color::GetAllColorsSize(); ++j) {
        glm::vec3 tmp(Color::AllColors[j]);
        model_shader.SetVec3(std::format("allColors[{}]", j), tmp);
      }

      // set material
//            glm::vec3 material_color = Color::Maroon;
      glm::vec3 material_color = Color::White;
      model_shader.SetVec3("material.ambient", material_color);
      model_shader.SetVec3("material.diffuse", material_color);
      model_shader.SetVec3("material.specular", material_color);
      model_shader.SetFloat("material.shininess", 32.0f);
      // set light
      model_shader.SetVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
      model_shader.SetVec3("dirLight.diffuse", 0.6f, 0.6f, 0.6f);
      model_shader.SetVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
      model_shader.SetVec3("dirLight.direction", camera_.GetFront());
      // set view position
      model_shader.SetVec3("viewPos", camera_.GetPosition());

      // view/projection transformations
      projection = glm::perspective(glm::radians(camera_.GetZoom()),
                                    static_cast<float>(screen_width_) / static_cast<float>(screen_height_),
                                    0.1f, 100.0f);
      view = camera_.GetViewMatrix();
      // model transformations
      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
      // normal matrix
      glm::mat3 normalMatrix(glm::transpose(glm::inverse(model)));
      model_shader.SetMat3("normalMatrix", normalMatrix);
      // set mvp matrix
      model_view_projection = projection * view * model;
      model_shader.SetMat4("modelViewProjection", model_view_projection);

      // set max seg class
      model_shader.SetInt("maxSegClass", all_models[i].GetMaxSegClass());

      all_models[i].Draw(model_shader);

      glfwSwapBuffers(window_);
      glfwPollEvents();

      mesh_projection.ProjSnapshot(PsbDataset::GetImageDir() + all_models[i].GetName(), "gt",
                                   kScreenWidth, kScreenHeight, true);
    }
  }
  for (auto &m: all_models) {
    m.DeleteAllMeshesObject();
  }
  glDeleteProgram(model_shader.GetProgramId());
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

glm::mat4 &Render::GetViewportMatrix() {
  return viewport_matrix_;
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

  // set viewport matrix with screen width and screen height
  viewport_matrix_ = {
      static_cast<float>(screen_width_) / 2.0f, 0.0f, 0.0f, 0.0f,
      0.0f, static_cast<float>(screen_height_) / 2.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      (static_cast<float>(screen_width_) - 1.0f) / 2.0f, (static_cast<float>(screen_height_) - 1.0f) / 2.0f, 0.0f, 1.0f
  };

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
  shader.SetFloat("zMin", zMin);
  shader.SetFloat("zMax", zMax);
  shader.SetFloat("grayMin", 0.2f);
  shader.SetFloat("grayMax", 0.8f);
}


float Render::delta_time_ = 0.0f;
float Render::last_frame_ = 0.0f;

bool Render::first_mouse_ = true;
float Render::last_x_ = kScreenWidth / 2.0f;
float Render::last_y_ = kScreenHeight / 2.0f;

Camera Render::camera_{};

