// Created by lonelyinnovator on 2023/3/2 0:39.
#include "shapenetcore_dataset.h"

const std::string &ShapeNetCoreDataset::GetDataDir() {
  return data_dir_;
}

const std::string &ShapeNetCoreDataset::GetGenDir() {
  return gen_dir_;
}

const std::vector<std::string> &ShapeNetCoreDataset::GetModelClasses() {
  return model_classes_;
}


const std::vector<int> &ShapeNetCoreDataset::GetModelNumPerClass() {
  return model_num_per_class_;
}

std::vector<std::vector<std::string>> &ShapeNetCoreDataset::GetModelNamesPerClass() {
  if (model_names_per_class_.size() == GetModelClasses().size()) {
    return model_names_per_class_;
  }
  model_names_per_class_.resize(GetModelClasses().size());
  for (int i = 0; i < GetModelClasses().size(); ++i) {
    for (int j = 1; j <= GetModelNumPerClass()[i]; ++j) {
      model_names_per_class_[i].push_back(std::to_string(j));
    }
  }
  return model_names_per_class_;
}


const std::string ShapeNetCoreDataset::data_dir_ = R"(E:\Work\pycharmWork\AllDatasets\ShapeNetCore\)";

const std::string ShapeNetCoreDataset::gen_dir_ = R"(E:\Work\pycharmWork\AllDatasets\ShapeNetCore_Gen\)";

const std::vector<std::string> ShapeNetCoreDataset::model_classes_{"Airplane", "Bag", "Cap", "Car", "Chair",
                                                                   "Earphone", "Guitar", "Knife", "Lamp", "Laptop",
                                                                   "Motorbike", "Mug", "Pistol", "Rocket", "Skateboard",
                                                                   "Table"};

const std::vector<int> ShapeNetCoreDataset::model_num_per_class_{500, 76, 55, 500, 500, 69, 500, 392, 500, 445, 202,
                                                                 184, 275, 66, 152, 500};


std::vector<std::vector<std::string>> ShapeNetCoreDataset::model_names_per_class_{};

