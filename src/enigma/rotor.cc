#include "rotor.h"

namespace s21 {

Rotor::Rotor() {
  for (char c = 'A'; c <= 'Z'; ++c) {
    static_side_.push_back(c);
  }
  dynamic_side_ = static_side_;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint> dist(0, static_side_.size() - 1);
  std::shuffle(dynamic_side_.begin(), dynamic_side_.end(), gen);

  dynamic_side_copy_ = dynamic_side_;
  n_ = dist(gen);
  n_copy_ = n_;
}

Rotor::Rotor(const std::vector<char>& static_side,
             const std::vector<char>& dynamic_side)
    : static_side_(static_side),
      dynamic_side_(dynamic_side),
      dynamic_side_copy_(dynamic_side) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint> dist(0, static_side_.size() - 1);

  n_ = dist(gen);
  n_copy_ = n_;
}

Rotor::Rotor(const std::vector<char>& static_side,
             const std::vector<char>& dynamic_side, const uint& n)
    : static_side_(static_side),
      dynamic_side_(dynamic_side),
      dynamic_side_copy_(dynamic_side),
      n_(n),
      n_copy_(n) {
  if (n_ >= static_side.size()) {
    throw std::invalid_argument(
        "The state cannot exceed the size of the rotor");
  }
}

char Rotor::Forward(char letter) const {
  auto it = std::find(static_side_.begin(), static_side_.end(), letter);

  if (it == static_side_.end()) {
    throw std::runtime_error("Rotor: The letter '" + std::to_string(letter) +
                             "' does not exist");
  }

  auto ix = std::distance(static_side_.begin(), it);
  return dynamic_side_[ix];
}

char Rotor::Backward(char letter) const {
  auto it = std::find(dynamic_side_.begin(), dynamic_side_.end(), letter);

  if (it == dynamic_side_.end()) {
    throw std::runtime_error("Rotor: The letter '" + std::to_string(letter) +
                             "' does not exist");
  }

  auto ix = std::distance(dynamic_side_.begin(), it);
  return static_side_[ix];
}

bool Rotor::Rotate() noexcept {
  if (n_ == static_side_.size() - 1) {
    n_ = 0;
  } else {
    ++n_;
  }
  std::rotate(dynamic_side_.begin(), dynamic_side_.begin() + 1,
              dynamic_side_.end());

  return n_ == 0;
}

std::pair<std::vector<char>, std::vector<char>> Rotor::GetSettings()
    const noexcept {
  return {static_side_, dynamic_side_copy_};
}

uint Rotor::GetState() const noexcept { return n_copy_; }

}  // namespace s21
