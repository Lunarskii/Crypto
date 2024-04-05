#include <iostream>

#include "enigma.h"

bool ProcessMenu();
unsigned GetChoice();

int main() {
  while (ProcessMenu()) {
  }

  return 0;
}

bool ProcessMenu() {
  static s21::EnigmaMachine enigma;
  static std::string path;

  try {
    switch (GetChoice()) {
      case 1:
        std::cout << "\tSet the path to the file\n";
        std::cin >> path;
        enigma.LoadSettings(path);
        break;
      case 2:
        std::cout << "\tSet the path to the file\n";
        std::cin >> path;
        enigma.SaveSettings(path);
        break;
      case 3:
        std::cout << "\tEnter n\n";
        static uint num_rotors;
        while (!(std::cin >> num_rotors)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        break;
      case 4:
        std::cout << "\tSet the path to the file\n";
        std::cin >> path;
        enigma.Encrypt(path);
        break;
      case 5:
        std::cout << "\tSet the path to the file\n";
        std::cin >> path;
        enigma.Decrypt(path);
        break;
      case 6:
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

  std::cout << "\t1. Load settings from config file\n"
               "\t2. Save settings to config file\n"
               "\t3. Set number of rotors\n"
               "\t4. Encrypt\n"
               "\t5. Decrypt\n"
               "\t6. Exit\n";

  if (!(std::cin >> chooser)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return chooser;
}