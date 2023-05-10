#include "interface.h"

const std::vector<std::string> Interface::AbstractInterface::m_menus{
    " -------------------------------------------------------------- \n"
    "|                          Memory 1.0                          |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. void init(std::size_t size);                              |\n"
    "| 2. Inserting an element at a specific address                |\n"
    "| 3. Printing the current state of the heap                    |\n"
    "| 4. void *malloc (std::size_t size)                           |\n"
    "| 5. void *calloc (std::size_t num, std::size_t size)          |\n"
    "| 6. void *realloc(void *ptr, std::size_t size)                |\n"
    "| 7. void free (void* ptr)                                     |\n"
    "| 8. void *malloc_onlyfree (std::size_t size)                  |\n"
    "| 9. void *calloc_onlyfree (std::size_t num, std::size_t size) |\n"
    "| 10. void *realloc_onlyfree(void *ptr, std::size_t size)      |\n"
    "| 11. void free_onlyfree (void* ptr)                           |\n"
    "| 12. Research on optimizing the search for free blocks        |\n"
    "| 13. defragmentation                                          |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Enter function arguments separated by a space             |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Write data to a specific address according                |\n"
    "|    to the following pattern: 0x2f7b1a0 int[10] {4, 5};       |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                       Select menu item                       |\n"
    " -------------------------------------------------------------- \n"
    "| 0. Exit                                                      |\n"
    "| 1. Enter the percentage of memory to be cleared (0 - 99)     |\n"
    " -------------------------------------------------------------- \n"
    " > ",
    " -------------------------------------------------------------- \n"
    "|                 The input data is incorrect                  |\n"
    " -------------------------------------------------------------- \n",
    " -------------------------------------------------------------- \n"
    "|            Successful completion of the programme            |\n"
    " -------------------------------------------------------------- \n"};

const std::vector<std::regex> Interface::m_patterns{
    std::regex(
        R"(0x([0-9A-Fa-f]+) (\w+)(?:\[([1-9]\d*)\])?\s+\{((?:\s*[^\s,]+(?:\s*,\s*[^\s,]+)*)?)\s*\};)"),
    std::regex("^0x([0-9A-Fa-f]+)\\s+([1-9][0-9]*)$"),
    std::regex("^([1-9][0-9]*)\\s+([1-9][0-9]*)$"),
    std::regex("^0x([0-9A-Fa-f]+)$"), std::regex("^([1-9][0-9]*)$")};

Interface::Interface() {
  InitFuncMenus();
  ShowMenu(m_menus[MenuItem::kIntroduction]);
}

void Interface::InitFuncMenus() {
  m_funcs.resize(MenuFuncs::kMenuFuncsAll);

  m_funcs[MenuFuncs::kMainFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kMemAllocFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kWriteItemFuncMenu]),
                MenuItem::kWriteItemMenu),
      []() -> bool {
        Memory::dump();
        return true;
      },
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kMallocFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kCallocFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kReallocFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kFreeFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kMallocOnlyFreeFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kCallocOnlyFreeFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kReallocOnlyFreeFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kFreeOnlyFreeFuncMenu]),
                MenuItem::kArgumentsMenu),
      std::bind(&Interface::RunMenu, this,
                std::ref(m_funcs[MenuFuncs::kStatisticFuncMenu]),
                MenuItem::kStatisticMenu),
      []() -> bool {
        Memory::defragmentation();
        return true;
      }};

  m_funcs[MenuFuncs::kMemAllocFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[4],
                [](const std::smatch &match) -> void {
                  Memory::init(std::stoull(match[1]));
                })};

  m_funcs[MenuFuncs::kWriteItemFuncMenu] = {
      std::bind(&Interface::Exit, this), [this]() -> bool {
        std::string input, token;
        std::getline(std::cin, input);

        std::smatch match;
        if (std::regex_match(input, match, m_patterns[0])) {
          const std::string &addr = match[1];
          const std::string &type = match[2];
          const std::string &size = match[3];
          std::string args = match[4];

          args.erase(std::remove_if(args.begin(), args.end(), ::isspace),
                     args.end());
          const std::size_t length = (size.empty() ? 1 : std::stoull(size));
          std::istringstream iss(args);
          if (type == "char") {
            std::vector<char> values;
            values.reserve(length);
            for (std::size_t i = 0;
                 (i < length) && std::getline(iss, token, ','); ++i) {
              values.push_back(std::move(token.front()));
            }
            return !Memory::write(
                reinterpret_cast<void *>(std::stoull(addr, nullptr, 16)),
                values);
          } else if (type == "int") {
            std::vector<int> values;
            values.reserve(length);
            for (std::size_t i = 0;
                 (i < length) && std::getline(iss, token, ','); ++i) {
              values.push_back(std::stoi(token));
            }
            return !Memory::write(
                reinterpret_cast<void *>(std::stoull(addr, nullptr, 16)),
                values);
          } else if (type == "double") {
            std::vector<double> values;
            values.reserve(length);
            for (std::size_t i = 0;
                 (i < length) && std::getline(iss, token, ','); ++i) {
              values.push_back(std::stod(token));
            }
            return !Memory::write(
                reinterpret_cast<void *>(std::stoull(addr, nullptr, 16)),
                values);
          }
        } else {
          ShowMenu(m_menus[MenuItem::kIncorectInputMenu]);
        }
        return false;
      }};

  m_funcs[MenuFuncs::kMallocFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[4],
                [](const std::smatch &match) -> void {
                  std::cout << Memory::malloc(std::stoull(match[1])) << '\n';
                })};

  m_funcs[MenuFuncs::kCallocFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[2],
                [](const std::smatch &match) -> void {
                  std::cout << Memory::calloc(std::stoull(match[1]),
                                           std::stoull(match[2]))
                            << '\n';
                })};

  m_funcs[MenuFuncs::kReallocFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[1],
                [](const std::smatch &match) -> void {
                  std::cout
                      << Memory::realloc(reinterpret_cast<void *>(
                                          std::stoull(match[1], nullptr, 16)),
                                      std::stoull(match[2]))
                      << '\n';
                })};

  m_funcs[MenuFuncs::kFreeFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[3],
                [](const std::smatch &match) -> void {
                  void *ptr = reinterpret_cast<void *>(
                      std::stoull(match[1], nullptr, 16));
                  Memory::free(ptr);
                })};

  m_funcs[MenuFuncs::kMallocOnlyFreeFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[4],
                [](const std::smatch &match) -> void {
                  std::cout << Memory::malloc_onlyfree(std::stoull(match[1]))
                            << '\n';
                })};

  m_funcs[MenuFuncs::kCallocOnlyFreeFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[2],
                [](const std::smatch &match) -> void {
                  std::cout << Memory::calloc_onlyfree(std::stoull(match[1]),
                                                    std::stoull(match[2]))
                            << '\n';
                })};

  m_funcs[MenuFuncs::kReallocOnlyFreeFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[1],
                [](const std::smatch &match) -> void {
                  std::cout << Memory::realloc_onlyfree(
                                   reinterpret_cast<void *>(
                                       std::stoull(match[1], nullptr, 16)),
                                   std::stoull(match[2]))
                            << '\n';
                })};

  m_funcs[MenuFuncs::kFreeOnlyFreeFuncMenu] = {
      std::bind(&Interface::Exit, this),
      std::bind(&Interface::RunMemoryMenu, this, m_patterns[3],
                [](const std::smatch &match) -> void {
                  void *ptr = reinterpret_cast<void *>(
                      std::stoull(match[1], nullptr, 16));
                  Memory::free_onlyfree(ptr);
                })};

  m_funcs[MenuFuncs::kStatisticFuncMenu] = {
      std::bind(&Interface::Exit, this), []() -> bool {
        const std::size_t ELEMENTS = 1e5;
        const std::size_t HEADER_SIZE = 48;
        const std::size_t FREES = CheckInputItem(-1, 100) / 100.0 * ELEMENTS;

        auto statictic = [&](std::function<void *(std::size_t)> allocator,
                             std::function<void(void *)> deallocator,
                             std::string msg) {
          std::vector<void *> ptrs;

          Memory::init(HEADER_SIZE * ELEMENTS + 1e6);
          ptrs.reserve(ELEMENTS);
          for (std::size_t i = 0; i < ELEMENTS; ++i) {
            ptrs.push_back(allocator(10));
          }
          std::shuffle(ptrs.begin(), ptrs.end(),
                       std::mt19937(std::random_device{}()));
          for (std::size_t i = 0; i < FREES; ++i) {
            deallocator(ptrs[i]);
          }
          auto start = std::chrono::steady_clock::now();
          for (std::size_t i = 0; i < FREES; ++i) {
            allocator(10);
          }
          auto end = std::chrono::steady_clock::now();
          std::cout << msg
                    << std::chrono::duration_cast<std::chrono::seconds>(end -
                                                                        start)
                           .count()
                    << "s\n";
        };

        std::cout << "Statistics of memory allocator:\n";
        statictic(Memory::malloc, Memory::free, "malloc: ");
        statictic(Memory::malloc_onlyfree, Memory::free_onlyfree,
                  "malloc_onlyfree: ");
        return false;
      }};
}

bool Interface::RunMemoryMenu(
    const std::regex &pattern,
    std::function<void(const std::smatch &match)> func) {
  std::string input;
  std::getline(std::cin, input);
  std::smatch match;
  if (std::regex_search(input, match, pattern)) {
    func(match);
  } else {
    ShowMenu(m_menus[MenuItem::kIncorectInputMenu]);
  }
  return false;
}

Interface::~Interface() { ShowMenu(m_menus[MenuItem::kCompletion]); }

void Interface::Exec() {
  RunMenu(m_funcs[MenuFuncs::kMainFuncMenu], MenuItem::kMainMenu);
}