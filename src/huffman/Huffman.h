//
// Created by ruslan on 12.11.23.
//

#ifndef CRYPTO_HUFFMAN_H
#define CRYPTO_HUFFMAN_H

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

namespace s21 {
using std::shared_ptr;

class Huffman {
 public:
  void SetFile(const std::string& file);
  void Encode();
  void Decode();

 private:
  struct Node {
    unsigned char byte;
    double freq;
    shared_ptr<Node> left;
    shared_ptr<Node> right;
    Node(unsigned char bt, double fr) : byte(bt), freq(fr) {}
  };
  struct comp {
    bool operator()(const shared_ptr<Node>& lhs,
                    const shared_ptr<Node>& rhs) const {
      return lhs->freq > rhs->freq;
    }
  };
  void OpenFiles(const std::string& mode);
  void OpenConfFile(bool read);
  void CloseFiles();
  void BuildCodes(const shared_ptr<Node>& root, const std::string& str);
  void GetFreq();
  void EncodeText();
  void DecodeText();
  void WriteByte(const std::string& str);
  std::string ReadByte();
  void WriteConfig();
  void ReadConfig();
  void WriteTotal();
  void ReadTotal();
  void WriteText();
  std::size_t GetInFileSize();
  shared_ptr<Node> BuildTree();

 private:
  std::unordered_map<unsigned char, int> freq_{};
  std::unordered_map<unsigned char, std::string> codes_{};
  uint64_t total_codes_{};
  std::filesystem::path filename_;
  std::ifstream in_file_;
  std::ofstream out_file_;
  std::fstream conf_file_;
};

}  // namespace s21

#endif  // CRYPTO_HUFFMAN_H
