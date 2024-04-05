#include "rsa.h"

#include <cassert>

namespace s21 {

void RSA::GenerateKeys(const std::filesystem::path& path) {
  if (!std::filesystem::is_directory(path)) {
    throw std::logic_error(path.string() + " is not a directory");
  }

  std::ofstream public_key(path.string() + "public_key");
  std::ofstream private_key(path.string() + "private_key");

  if (!public_key.is_open() || !private_key.is_open()) {
    throw std::runtime_error("Keys cannot be created");
  }

  PrimeNumberGenerator generator;
  size_type p = generator.Generate(), q;
  do {
    q = generator.Generate();
  } while (q == p);
  size_type n = p * q;
  size_type phi = (p - 1) * (q - 1);
  size_type e = FindPublicExponent_(phi);
  size_type d;
  mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

  public_key << e.get_str().append("\n").append(n.get_str());
  private_key << d.get_str().append("\n").append(n.get_str());

  public_key.close();
  private_key.close();
}

void RSA::Encrypt(const std::filesystem::path& path,
                  const std::filesystem::path& public_key) {
  if (std::filesystem::is_directory(path) || !std::filesystem::exists(path)) {
    throw std::logic_error(path.string() + " is not a file");
  }

  std::ifstream encryption_file(path, std::ios::binary);
  std::ofstream encoded_file(path.string() + "_encoded", std::ios::binary);

  if (!encryption_file.is_open() || !encoded_file.is_open()) {
    throw std::runtime_error("File cannot be encrypted");
  }

  auto key = GetKey_(public_key);

  while (!encryption_file.eof()) {
    std::bitset<256> bitset;
    unsigned char buffer[32]{};

    encryption_file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
    if (!encryption_file.gcount()) break;
    ConvertCharArrayToBitset_(buffer, bitset);
    mpz_class encrypted_block;
    mpz_class block{bitset.to_string(), 2};
    mpz_powm_sec(encrypted_block.get_mpz_t(), block.get_mpz_t(),
                 key.first.get_mpz_t(), key.second.get_mpz_t());
    encoded_file << encrypted_block << " ";
  }

  encryption_file.close();
  encoded_file.close();
}

void RSA::Decrypt(const std::filesystem::path& path,
                  const std::filesystem::path& private_key) {
  if (std::filesystem::is_directory(path)) {
    throw std::logic_error(path.string() + " is not a file");
  }

  std::ifstream decryption_file(path, std::ios::binary);
  std::ofstream decoded_file(path.string() + "_decoded", std::ios::binary);

  if (!decryption_file.is_open() || !decoded_file.is_open()) {
    throw std::runtime_error("File cannot be decrypted");
  }

  auto prv_k = GetKey_(private_key);

  mpz_class encrypted_block;
  mpz_class decrypted_block;
  while (decryption_file >> encrypted_block) {
    mpz_powm(decrypted_block.get_mpz_t(), encrypted_block.get_mpz_t(),
             prv_k.first.get_mpz_t(), prv_k.second.get_mpz_t());
    std::bitset<256> bitset(decrypted_block.get_str(2));
    for (int i = 0; i < 32; ++i) {
      std::bitset<8> part(bitset.to_string().substr(i * 8, 8));

      if (part == 0) {
        break;
      }

      decoded_file << static_cast<char>(part.to_ulong());
    }
  }

  decryption_file.close();
  decoded_file.close();
}

RSA::size_type RSA::FindPublicExponent_(const size_type& phi) {
  PrimeNumberGenerator generator;
  size_type exponent;

  do {
    exponent = generator.Generate(2, phi - 1);
  } while (gcd(exponent, phi) != 1);

  return exponent;
}

void RSA::ConvertCharArrayToBitset_(const unsigned char* arr,
                                    std::bitset<256>& bitset) {
  uint16_t bitset_size = bitset.size();

  for (uint16_t i = 0; i < bitset_size; ++i) {
    bitset[bitset_size - 1 - i] = (arr[i / 8] >> (7 - i % 8)) & 1;
  }
}

std::pair<mpz_class, mpz_class> RSA::GetKey_(const std::filesystem::path& key) {
  std::ifstream key_file(key);

  if (!key_file.is_open()) {
    throw std::runtime_error("The key does not exist");
  }

  std::string first_part;
  std::string second_part;

  key_file >> first_part;
  key_file >> second_part;

  key_file.close();

  return std::make_pair(mpz_class(first_part), mpz_class(second_part));
}

}  // namespace s21
