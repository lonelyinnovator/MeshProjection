// Created by lonelyinnovator on 2023/2/23 17:03.
/**
 * PSB dataset
 */

#ifndef MESHPROJECTION_PSBDATASET_H
#define MESHPROJECTION_PSBDATASET_H

#include <iostream>
#include <vector>
#include <string>


/**
 * PSB dataset, set model path, can load mesh model from dataset.
 * Examples:
 *   auto model_name_list = PsbDataset::GetAllModelName();
 *   auto model_dir = PsbDataset::GetModelDir();
 */
class PsbDataset {
 public:
  /**
   * Get all model name in psb dataset(1, 2, ...).
   * @return psb model name array
   */
  static std::vector<std::string> GetAllModelName();

  /**
   * Get data directory.
   * @return data directory
   */
  static const std::string &GetDataDir();

  /**
   * Get image directory.
   * @return image directory
   */
  static const std::string &GetImageDir();

  /**
   * Get model directory.
   * @return model directory
   */
  static const std::string &GetModelDir();

  /**
   * Get segmentation directory.
   * @return segmentation directory
   */
  static const std::string &GetSegDir();

 private:
  // absolute data dir
  static const std::string data_dir_;

  // output image dir
  static const std::string image_dir_;

  // model dir
  static const std::string model_dir_;

  // seg dir
  static const std::string seg_dir_;
};


#endif //MESHPROJECTION_PSBDATASET_H
