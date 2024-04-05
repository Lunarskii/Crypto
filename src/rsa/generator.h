#ifndef A4_CRYPTO_RSA_GENERATOR_H_
#define A4_CRYPTO_RSA_GENERATOR_H_

#include <gmpxx.h>

#include <chrono>
#include <cmath>
#include <ctime>
#include <limits>
#include <random>

namespace s21 {

class PrimeNumberGenerator {
 public:
  using size_type = mpz_class;

  PrimeNumberGenerator() : rng_(gmp_randinit_mt) {
    rng_.seed(std::chrono::system_clock::now().time_since_epoch().count());
  }
  size_type Generate();
  size_type Generate(const size_type& min, const size_type& max);

 private:
  bool IsPrime(size_type n);
  gmp_randclass rng_;
};

}  // namespace s21

#endif  // A4_CRYPTO_RSA_GENERATOR_H_
