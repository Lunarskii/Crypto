//
// Created by ruslan on 10.11.23.
//

#include <fstream>
#include <iostream>
#include <random>

#include "Des.h"

bool ProcessMenu();

uint64_t GenerateKey();

void SaveKeyToFile(const std::string &file, uint64_t key);

int main() {
  while (ProcessMenu()) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
  }

  return 0;
}

bool ProcessMenu() {
  std::cout << "1. Generate key\n"
               "2. Load key\n"
               "3. Encrypt\n"
               "4. Decrypt\n"
               "5. Exit\n";
  static s21::Des des;
  int ch;
  std::cin >> ch;
  try {
    switch (ch) {
      case 1: {
        std::string filepath;
        std::cout << "Enter filepath\n";
        std::cin >> filepath;
        SaveKeyToFile(filepath, GenerateKey());
        std::cout << "Generated" << '\n';
      } break;
      case 2: {
        std::string filepath;
        std::cout << "Enter filepath\n";
        std::cin >> filepath;
        des.LoadKey(filepath);
        std::cout << "Loaded" << '\n';
      } break;
      case 3: {
        std::string filepath;
        std::cout << "Enter filepath\n";
        std::cin >> filepath;
        des.SetFile(filepath);
        des.Encrypt();
        std::cout << "Encrypted" << '\n';
      } break;
      case 4: {
        std::string filepath;
        std::cout << "Enter filepath\n";
        std::cin >> filepath;
        des.SetFile(filepath);
        des.Decrypt();
        std::cout << "Decrypted" << '\n';
      } break;
      case 5:
        return false;
      default:
        std::cout << "try again..\n";
    }
  } catch (std::runtime_error &e) {
    std::cout << e.what() << '\n';
  }

  return true;
}

uint64_t GenerateKey() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> dist(
      0, std::numeric_limits<uint64_t>::max());
  return dist(gen);
}

void SaveKeyToFile(const std::string &file, uint64_t key) {
  std::ofstream out(file, std::ios::binary | std::ios::out);
  if (!out) {
    throw std::runtime_error("error in opening file");
  }
  out.write(reinterpret_cast<char *>(&key), sizeof(key));
  if (!out) {
    throw std::runtime_error("error in writing");
  }

  out.close();
}