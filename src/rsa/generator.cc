#include "generator.h"

namespace s21 {

PrimeNumberGenerator::size_type PrimeNumberGenerator::Generate() {
  mpz_class random_number;
  do {
    random_number = rng_.get_z_bits(1024);
  } while (!IsPrime(random_number));

  return random_number;
}

PrimeNumberGenerator::size_type PrimeNumberGenerator::Generate(
    const size_type& min, const size_type& max) {
  mpz_class random_number;
  auto range = max - min + 1;

  random_number = rng_.get_z_range(range);
  random_number += min;

  return random_number;
}

bool PrimeNumberGenerator::IsPrime(size_type n) {
  return mpz_probab_prime_p(n.get_mpz_t(), 15) == 1;
}

}  // namespace s21
