//
// Created by ruslan on 12.11.23.
//

#include "Huffman.h"

#include <bitset>
#include <filesystem>
#include <iostream>
#include <queue>

namespace s21 {
void Huffman::SetFile(const std::string &file) {
  filename_ = file;
  if (!std::filesystem::exists(filename_) ||
      !std::filesystem::is_regular_file(filename_))
    throw std::runtime_error("wrong file");
}

void Huffman::Encode() {
  OpenFiles("_encoded");
  OpenConfFile(false);
  GetFreq();
  BuildCodes(BuildTree(), "");
  EncodeText();
  WriteConfig();
  CloseFiles();
}

shared_ptr<Huffman::Node> Huffman::BuildTree() {
  std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>,
                      comp>
      pq;
  auto total = GetInFileSize();
  for (auto const &[key, value] : freq_) {
    pq.push(std::make_shared<Node>(key, double(value) / double(total)));
  }
  while (pq.size() > 1) {
    auto left = pq.top();
    pq.pop();
    auto right = pq.top();
    pq.pop();
    auto node = std::make_shared<Node>(0, left->freq + right->freq);
    node->left = left;
    node->right = right;
    pq.push(node);
  }
  auto root = pq.top();
  pq.pop();
  return root;
}

void Huffman::GetFreq() {
  in_file_.clear();
  in_file_.seekg(0, std::ios::beg);
  int ch;
  while ((ch = in_file_.get()) != EOF) {
    ++freq_[ch];
  }
}

void Huffman::BuildCodes(const shared_ptr<Node> &root, const std::string &str) {
  if (!root) {
    return;
  }
  if (!root->left && !root->right) {
    codes_[root->byte] = str;
  }
  BuildCodes(root->left, str + "0");
  BuildCodes(root->right, str + "1");
}

void Huffman::EncodeText() {
  WriteText();
  WriteTotal();
}

void Huffman::WriteText() {
  int ch;
  std::string text;

  in_file_.clear();
  in_file_.seekg(0, std::ios::beg);
  while ((ch = in_file_.get()) != EOF) {
    auto code = codes_[ch];
    text += code;
    total_codes_ += code.size();
    while (text.size() >= 8) {
      WriteByte(text.substr(0, 8));
      text.erase(0, 8);
    }
  }
  if (!text.empty()) WriteByte(text);
}

void Huffman::WriteTotal() {
  std::bitset<64> total(total_codes_);
  auto text_set = total.to_string();
  for (int i = 0; i < 8; ++i) {
    WriteByte(text_set.substr(i * 8, 8));
  }
}

void Huffman::WriteByte(const std::string &str) {
  unsigned char buffer = 0;
  for (std::size_t i = 0; i < str.size(); ++i) {
    buffer |= (str[i] == '1') << (7 - i);
  }
  out_file_.write(reinterpret_cast<const char *>(&buffer), 1);
}

void Huffman::WriteConfig() {
  for (auto const &[key, value] : codes_) {
    conf_file_ << static_cast<unsigned>(key) << " " << value << "\n";
  }
}

void Huffman::Decode() {
  OpenFiles("_decoded");
  OpenConfFile(true);
  ReadConfig();
  ReadTotal();
  DecodeText();
  CloseFiles();
}

void Huffman::ReadConfig() {
  conf_file_ >> std::skipws;
  unsigned ch;
  std::string code;
  while (conf_file_ >> ch >> code) {
    codes_[ch] = code;
  }
}

void Huffman::DecodeText() {
  std::size_t idx = 0, code_size = 1;
  std::string text;

  in_file_.clear();
  in_file_.seekg(0, std::ios::beg);
  while (idx < total_codes_) {
    if (code_size > text.size()) text += ReadByte();
    auto sub_str = text.substr(0, code_size);
    for (auto const &[key, value] : codes_) {
      if (sub_str == value) {
        text.erase(0, code_size);
        code_size = 0;
        out_file_.write(reinterpret_cast<const char *>(&key), 1);
        break;
      }
    }
    ++idx;
    ++code_size;
  }
}

void Huffman::ReadTotal() {
  in_file_.seekg(-8, std::ios::end);
  std::string text_set;
  for (int i = 0; i < 8; ++i) {
    text_set += ReadByte();
  }
  total_codes_ = std::bitset<64>(text_set).to_ulong();
  in_file_.seekg(0, std::ios::beg);
}

std::string Huffman::ReadByte() {
  unsigned char ch;
  in_file_.read(reinterpret_cast<char *>(&ch), 1);
  if (!in_file_.gcount()) throw std::runtime_error("Nothing to read");
  return std::bitset<8>(ch).to_string();
}

void Huffman::OpenFiles(const std::string &mode) {
  in_file_.open(filename_, std::ios::in | std::ios::binary);
  if (!in_file_) throw std::runtime_error("in file can't be open");
  out_file_.open(filename_.string() + mode, std::ios::out | std::ios::binary);
  if (!out_file_) throw std::runtime_error("in file can't be open");
}

void Huffman::CloseFiles() {
  in_file_.close();
  out_file_.close();
  conf_file_.close();
}

std::size_t Huffman::GetInFileSize() {
  in_file_.seekg(0, std::ios::end);
  return in_file_.tellg();
}

void Huffman::OpenConfFile(bool read) {
  std::filesystem::path fs(filename_);
  conf_file_.open(fs.parent_path().string() + "/huffman.cfg",
                  read ? std::ios::in : std::ios::out);
  if (!conf_file_) throw std::runtime_error("conf file can't be open");
}
}  // namespace s21