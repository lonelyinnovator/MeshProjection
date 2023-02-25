// Created by lonelyinnovator on 2023/2/22 16:03.
/**
 * MeshProjection, to project mesh in multiple views.
 */

#ifndef OPENGL_MESH_PROJECTION_H
#define OPENGL_MESH_PROJECTION_H

#include <iostream>
#include <vector>
#include <algorithm>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"

// roll number: camera rotation number
const unsigned int kRollNum = 4;
// projection camera number
const unsigned int kProjCameraNum = 26 * kRollNum;

/**
 * To project mesh on render window in different views(use proj_camera).
 * Example:
 *   MeshProjection mp;
 *   Camera camera = mp.GetNextProjCamera();
 *   mp.ProjSnapshot(...);
 */
class MeshProjection {
 public:
  /**
   * Constructor
   */
  MeshProjection();

  /**
   * Use radius to init mesh projection.
   * @param radius distance between center point to viewer(camera)
   */
  MeshProjection(float radius);

  /**
   * Deconstruct
   */
  ~MeshProjection();

  /**
   * Snapshot current rendered window to image.
   * @param output_dir image output dir
   * @param prefix image prefix
   * @param width image width
   * @param height image height
   * @param is_rgb set true to save rgb image; false to save gray image
   */
  void ProjSnapshot(const std::string &output_dir, const std::string &prefix, int width, int height, bool is_rgb);

  /**
   * Get radius.
   * @return radius
   */
  float GetRadius() const;

  /**
   * Set radius.
   * @param radius radius
   */
  void SetRadius(float radius);

  /**
   * Get current camera index.
   * @return current camera index
   */
  int GetCurrentCameraIndex() const;

  /**
   * Reset current camera index to -1(default value)
   */
  void ResetCurrentCameraIndex();

  /**
   * Get next projection camera.
   * @param out_camera output projection camera
   * @return if get camera successfully
   */
  bool GetNextProjCamera(Camera &out_camera);

  /**
   * Get projection camera by camera index.
   * @param out_camera output projection camera
   * @param index camera index
   * @return if get camera successfully
   */
  bool GetProjCameraByIndex(Camera &out_camera, int index);


 private:
  /**
   * Set up projection camera in different views.
   */
  void SetUpProjCamera();

  /**
   * Compute camera position.
   * @param center_point mesh center point(usually world origin)
   * @param radius radius
   * @param pitch camera pitch angle
   * @param yaw camera yaw angle
   * @return camera position
   */
  glm::vec3 ComputeCameraPosition(glm::vec3 center_point, float radius, float pitch, float yaw);

  // distance of camera and mesh center point
  float radius_{3.0f};

  // projection cameras
  Camera proj_cameras_[kProjCameraNum];

  // current camera index in proj_cameras array
  int current_camera_index_{-1};
};


#endif //OPENGL_MESH_PROJECTION_H