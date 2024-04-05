#ifndef CRYPTO_RSA_TESTS_H
#define CRYPTO_RSA_TESTS_H

#include <gtest/gtest.h>

#include "../des/Des.h"
#include "../enigma/enigma.h"
#include "../huffman/Huffman.h"
#include "../rsa/rsa.h"

namespace Test {

class TestModule : public ::testing::Test {
 protected:
  s21::Des des;
  s21::Huffman huffman;
  s21::RSA rsa;
  s21::EnigmaMachine enigma;
  std::string path{"tests/"};
  std::string no_file{"tests/not_exist"};
  std::string file{"tests/tests.cc"};
  std::string encoded_file{"tests/tests.cc_encoded"};
  std::string decoded_file{"tests/tests.cc_encoded_decoded"};
  std::string huffman_cfg{"tests/huffman.cfg"};
  std::string private_key{"tests/private_key"};
  std::string public_key{"tests/public_key"};
  std::string short_file{"tests/short_file"};
  std::string key{"tests/des_key"};
  std::string enigma_text{"tests/materials/enigma_text.txt"};
  std::string enigma_text_encoded{"tests/materials/enigma_text.txt_encoded"};
  std::string enigma_text_decoded{
      "tests/materials/enigma_text.txt_encoded_decoded"};
  std::string enigma_saved_config{"tests/materials/enigma_saved.conf"};
  std::string enigma_full_data{"tests/materials/enigma_full.conf"};
  std::string enigma_not_with_all_states{
      "tests/materials/enigma_not_with_all_states.conf"};
  std::string enigma_without_reflector{
      "tests/materials/enigma_without_reflector.conf"};
  std::string enigma_without_rotors{
      "tests/materials/enigma_without_rotors.conf"};
  std::string enigma_only_reflector{
      "tests/materials/enigma_only_reflector.conf"};
};

bool CompareFiles(std::string& in_file, std::string& decoded_file);

}  // namespace Test

#endif  // CRYPTO_RSA_TESTS_H
