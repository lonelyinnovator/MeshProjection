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
};


#endif //MESHPROJECTION_GENERAL_ALGORITHM_H
