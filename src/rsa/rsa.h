#ifndef A4_CRYPTO_RSA_RSA_H_
#define A4_CRYPTO_RSA_RSA_H_

#include <bitset>
#include <filesystem>
#include <fstream>

#include "generator.h"

namespace s21 {

class RSA {
 public:
  using size_type = PrimeNumberGenerator::size_type;

  void GenerateKeys(const std::filesystem::path& path);
  void Encrypt(const std::filesystem::path& path,
               const std::filesystem::path& public_key);
  void Decrypt(const std::filesystem::path& path,
               const std::filesystem::path& private_key);

 private:
  size_type FindPublicExponent_(const size_type& phi);

  void ConvertCharArrayToBitset_(const unsigned char* arr,
                                 std::bitset<256>& bitset);
  std::pair<mpz_class, mpz_class> GetKey_(const std::filesystem::path& key);
};

}  // namespace s21

#endif  // A4_CRYPTO_RSA_RSA_H_
