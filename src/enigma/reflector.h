#ifndef A4_CRYPTO_ENIGMA_REFLECTOR_H_
#define A4_CRYPTO_ENIGMA_REFLECTOR_H_

#include <algorithm>
#include <map>
#include <random>
#include <stdexcept>
#include <vector>

namespace s21 {

class Reflector {
 public:
  Reflector();
  explicit Reflector(const std::map<char, char>& reflector);
  char Forward(char letter);
  std::map<char, char> GetSettings();

 private:
  std::map<char, char> reflector_;
};

}  // namespace s21

#endif  // A4_CRYPTO_ENIGMA_REFLECTOR_H_
