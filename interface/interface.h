#pragma once

#include "ainterface.h"
#include "memory.h"

class Interface final : virtual public AbstractInterface {
 public:
  Interface();
  explicit Interface(const Interface &other) = delete;
  explicit Interface(Interface &&other) = delete;
  Interface &operator=(const Interface &other) = delete;
  Interface &operator=(Interface &&other) = delete;
  ~Interface();

  virtual void Exec() final override;

 private:
  enum MenuFuncs : std::size_t {
    kMainFuncMenu = 0U,
    kMemAllocFuncMenu,
    kWriteItemFuncMenu,
    kMallocFuncMenu,
    kCallocFuncMenu,
    kReallocFuncMenu,
    kFreeFuncMenu,
    kMallocOnlyFreeFuncMenu,
    kCallocOnlyFreeFuncMenu,
    kReallocOnlyFreeFuncMenu,
    kFreeOnlyFreeFuncMenu,
    kStatisticFuncMenu,
    kMenuFuncsAll
  };

  enum MenuItem : std::size_t {
    kIntroduction = 0U,
    kMainMenu,
    kArgumentsMenu,
    kWriteItemMenu,
    kStatisticMenu,
    kIncorectInputMenu,
    kCompletion
  };

  static const std::vector<std::regex> m_patterns;

  void InitFuncMenus();
  bool RunMemoryMenu(const std::regex &pattern, std::function<void(const std::smatch &match)> func);
};