// Created by lonelyinnovator on 2023/2/23 19:53.

#include "general_algorithm.h"


int GeneralAlgorithm::MostFreqNum(std::vector<int> &nums) {
  if (nums.empty()) return 0;
  int res = 0, mx = -1;
  std::map<int, int> m;
  for (int &num: nums) {
    m[num]++;
  }
  for (int i = 0; i < m.size(); ++i) {
    if (mx < m[i]) {
      mx = m[i];
      res = i;
    }
  }
  return res;
}

std::vector<std::string> GeneralAlgorithm::Split(const std::string &in_str, char delim) {
  std::vector<std::string> res;
  std::stringstream ss(in_str);
  std::string line;
  while (std::getline(ss, line, delim)) {
    res.push_back(line);
  }
  return res;
}
