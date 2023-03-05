// Created by lonelyinnovator on 2023/3/2 0:33.


#ifndef MESHPROJECTION_SHAPENETCOREDATASET_H
#define MESHPROJECTION_SHAPENETCOREDATASET_H

#include <iostream>
#include <vector>
#include <map>
#include <string>

class ShapeNetCoreDataset {
 public:
  static const std::string &GetDataDir();

  static const std::string &GetGenDir();

  static const std::vector<std::string> &GetModelClasses();

  static const std::vector<int> &GetModelNumPerClass();

  static std::vector<std::vector<std::string>> &GetModelNamesPerClass();

 private:
  // absolute data dir
  static const std::string data_dir_;

  // generated dir
  static const std::string gen_dir_;

  // all model names
  static const std::vector<std::string> model_classes_;

  // num of model in each class
  static const std::vector<int> model_num_per_class_;

  // key: class name, value: all model names in class
  static std::vector<std::vector<std::string>> model_names_per_class_;
};

#endif //MESHPROJECTION_SHAPENETCOREDATASET_H
