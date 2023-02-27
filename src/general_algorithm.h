// Created by lonelyinnovator on 2023/2/23 19:53.
/**
 * General algorithm used in the program.
 */

#ifndef MESHPROJECTION_GENERAL_ALGORITHM_H
#define MESHPROJECTION_GENERAL_ALGORITHM_H

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * General algorithm.
 */
class GeneralAlgorithm {
 public:

  /**
   * Get the most frequently appeared number in the array.
   * @param nums number array
   * @return most frequently appeared number
   */
  static int MostFreqNum(std::vector<int> &nums);

  /**
   * Split string by delimiter into sub string parts
   * @param in_str input string
   * @param delim delimiter
   * @return split sub string
   */
  static std::vector<std::string> Split(const std::string &in_str, char delim = '\n');

  static float Cross2d(glm::vec3 v1, glm::vec3 v2);

  static float Cross2d(glm::vec2 v1, glm::vec2 v2);

  static bool IsInsideTriangle(glm::vec2 &point, std::vector<glm::vec2> &triangle_vertices);

  static std::tuple<float, float, float>
  ComputeBaryCentric2d(glm::vec2 &point, std::vector<glm::vec2> &triangle_vertices);
};


#endif //MESHPROJECTION_GENERAL_ALGORITHM_H
