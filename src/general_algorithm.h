// Created by lonelyinnovator on 2023/2/23 19:53.
/**
 * General algorithm used in the program.
 */

#ifndef OPENGL_GENERAL_ALGORITHM_H
#define OPENGL_GENERAL_ALGORITHM_H

#include <iostream>
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
};


#endif //OPENGL_GENERAL_ALGORITHM_H
