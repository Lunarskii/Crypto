#include <iostream>

#include "rsa.h"

bool ProcessMenu();
unsigned GetChoice();

int main() {
  while (ProcessMenu()) {
  }

  return 0;
}

bool ProcessMenu() {
  static s21::RSA rsa;
  static std::string path;

  try {
    switch (GetChoice()) {
      case 1:
        std::cout << "\tSpecify the directory\n";
        std::cin >> path;
        rsa.GenerateKeys(path);
        break;
      case 2:
        static std::string public_key;
        std::cout << "\tSpecify the path to the file\n";
        std::cin >> path;
        std::cout << "\tSpecify the path to the public key\n";
        std::cin >> public_key;
        rsa.Encrypt(path, public_key);
        break;
      case 3:
        static std::string private_key;
        std::cout << "\tSpecify the path to the file\n";
        std::cin >> path;
        std::cout << "\tSpecify the path to the private key\n";
        std::cin >> private_key;
        rsa.Decrypt(path, private_key);
        break;
      case 4:
        std::cout << "\tExiting...\n";
        return false;
        break;
      default:
        std::cout << "\tTry again...\n";
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return true;
}

unsigned GetChoice() {
  unsigned chooser = 0;

  std::cout << "\t1. Generate keys\n"
               "\t2. Encrypt\n"
               "\t3. Decrypt\n"
               "\t4. Exit\n";

  if (!(std::cin >> chooser)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return chooser;
}
