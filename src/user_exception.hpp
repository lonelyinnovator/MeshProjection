// Created by lonelyinnovator on 2023/2/26 23:08.
/**
 * Define user exception and sub-exceptions.
 */

#ifndef MESHPROJECTION_USER_EXCEPTION_HPP
#define MESHPROJECTION_USER_EXCEPTION_HPP

#include <iostream>
#include <string>
#include <utility>

#define FMT_HEADER_ONLY

#include <fmt/format.h>

/**
 * User exception, base class.
 * Examples:
 *   try {
 *     ...
 *   }
 *   catch(const UserException &e) {
 *     std::cout << e.what() << std::endl;
 *   }
 */
class UserException {
 public:
  /**
   * User exception constructor.
   * @param str error message
   * @param id exception id, base class is 0
   */
  explicit UserException(const char *str = nullptr, int id = 0) : error_message_(str), id_(id) {};

  /**
   * User exception constructor.
   * @param str error message
   * @param id exception id, base class is 0
   */
  explicit UserException(std::string str = "", int id = 0) : error_message_(std::move(str)), id_(id) {};

  /**
   * Return formatted error message.
   * @return formatted error message
   */
  [[nodiscard]] virtual std::string what() const = 0;

 protected:
  // error message
  std::string error_message_;
  // exception id
  int id_;
};


/**
 * Filestream exception, sub class of User exception.
 * Examples:
 *   if (error) {
 *     throw FStreamException("error message");
 *   }
 */
class FStreamException : public UserException {
 public:
  /**
   * FileStream exception constructor.
   * @param str error message
   * @param id exception id
   */
  explicit FStreamException(const char *str = nullptr, int id = 1) : UserException(str, id) {};

  /**
   * FileStream exception constructor.
   * @param str error message
   * @param id exception id
   */
  explicit FStreamException(std::string str = "", int id = 1) : UserException(std::move(str), id) {};

  /**
   * Return formatted error message.
   * @return formatted error message
   */
  [[nodiscard]] std::string what() const override {
    return fmt::format("error message: {}, id: {}", error_message_, id_);
  }
};


#endif //MESHPROJECTION_USER_EXCEPTION_HPP
