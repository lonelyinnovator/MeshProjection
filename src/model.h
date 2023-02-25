// Created by lonelyinnovator on 2023/2/22 0:04.
/**
 * Load all meshes in model
 */

#ifndef OPENGL_MODEL_H
#define OPENGL_MODEL_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader.h"


/**
 * Load texture from file.
 * @param path filename
 * @param directory file directory
 * @param gamma gamma
 * @return texture id
 */
unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);


/**
 * Load all meshes, textures etc. in model.
 * Examples:
 *   Model model(path, seg_path);
 *   model.draw(shader);
 */
class Model {
 public:
  /**
   * Constructor, expects a filepath to a 3D model.
   * @param model_path model file path
   * @param segPath
   * @param gamma
   */
  Model(const std::string &model_path, const std::string &segPath = "", bool gamma = false);

  /**
   * Draws the model, and thus all its meshes.
   * @param shader shader to draw model
   */
  void Draw(Shader &shader);

  /**
   * Get loaded textures in model.
   * @return loaded textures in model.
   */
  const std::vector<Texture> &GetTexturesLoaded() const;

  /**
   * Get all meshes in model.
   * @return all meshes in model
   */
  const std::vector<Mesh> &GetMeshes() const;

  /**
   * Get model directory.
   * @return model directory
   */
  const std::string &GetDirectory() const;

  /**
   * Get model name(e.g. 21).
   * @return model name
   */
  const std::string &GetName() const;

  /**
   * Get model suffix(e.g. .off).
   * @return model suffix
   */
  const std::string &GetSuffix() const;

  /**
   * Get model segmentation result.
   * @return model segmentation result
   */
  const std::vector<int> &GetModelSeg() const;

  /**
   * Get max segmentation class in segmentation result.
   * @return max segmentation class
   */
  int GetMaxSegClass() const;

  /**
   * Get gamma_correction.
   * @return gamma_correction
   */
  bool GetGammaCorrection() const;

 private:

  /**
   * loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
   * @param path model path
   */
  void LoadModel(const std::string &path);

  /**
   * processes a node in a recursive fashion.
   * Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
   * @param node ASSIMP node
   * @param scene ASSIMP scene
   */
  void ProcessNode(aiNode *node, const aiScene *scene);

  /**
   * Process mesh and save in the array meshes.
   * @param mesh ASSIMP mesh
   * @param scene ASSIMP scene
   * @return
   */
  Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

  /**
   * Load model segmentation file from path.
   * @param seg_path segmentation file path
   */
  void LoadModelSeg(const std::string &seg_path);

  /**
   * checks all material textures of a given type and loads the textures if they're not loaded yet.
   * the required info is returned as a Texture struct.
   * @param mat ASSIMP material
   * @param type texture type
   * @param type_name texture type name
   * @return texture array
   */
  std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string type_name);

  // stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
  std::vector<Texture> textures_loaded_{};
  // model meshes
  std::vector<Mesh> meshes_{};
  // model dir
  std::string directory_{};
  // model name
  std::string name_{};
  // model suffix (.off, .obj, ...)
  std::string suffix_{};
  // model seg to load
  std::vector<int> model_seg_{};
  // max seg class in model seg
  int max_seg_class_{0};

  bool gamma_correction_{true};
};


#endif //OPENGL_MODEL_H
