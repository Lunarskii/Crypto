//
// Created by ruslan on 12.11.23.
//

#include <fstream>
#include <iostream>
#include <random>

#include "Huffman.h"

bool ProcessMenu();

int main() {
  while (ProcessMenu()) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
  }

  return 0;
}

bool ProcessMenu() {
  std::cout << "In case of decode, please place huffman.cfg in the same dir "
               "with encoded file\n"
               "1. Encode\n"
               "2. Decode\n"
               "3. Exit\n";
  static s21::Huffman huffman;
  int ch;
  std::cin >> ch;
  try {
    switch (ch) {
      case 1: {
        std::string filepath;
        std::cout << "Enter filepath\n";
        std::cin >> filepath;
        huffman.SetFile(filepath);
        huffman.Encode();
        std::cout << "Encoded" << '\n';
      } break;
      case 2: {
        std::string filepath;
        std::cout << "Enter filepath\n";
        std::cin >> filepath;
        huffman.SetFile(filepath);
        huffman.Decode();
        std::cout << "Decoded" << '\n';
      } break;
      case 3:
        return false;
      default:
        std::cout << "try again..\n";
    }
  } catch (std::runtime_error& e) {
    std::cout << e.what() << '\n';
  }

  return true;
}
