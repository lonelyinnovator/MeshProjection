// Created by lonelyinnovator on 2023/2/22 16:03.

#include "mesh_projection.h"

MeshProjection::MeshProjection() {
  SetUpProjCamera();
}

MeshProjection::MeshProjection(float radius) {
  radius_ = radius;
  SetUpProjCamera();
}


MeshProjection::~MeshProjection() {

}

void MeshProjection::ProjSnapshot(const std::string &output_dir, const std::string &prefix, int width, int height,
                                  bool is_rgb) {
  if (current_camera_index_ >= 0) {
    std::string filename = std::format("{}_{}_{}.png", prefix, std::to_string(current_camera_index_ / kRollNum),
                                       std::to_string(current_camera_index_ % kRollNum));
    Camera::Snapshot(filename, std::format(R"({}\{}\)", output_dir, prefix), width, height, is_rgb);
  }
}

float MeshProjection::GetRadius() const {
  return radius_;
}

void MeshProjection::SetRadius(float radius) {
  radius_ = radius;
  SetUpProjCamera();
}

int MeshProjection::GetCurrentCameraIndex() const {
  return current_camera_index_;
}

void MeshProjection::ResetCurrentCameraIndex() {
  current_camera_index_ = -1;
}

bool MeshProjection::GetNextProjCamera(Camera &out_camera) {
  current_camera_index_++;
  if (current_camera_index_ >= kProjCameraNum) {
    return false;
  }
  out_camera = proj_cameras_[current_camera_index_];
  return true;
}

bool MeshProjection::GetProjCameraByIndex(Camera &out_camera, int index) {
  if (index < 0 || index >= kProjCameraNum) {
    return false;
  }
  out_camera = proj_cameras_[index];
  return true;
}

void MeshProjection::SetUpProjCamera() {
  float roll_angle[kRollNum];
  for (int i = 0; i < kRollNum; ++i) {
    roll_angle[i] = i * 360.0f / kRollNum;
  }
  for (int i = 0; i < kProjCameraNum / kRollNum; ++i) {
    float pitch = 0.0f, yaw = -90.0f;
    glm::vec3 pos;
    if (i >= 0 && i <= 7) {
      // camera 1-8
      pitch = 0.0f;
      yaw = -90.0f - i * 45.0f;
    } else if (i >= 8 && i <= 15) {
      // camera 9-16
      pitch = -45.0f;
      yaw = -90.0f - (i - 8) * 45.0f;
    } else if (i >= 16 && i <= 23) {
      // camera 17-24
      pitch = 45.0f;
      yaw = -90.0f - (i - 16) * 45.0f;
    } else if (i == 24) {
      // camera 25
      pitch = -90.0f;
      yaw = -90.0f;
    } else if (i == 25) {
      // camera 26
      pitch = 90.0f;
      yaw = -90.0f;
    }
    pos = ComputeCameraPosition(glm::vec3(0.0f), radius_, pitch, yaw);
    for (int j = 0; j < kRollNum; ++j) {
      proj_cameras_[kRollNum * i + j] = Camera(pos, glm::vec3(0.0f, 1.0f, 0.0f), yaw, pitch,
                                               roll_angle[j]);
    }
  }

}

glm::vec3 MeshProjection::ComputeCameraPosition(glm::vec3 center_point, float radius, float pitch, float yaw) {
  glm::vec3 pos;
  pos.x = center_point.x - radius * cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  pos.y = center_point.y - radius * sin(glm::radians(pitch));
  pos.z = center_point.z - radius * cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  return pos;
}







