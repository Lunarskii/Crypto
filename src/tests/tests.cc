#include "tests.h"

namespace Test {

TEST_F(TestModule, des_00) {
  EXPECT_ANY_THROW(des.LoadKey(no_file));
  EXPECT_ANY_THROW(des.LoadKey(short_file));
  des.LoadKey(key);
  EXPECT_ANY_THROW(des.SetFile(no_file));
}

TEST_F(TestModule, des_01) {
  des.LoadKey(key);
  des.SetFile(file);
  des.Encrypt();
  des.SetFile(encoded_file);
  des.Decrypt();
  EXPECT_TRUE(CompareFiles(file, decoded_file));
  std::remove(encoded_file.c_str());
  std::remove(decoded_file.c_str());
}

TEST_F(TestModule, huffman_00) { EXPECT_ANY_THROW(huffman.SetFile(no_file)); }

TEST_F(TestModule, huffman_01) {
  huffman.SetFile(file);
  huffman.Encode();
  huffman.SetFile(encoded_file);
  huffman.Decode();
  EXPECT_TRUE(CompareFiles(file, decoded_file));
  std::remove(encoded_file.c_str());
  std::remove(decoded_file.c_str());
  std::remove(huffman_cfg.c_str());
}

TEST_F(TestModule, rsa_00) {
  EXPECT_ANY_THROW(rsa.GenerateKeys(no_file + '/'));
  EXPECT_ANY_THROW(rsa.GenerateKeys(no_file));
  EXPECT_ANY_THROW(rsa.Encrypt(no_file, no_file));
  EXPECT_ANY_THROW(rsa.Encrypt(file, no_file));
  EXPECT_ANY_THROW(rsa.Decrypt(file, no_file));
  EXPECT_ANY_THROW(rsa.Decrypt(file, no_file));
}

TEST_F(TestModule, rsa_01) {
  rsa.GenerateKeys(path);
  rsa.Encrypt(file, public_key);
  rsa.Decrypt(encoded_file, private_key);
  EXPECT_TRUE(CompareFiles(file, decoded_file));
  std::remove(encoded_file.c_str());
  std::remove(decoded_file.c_str());
  std::remove(private_key.c_str());
  std::remove(public_key.c_str());
}

TEST_F(TestModule, ENIGMA_RANDOM_NUMBER_ROTORS) {
  enigma.SetNumRotors(7);
  enigma.LoadSettings(enigma_only_reflector);
  enigma.SaveSettings(enigma_saved_config);
  enigma.Encrypt(enigma_text);
  enigma.LoadSettings(enigma_saved_config);
  enigma.Decrypt(enigma_text_encoded);
  EXPECT_TRUE(CompareFiles(enigma_text, enigma_text_decoded));
}

TEST_F(TestModule, ENIGMA_0_NUMBER_ROTORS) {
  EXPECT_ANY_THROW(enigma.SetNumRotors(0));
}

TEST_F(TestModule, ENIGMA_CONFIG_WITH_ALL_DATA) {
  enigma.LoadSettings(enigma_full_data);
  enigma.Encrypt(enigma_text);
  enigma.LoadSettings(enigma_full_data);
  enigma.Decrypt(enigma_text_encoded);
  EXPECT_TRUE(CompareFiles(enigma_text, enigma_text_decoded));
}

TEST_F(TestModule, ENIGMA_CONFIG_WITHOUT_ROTORS) {
  enigma.LoadSettings(enigma_without_rotors);
  EXPECT_ANY_THROW(enigma.Encrypt(enigma_text));
}

TEST_F(TestModule, ENIGMA_CONFIG_NOT_WITH_ALL_STATES) {
  enigma.LoadSettings(enigma_not_with_all_states);
  enigma.Encrypt(enigma_text);
  enigma.SaveSettings(enigma_saved_config);
  enigma.LoadSettings(enigma_saved_config);
  enigma.Decrypt(enigma_text_encoded);
  EXPECT_TRUE(CompareFiles(enigma_text, enigma_text_decoded));
}

TEST_F(TestModule, ENIGMA_CONFIG_WITHOUT_REFLECTOR) {
  enigma.LoadSettings(enigma_without_reflector);
  EXPECT_ANY_THROW(enigma.Encrypt(enigma_text));
}

TEST_F(TestModule, ENIGMA_INVALID_FILES) {
  EXPECT_ANY_THROW(enigma.LoadSettings(no_file));
  EXPECT_ANY_THROW(enigma.Encrypt(no_file));
  EXPECT_ANY_THROW(enigma.Decrypt(no_file));
}

bool CompareFiles(std::string &in_file, std::string &decoded_file) {
  std::ifstream in(in_file, std::ios::binary | std::ios::in),
      decoded_in(decoded_file, std::ios::binary | std::ios::in);
  if (!in || !decoded_in) return false;
  int ch1, ch2;
  while ((ch1 = in.get()) != EOF) {
    ch2 = decoded_in.get();
    if (ch1 != ch2) return false;
  }
  return true;
}

}  // namespace Test
