// Created by lonelyinnovator on 2023/2/24 9:00.
/**
 * Render the OpenGL window
 */

#ifndef MESHPROJECTION_RENDER_H
#define MESHPROJECTION_RENDER_H

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "mesh_projection.h"
#include "camera.h"
#include "model.h"
#include "psbdataset.h"
#include "color.hpp"
#include "file_process.h"
#include "shapenetcore_dataset.h"

// opengl screen width
const int kScreenWidth = 800;
// opengl screen height
const int kScreenHeight = 800;


/**
 * OpenGL Render.
 * Examples:
 *   Render render;
 *   render.OneCameraRender();
 */
class Render {
 public:
  /**
   * Default render constructor.
   */
  Render();

  /**
   * Set multi sample num(msaa) to render.
   * @param multi_sample_num msaa sample num
   */
  explicit Render(int multi_sample_num);

  /**
   * Set screen width, height and optional multi_sample_num to render.
   * @param width screen width
   * @param height screen height
   * @param multi_sample_num msaa sample num
   */
  Render(int width, int height, int multi_sample_num = 4);

  /**
   * Deconstruct.
   */
  ~Render();

  void RasterizationFromScratch(const Model &one_model, const glm::mat4 &total_matrix, const MeshProjection &mp) const;

  /**
   * Render with only one camera.
   */
  void OneCameraRender();

  /**
   * Render mesh projection.
   * @param shaded set true to use shaded render; false to Use depth map render
   */
  void MeshProjectionRender(std::vector<Model> &all_models, bool shaded);

  /**
   * Render shaded segmentation result.
   */
  void ShadedSegRender();

  /**
   * Get camera.
   * @return camera
   */
  static const Camera &GetCamera();

  /**
   * Set camera.
   * @param camera camera
   */
  static void SetCamera(const Camera &camera);

  /**
   * Get screen width.
   * @return screen width
   */
  [[nodiscard]] int GetScreenWidth() const;

  /**
   * Get screen height.
   * @return screen height
   */
  [[nodiscard]] int GetScreenHeight() const;

  /**
   * Get glfw window.
   * @return glfw window
   */
  [[nodiscard]] GLFWwindow *GetWindow() const;

  /**
   * Get multiple sample number.
   * @return multiple sample number
   */
  [[nodiscard]] int GetMultiSampleNum() const;

  glm::mat4 &GetViewportMatrix();

 private:
  /**
   * Called on frame buffer size changed.
   * @param window glfw window
   * @param width window width
   * @param height window height
   */
  static void OnFramebufferSizeChange(GLFWwindow *window, int width, int height);

  /**
   * Process keyboard input(mainly).
   * @param window glfw window
   */
  static void ProcessInput(GLFWwindow *window);

  /**
   * Called on mouse move
   * @param window glfw window
   * @param pos_x_in mouse position x
   * @param pos_y_in mouse position y
   */
  static void OnMouseMove(GLFWwindow *window, double pos_x_in, double pos_y_in);

  /**
   * Called on mouse wheel scroll
   * @param window glfw window
   * @param offset_x mouse wheel offset x
   * @param offset_y mouse wheel offset y
   */
  static void OnMouseScroll(GLFWwindow *window, double offset_x, double offset_y);

  /**
   * Init render.
   */
  void InitRender();

  /**
   * Compute minimum and maximum depth from model(mesh) to our view(camera)
   * @param shader model shader
   * @param ourModel model(mesh)
   * @param projection projection matrix
   * @param view view matrix
   * @param model model transformation matrix
   */
  void ComputeDepthMinMax(Shader &shader, Model &ourModel, glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model);

  // keyboard input delta time
  static float delta_time_;
  // keyboard input last time
  static float last_frame_;

  // if mouse first move
  static bool first_mouse_;
  // mouse last move position x
  static float last_x_;
  // mouse last move position y
  static float last_y_;

  // camera
  static Camera camera_;

  // screen width
  int screen_width_{kScreenWidth};
  // screen height
  int screen_height_{kScreenHeight};
  // window
  GLFWwindow *window_{};
  // msaa sample num
  int multi_sample_num_{4};
  // viewport matrix(column vector)
  glm::mat4 viewport_matrix_;


};


#endif //MESHPROJECTION_RENDER_H
