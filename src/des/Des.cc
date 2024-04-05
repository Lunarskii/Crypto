//
// Created by ruslan on 10.11.23.
//

#include "Des.h"

#include <fstream>

namespace s21 {
void Des::LoadKey(std::string &filepath) {
  std::ifstream in(filepath, std::ios::binary | std::ios::in);
  if (!in) {
    throw std::runtime_error("file can't be open");
  }
  unsigned char buffer[8] = {};
  in.read(reinterpret_cast<char *>(&buffer), 8);
  if (!in) {
    throw std::runtime_error("error in read");
  }
  full_key_t key{};
  for (int i = 0; i < 64; ++i) {
    key[63 - i] = (buffer[i / 8] >> (7 - i % 8)) & 1;
  }
  in.close();
  auto cut_key = CutKey(key);
  SplitKey(cut_key);
}

void Des::Encrypt() {
  inverse_ = false;
  Process("_encoded");
}

void Des::SetFile(std::string &filepath) {
  file_ = filepath;
  if (!std::filesystem::exists(file_) ||
      !std::filesystem::is_regular_file(file_))
    throw std::runtime_error("wrong file");
}

void Des::Permutate(const int *arr) {
  text_t result{};
  for (int i = 0; i < 64; ++i) {
    result[63 - i] = text_[64 - arr[i]];
  }
  text_ = result;
}

void Des::ConvertToBitset(const unsigned char *arr) {
  for (int i = 0; i < 64; ++i) {
    text_[63 - i] = (arr[i / 8] >> (7 - i % 8)) & 1;
  }
}

void Des::SplitKey(Des::key_t key) {
  for (int i = 0; i < 28; ++i) {
    left_key_[i] = key[i + 28];
    right_key_[i] = key[i];
  }
}

void Des::GenerateSubKeys() {
  for (int i = 0; i < 16; ++i) {
    auto idx = inverse_ ? 15 - i : i;
    sub_keys_[idx] =
        ApplyPC2(RotateLeft(left_key_, tables_.shift_schedule[i]),
                 RotateLeft(right_key_, tables_.shift_schedule[i]));
  }
}

Des::half_key_t &Des::RotateLeft(Des::half_key_t &key, int shift) {
  for (int i = 0; i < shift; ++i) {
    bool save = key[key.size() - 1];
    key <<= 1;
    key.set(0, save);
  }
  return key;
}

Des::sub_key_t Des::ApplyPC2(const half_key_t &left, const half_key_t &right) {
  key_t key{};
  for (int i = 0; i < 28; ++i) {
    key[i] = right[i];
    key[i + 28] = left[i];
  }
  sub_key_t sub_key{};
  for (int i = 0; i < 48; ++i) {
    sub_key[47 - i] = key[56 - tables_.PC_2[i]];
  }
  return sub_key;
}

void Des::SplitText() {
  for (int i = 0; i < 32; ++i) {
    left_text_[i] = text_[i + 32];
    right_text_[i] = text_[i];
  }
}

void Des::ExpText() {
  for (int i = 0; i < 48; ++i) {
    exp_text_[47 - i] = right_text_[32 - tables_.E_exp[i]];
  }
}

void Des::TransformS() {
  auto splited_text = SplitTextForS(exp_text_);
  std::array<s4_text_t, 8> result;
  for (int i = 0; i < 8; ++i) {
    result[i] = TransformSBlock(splited_text[i], tables_.SBOX[i]);
  }
  right_text_ = CombineSBlocks(result);
}

std::array<Des::s6_text_t, 8> Des::SplitTextForS(const Des::exp_text_t &text) {
  std::array<s6_text_t, 8> result;
  for (int i = 7; i >= 0; --i) {
    for (int j = 0; j < 6; ++j) {
      result[i][j] = text[(7 - i) * 6 + j];
    }
  }
  return result;
}

Des::s4_text_t Des::TransformSBlock(const Des::s6_text_t &text,
                                    const int arr[4][16]) {
  int row = (text[5] << 1) | text[0];
  int column = ((text[4] << 3) | (text[3] << 2) | (text[2] << 1) | text[1]);
  return arr[row][column];
}

Des::half_text_t Des::CombineSBlocks(const std::array<s4_text_t, 8> &text) {
  half_text_t result{};
  for (int i = 7; i >= 0; --i) {
    for (int j = 0; j < 4; ++j) {
      result[(7 - i) * 4 + j] = text[i][j];
    }
  }
  return result;
}

void Des::TransformP() {
  half_text_t result{};
  for (int i = 0; i < 32; ++i) {
    result[31 - i] = right_text_[32 - tables_.P_transform[i]];
  }
  right_text_ = result;
}

Des::key_t Des::CutKey(Des::full_key_t &key) {
  key_t result{};
  for (int i = 0; i < 56; ++i) {
    result[55 - i] = key[64 - tables_.PC_1[i]];
  }
  return result;
}

void Des::CombineLeftRight() {
  for (int i = 0; i < 32; ++i) {
    text_[i + 32] = left_text_[i];
    text_[i] = right_text_[i];
  }
}

void Des::ConvertToCharset(unsigned char *arr) {
  std::fill_n(arr, 8, 0);
  for (int i = 0; i < 64; ++i) {
    auto mask = text_[63 - i] << (7 - i % 8);
    arr[i / 8] |= mask;
  }
}

void Des::Decrypt() {
  inverse_ = true;
  Process("_decoded");
}

void Des::Process(const std::string &postfix) {
  if (left_key_.none() && right_key_.none())
    throw std::runtime_error("load the key");
  std::ifstream in(file_, std::ios::in | std::ios::binary);
  if (!in) throw std::runtime_error("input file can't be open");
  std::ofstream out(file_.string() + postfix, std::ios::out | std::ios::binary);
  if (!out) throw std::runtime_error("output file can't be open");
  GenerateSubKeys();
  do {
    unsigned char buffer[8] = {};
    in.read(reinterpret_cast<char *>(&buffer), 8);
    if (in.gcount() == 0) break;
    ConvertToBitset(buffer);
    Permutate(tables_.IP);
    SplitText();
    for (int i = 0; i < 16; ++i) {
      auto save = right_text_;
      ExpText();
      exp_text_ ^= sub_keys_[i];
      TransformS();
      TransformP();
      right_text_ ^= left_text_;
      left_text_ = save;
    }
    std::swap(left_text_, right_text_);
    CombineLeftRight();
    Permutate(tables_.IP_1);
    ConvertToCharset(buffer);
    out.write(reinterpret_cast<const char *>(&buffer), 8);
  } while (!in.eof());
}

}  // namespace s21