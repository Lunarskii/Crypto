#ifndef A4_CRYPTO_ENIGMA_ROTOR_H_
#define A4_CRYPTO_ENIGMA_ROTOR_H_

#include <algorithm>
#include <random>
#include <stdexcept>
#include <vector>

namespace s21 {

class Rotor {
 public:
  Rotor();
  Rotor(const std::vector<char>& static_side,
        const std::vector<char>& dynamic_side);
  Rotor(const std::vector<char>& static_side,
        const std::vector<char>& dynamic_side, const uint& n);
  [[nodiscard]] char Forward(char letter) const;
  [[nodiscard]] char Backward(char letter) const;
  bool Rotate() noexcept;
  [[nodiscard]] std::pair<std::vector<char>, std::vector<char>> GetSettings()
      const noexcept;
  [[nodiscard]] uint GetState() const noexcept;

 private:
  std::vector<char> static_side_;
  std::vector<char> dynamic_side_;
  std::vector<char> dynamic_side_copy_;
  uint n_;
  uint n_copy_;
};

}  // namespace s21

#endif  // A4_CRYPTO_ENIGMA_ROTOR_H_
