// Created by lonelyinnovator on 2023/2/23 22:24.

#ifndef MESHPROJECTION_COLOR_HPP
#define MESHPROJECTION_COLOR_HPP

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fmt/core.h>


class Color {
 public:
  // colors
  constexpr static const glm::vec3 Red = glm::vec3(255.0f, 0.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 Blue = glm::vec3(0.0f, 0.0f, 255.0f) / 255.0f;
  constexpr static const glm::vec3 Green = glm::vec3(0.0f, 255.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 Yellow = glm::vec3(255.0f, 255.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 Cyan = glm::vec3(0.0f, 255.0f, 255.0f) / 255.0f;
  constexpr static const glm::vec3 Orange = glm::vec3(255.0f, 165.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 Purple = glm::vec3(160.0f, 32.0f, 240.0f) / 255.0f;
  constexpr static const glm::vec3 Pink = glm::vec3(255.0f, 192.0f, 203.0f) / 255.0f;
  constexpr static const glm::vec3 Grey = glm::vec3(190.0f, 190.0f, 190.0f) / 255.0f;
  constexpr static const glm::vec3 Gold = glm::vec3(255.0f, 215.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 Brown = glm::vec3(165.0f, 42.0f, 42.0f) / 255.0f;
  constexpr static const glm::vec3 HotPink = glm::vec3(255.0f, 105.0f, 180.0f) / 255.0f;
  constexpr static const glm::vec3 DeepPink = glm::vec3(255.0f, 20.0f, 147.0f) / 255.0f;
  constexpr static const glm::vec3 VioletRed = glm::vec3(208.0f, 32.0f, 144.0f) / 255.0f;
  constexpr static const glm::vec3 Maroon = glm::vec3(176.0f, 48.0f, 96.0f) / 255.0f;
  constexpr static const glm::vec3 Magenta = glm::vec3(255.0f, 0.0f, 255.0f) / 255.0f;
  constexpr static const glm::vec3 Violet = glm::vec3(238.0f, 130.0f, 238.0f) / 255.0f;

  constexpr static const glm::vec3 Snow = glm::vec3(255.0f, 250.0f, 250.0f) / 255.0f;
  constexpr static const glm::vec3 GhostWhite = glm::vec3(248.0f, 248.0f, 255.0f) / 255.0f;
  constexpr static const glm::vec3 WhiteSmoke = glm::vec3(245.0f, 245.0f, 245.0f) / 255.0f;
  constexpr static const glm::vec3 PeachPuff = glm::vec3(255.0f, 218.0f, 185.0f) / 255.0f;
  constexpr static const glm::vec3 LemonChiffon = glm::vec3(255.0f, 250.0f, 205.0f) / 255.0f;
  constexpr static const glm::vec3 Lavender = glm::vec3(230.0f, 230.0f, 250.0f) / 255.0f;
  constexpr static const glm::vec3 LightSlateGray = glm::vec3(119.0f, 136.0f, 153.0f) / 255.0f;
  constexpr static const glm::vec3 LightGray = glm::vec3(211.0f, 211.0f, 211.0f) / 255.0f;
  constexpr static const glm::vec3 CornflowerBlue = glm::vec3(100.0f, 149.0f, 237.0f) / 255.0f;
  constexpr static const glm::vec3 DeepSkyBlue = glm::vec3(0.0f, 191.0f, 255.0f) / 255.0f;
  constexpr static const glm::vec3 LightSkyBlue = glm::vec3(135.0f, 206.0f, 250.0f) / 255.0f;
  constexpr static const glm::vec3 Aquamarine = glm::vec3(127.0f, 255.0f, 212.0f) / 255.0f;
  constexpr static const glm::vec3 DarkGreen = glm::vec3(0.0f, 100.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 MediumSeaGreen = glm::vec3(60.0f, 179.0f, 113.0f) / 255.0f;
  constexpr static const glm::vec3 SpringGreen = glm::vec3(0.0f, 255.0f, 127.0f) / 255.0f;
  constexpr static const glm::vec3 GreenYellow = glm::vec3(173.0f, 255.0f, 47.0f) / 255.0f;
  constexpr static const glm::vec3 YellowGreen = glm::vec3(154.0f, 205.0f, 50.0f) / 255.0f;
  constexpr static const glm::vec3 LightYellow = glm::vec3(255.0f, 255.0f, 224.0f) / 255.0f;
  constexpr static const glm::vec3 DarkGoldenrod = glm::vec3(184.0f, 134.0f, 11.0f) / 255.0f;
  constexpr static const glm::vec3 RosyBrown = glm::vec3(188.0f, 143.0f, 143.0f) / 255.0f;
  constexpr static const glm::vec3 IndianRed = glm::vec3(205.0f, 92.0f, 92.0f) / 255.0f;
  constexpr static const glm::vec3 Salmon = glm::vec3(250.0f, 128.0f, 114.0f) / 255.0f;
  constexpr static const glm::vec3 Coral = glm::vec3(255.0f, 127.0f, 80.0f) / 255.0f;
  constexpr static const glm::vec3 Tomato = glm::vec3(255.0f, 99.0f, 71.0f) / 255.0f;
  constexpr static const glm::vec3 OrangeRed = glm::vec3(255.0f, 69.0f, 0.0f) / 255.0f;
  constexpr static const glm::vec3 DarkOrchid = glm::vec3(153.0f, 50.0f, 204.0f) / 255.0f;
  constexpr static const glm::vec3 White = glm::vec3(255.0f, 255.0f, 255.0f) / 255.0f;
  constexpr static const glm::vec3 Black = glm::vec3(0.0f, 0.0f, 0.0f) / 255.0f;


  // all colors
  constexpr static const glm::vec3 AllColors[]{
      Red, Blue, Green, Yellow, Cyan,
      Orange, Purple, Pink, Grey, Gold,
      Brown, HotPink, DeepPink, VioletRed, Maroon,
      Magenta, Violet, Snow, GhostWhite, WhiteSmoke,
      PeachPuff, LemonChiffon, Lavender, LightSlateGray, LightGray,
      CornflowerBlue, DeepSkyBlue, LightSkyBlue, Aquamarine, DarkGreen,
      MediumSeaGreen, SpringGreen, GreenYellow, YellowGreen, LightYellow,
      DarkGoldenrod, RosyBrown, IndianRed, Salmon, Coral,
      Tomato, OrangeRed, DarkOrchid, White, Black,
  };

  /**
   * Get size of array AllColors.
   * @return size of array AllColors
   */
  static int GetAllColorsSize() {
    return sizeof(AllColors) / sizeof(glm::vec3);
  }

  /**
   * Print color.
   * @param color color
   */
  static void PrintColor(const glm::vec3 &color) {
    std::cout << fmt::format("{} {} {}", color.x, color.y, color.z) << std::endl;
  }
};


#endif //MESHPROJECTION_COLOR_HPP
