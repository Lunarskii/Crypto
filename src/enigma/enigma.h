#ifndef A4_CRYPTO_ENIGMA_ENIGMA_H_
#define A4_CRYPTO_ENIGMA_ENIGMA_H_

#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "reflector.h"
#include "rotor.h"

namespace s21 {

class EnigmaMachine {
 public:
  EnigmaMachine() = default;
  explicit EnigmaMachine(uint num_rotors);
  void SetNumRotors(uint num_rotors);
  void Encrypt(const std::filesystem::path& path);
  void Decrypt(const std::filesystem::path& path);
  void LoadSettings(const std::filesystem::path& path);
  void SaveSettings(const std::filesystem::path& path);

 private:
  char EncryptLetter_(char letter);
  void Process_(const std::filesystem::path& from,
                const std::filesystem::path& to);
  bool CheckMatchSize_();
  void LoadRotorSettings_(const std::string& line,
                          const std::vector<uint>& states);
  void LoadReflectorSettings_(const std::string& line);
  void SaveStateSettings_(std::ofstream& stream);
  void SaveRotorSettings_(std::ofstream& stream);
  void SaveReflectorSettings_(std::ofstream& stream);
  static std::vector<std::pair<std::vector<char>, std::vector<char>>> ReadMap_(
      const std::string& line);
  static std::vector<uint> ReadArray_(const std::string& line);

 private:
  std::vector<Rotor> rotors_;
  std::unique_ptr<Reflector> reflector_;
};

}  // namespace s21

#endif  // A4_CRYPTO_ENIGMA_ENIGMA_H_
