#include <cppformlang/greeter.h>

#include <utility>

using namespace greeter;

Greeter::Greeter(std::string _name) : name(std::move(_name)) {}

std::string Greeter::greet(LanguageCode lang) const {
  switch (lang) {
    default:
    case LanguageCode::EN:
      return "Hello, " + name + "!";
    case LanguageCode::DE:
      return "Hallo " + name + "!";
    case LanguageCode::ES:
      return "¡Hola " + name + "!";
    case LanguageCode::FR:
      return "Bonjour " + name + "!";
  }
}
