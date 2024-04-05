//
// Created by ruslan on 10.11.23.
//

#ifndef CRYPTO_DES_H
#define CRYPTO_DES_H
#include <bitset>
#include <filesystem>
#include <iostream>
#include <tuple>

#include "arrays.h"

namespace s21 {

class Des {
  using full_key_t = std::bitset<64>;
  using text_t = std::bitset<64>;
  using key_t = std::bitset<56>;
  using half_key_t = std::bitset<28>;
  using sub_key_t = std::bitset<48>;
  using exp_text_t = sub_key_t;
  using half_text_t = std::bitset<32>;
  using s6_text_t = std::bitset<6>;
  using s4_text_t = std::bitset<4>;

 public:
  void LoadKey(std::string &filepath);
  void Encrypt();
  void Decrypt();
  void SetFile(std::string &filepath);

 private:
  void Process(const std::string &postfix);
  void Permutate(const int *);
  void ConvertToBitset(const unsigned char *arr);
  void ConvertToCharset(unsigned char *arr);
  void SplitKey(Des::key_t key);
  void SplitText();
  void ExpText();
  void GenerateSubKeys();
  half_key_t &RotateLeft(Des::half_key_t &key, int shift);
  sub_key_t ApplyPC2(const half_key_t &left, const half_key_t &right);
  void TransformS();
  Des::s4_text_t TransformSBlock(const Des::s6_text_t &text, const int[4][16]);
  std::array<s6_text_t, 8> SplitTextForS(const Des::exp_text_t &text);
  half_text_t CombineSBlocks(const std::array<s4_text_t, 8> &text);
  void TransformP();
  key_t CutKey(full_key_t &key);
  void CombineLeftRight();

 private:
  bool inverse_{};
  exp_text_t exp_text_{};
  text_t text_;
  half_key_t left_key_{}, right_key_{};
  half_text_t left_text_{}, right_text_{};
  std::array<sub_key_t, 16> sub_keys_;
  const Data tables_;
  std::filesystem::path file_;
};

}  // namespace s21
#endif  // CRYPTO_DES_H
