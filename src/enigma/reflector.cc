#include "reflector.h"

namespace s21 {

Reflector::Reflector() {
  std::vector<char> alphabet;

  for (char c = 'A'; c <= 'Z'; ++c) {
    alphabet.push_back(c);
  }

  std::random_device rd;
  std::mt19937 gen(rd());
  std::shuffle(alphabet.begin(), alphabet.end(), gen);

  for (std::size_t i = 0; i < alphabet.size() / 2; ++i) {
    reflector_.insert({alphabet[i], alphabet[alphabet.size() - i - 1]});
    reflector_.insert({alphabet[alphabet.size() - i - 1], alphabet[i]});
  }
}

Reflector::Reflector(const std::map<char, char>& reflector)
    : reflector_(reflector) {
  for (const auto& i : reflector_) {
    auto it = reflector_.find(i.second);

    if (it != reflector_.end() && i.first != it->second) {
      reflector_.erase(i.second);
    }
    if (reflector_.find(i.second) == reflector_.end()) {
      reflector_.insert({i.second, i.first});
    }
  }
}

char Reflector::Forward(char letter) {
  auto it = reflector_.find(letter);

  if (it == reflector_.end()) {
    throw std::runtime_error("Reflector: The letter '" +
                             std::to_string(letter) + "' does not exist");
  }

  return it->second;
}

std::map<char, char> Reflector::GetSettings() { return reflector_; }

}  // namespace s21
