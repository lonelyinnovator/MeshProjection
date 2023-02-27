// Created by lonelyinnovator on 2023/2/23 17:03.

#include "psbdataset.h"

std::vector<std::string> PsbDataset::GetAllModelName() {
  std::vector<std::string> all_model_name;
  for (int i = 1; i <= 260; ++i) all_model_name.push_back(std::to_string(i));
  for (int i = 281; i <= 400; ++i) all_model_name.push_back(std::to_string(i));
  return all_model_name;
}


const std::string &PsbDataset::GetDataDir() {
  return data_dir_;
}

const std::string &PsbDataset::GetImageDir() {
  return image_dir_;
}

const std::string &PsbDataset::GetModelDir() {
  return model_dir_;
}

const std::string &PsbDataset::GetSegDir() {
  return seg_dir_;
}


const std::string &PsbDataset::GetPixelDir() {
  return pixel_dir_;
}


const std::string PsbDataset::data_dir_ = R"(E:\Work\pycharmWork\3DMeshCode\CLIP4Mesh\data\psb\)";

const std::string PsbDataset::image_dir_ = PsbDataset::data_dir_ + R"(images\)";

const std::string PsbDataset::model_dir_ = PsbDataset::data_dir_ + R"(off\)";

const std::string PsbDataset::seg_dir_ = PsbDataset::data_dir_ + R"(seg\Benchmark\)";

const std::string PsbDataset::pixel_dir_ = PsbDataset::data_dir_ + R"(pixel\)";

