#include <cppformlang/version.h>

#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <unordered_map>

int main(int argc, char** argv) {
  cxxopts::Options options(argv[0], "A program to welcome the world!");

  std::string language;
  std::string name;

  // clang-format off
  options.add_options()
    ("h,help", "Show help")
    ("v,version", "Print the current version number")
  ;
  // clang-format on

  auto result = options.parse(argc, argv);

  if (result["help"].as<bool>()) {
    std::cout << options.help() << std::endl;
    return 0;
  } else if (result["version"].as<bool>()) {
    std::cout << "cppformlang, version " << CPPFORMLANG_VERSION << std::endl;
    return 0;
  }

  std::cout << "end" << std::endl;

  return 0;
}
