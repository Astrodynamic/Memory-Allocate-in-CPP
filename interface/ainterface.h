#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <regex>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class AbstractInterface {
 public:
  AbstractInterface() = default;
  explicit AbstractInterface(const AbstractInterface &other) = delete;
  explicit AbstractInterface(AbstractInterface &&other) = delete;
  AbstractInterface &operator=(const AbstractInterface &other) = delete;
  AbstractInterface &operator=(AbstractInterface &&other) = delete;
  virtual ~AbstractInterface() = default;

  virtual void Exec() = 0;

 protected:
  bool RunMenu(const std::vector<std::function<bool(void)>> &func,
               std::size_t menu);
  std::size_t ShowMenu(const std::string &menu, const std::size_t items = 0U);

  [[nodiscard]] static std::int64_t CheckInputItem(const std::int64_t min,
                                                   const std::int64_t max);
  [[nodiscard]] static std::pair<bool, std::filesystem::path>
  CheckInputPathFile();
  [[nodiscard]] bool Exit();

  static const std::vector<std::string> m_menus;
  std::vector<std::vector<std::function<bool(void)>>> m_funcs;
};
