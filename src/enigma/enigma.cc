#include "enigma.h"

namespace s21 {

EnigmaMachine::EnigmaMachine(uint num_rotors) { SetNumRotors(num_rotors); }

void EnigmaMachine::SetNumRotors(uint num_rotors) {
  if (num_rotors == 0) {
    throw std::invalid_argument("The number of rotors cannot be zero");
  }

  for (uint i = 0; i < num_rotors; ++i) {
    rotors_.emplace_back();
  }
}

char EnigmaMachine::EncryptLetter_(char letter) {
  if (letter >= 'A' && letter <= 'Z') {
    for (const auto& rotor : rotors_) {
      letter = rotor.Forward(letter);
    }
    letter = reflector_->Forward(letter);
    for (auto it = rotors_.rbegin(); it != rotors_.rend(); ++it) {
      letter = it->Backward(letter);
    }

    for (auto it = rotors_.begin(); it != rotors_.end() && it->Rotate(); ++it) {
    }
  }

  return letter;
}

void EnigmaMachine::Process_(const std::filesystem::path& from,
                             const std::filesystem::path& to) {
  if (std::filesystem::is_directory(from)) {
    throw std::logic_error(from.string() + " is not a file");
  }

  std::ifstream from_file(from, std::ios::binary);
  std::ofstream to_file(to, std::ios::binary);

  if (!from_file.is_open() || !to_file.is_open() || reflector_ == nullptr ||
      rotors_.empty()) {
    throw std::invalid_argument("Encryption/decryption is not possible");
  }

  char c;

  while (from_file.get(c)) {
    to_file << EncryptLetter_(c);
  }

  from_file.close();
  to_file.close();
}

void EnigmaMachine::Encrypt(const std::filesystem::path& path) {
  Process_(path, path.string() + "_encoded");
}

void EnigmaMachine::Decrypt(const std::filesystem::path& path) {
  Process_(path, path.string() + "_decoded");
}

void EnigmaMachine::LoadSettings(const std::filesystem::path& path) {
  if (std::filesystem::is_directory(path)) {
    throw std::logic_error(path.string() + " is not a file");
  }

  std::ifstream settings(path, std::ios::binary);

  if (!settings.is_open()) {
    throw std::runtime_error("Settings cannot be loaded");
  }

  std::string key;
  std::string line;
  std::vector<uint> states;

  while (std::getline(settings, line)) {
    line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
    if (auto pos = line.find('='); pos != std::string::npos) {
      key = line.substr(0, pos);
      line = line.substr(++pos);
    } else {
      continue;
    }

    if (!line.empty()) {
      if (key == "states") {
        states = ReadArray_(line);
      } else if (key == "reflector") {
        LoadReflectorSettings_(line);
      } else if (key == "rotor") {
        LoadRotorSettings_(line, states);
      }
    }
  }
  settings.close();

  if (reflector_ != nullptr && CheckMatchSize_()) {
    throw std::invalid_argument("Invalid settings file format");
  }
}

void EnigmaMachine::SaveSettings(const std::filesystem::path& path) {
  if (std::filesystem::is_directory(path)) {
    throw std::logic_error(path.string() + " is not a file");
  }

  std::ofstream settings(path, std::ios::binary);

  if (!settings.is_open()) {
    throw std::runtime_error("Settings cannot be saved");
  }

  SaveStateSettings_(settings);
  SaveRotorSettings_(settings);
  SaveReflectorSettings_(settings);

  settings.close();
}

bool EnigmaMachine::CheckMatchSize_() {
  auto reflector_size = reflector_->GetSettings().size();
  return std::any_of(rotors_.begin(), rotors_.end(),
                     [&reflector_size](const auto& i) {
                       return i.GetSettings().first.size() != reflector_size;
                     });
}

void EnigmaMachine::LoadRotorSettings_(const std::string& line,
                                       const std::vector<uint>& states) {
  bool empty = states.empty();
  uint i = 0;

  rotors_.clear();
  for (const auto& [left_side, right_side] : ReadMap_(line)) {
    if (empty || i >= states.size()) {
      rotors_.emplace_back(left_side, right_side);
    } else {
      rotors_.emplace_back(left_side, right_side, states[i++]);
    }
  }
}

void EnigmaMachine::LoadReflectorSettings_(const std::string& line) {
  std::map<char, char> map;
  const auto [left_side, right_side] = ReadMap_(line).front();

  for (std::size_t i = 0; i < left_side.size(); ++i) {
    map.insert({left_side[i], right_side[i]});
  }

  reflector_ = std::make_unique<Reflector>(map);
}

void EnigmaMachine::SaveStateSettings_(std::ofstream& stream) {
  stream << "states=";
  for (std::size_t i = 0, rotors_size = rotors_.size(); i < rotors_size; ++i) {
    stream << rotors_[i].GetState();
    if (i != rotors_size - 1) {
      stream << ", ";
    }
  }
  stream << "\n";
}

void EnigmaMachine::SaveRotorSettings_(std::ofstream& stream) {
  stream << "rotor=";
  for (std::size_t i = 0, rotors_size = rotors_.size(); i < rotors_size; ++i) {
    const auto& [left_side, right_side] = rotors_[i].GetSettings();

    for (std::size_t j = 0, size = left_side.size(); j < size; ++j) {
      stream << left_side[j] << ":" << right_side[j];
      if (j != size - 1) {
        stream << ", ";
      }
    }
    if (i != rotors_size - 1) {
      stream << "; ";
    }
  }
  stream << "\n";
}

void EnigmaMachine::SaveReflectorSettings_(std::ofstream& stream) {
  if (reflector_ == nullptr) {
    return;
  }

  stream << "reflector=";
  auto map = reflector_->GetSettings();
  auto it = map.begin();
  for (std::size_t i = 0, map_size = map.size(); i < map_size; ++i, ++it) {
    stream << it->first << ":" << it->second;
    if (i != map_size - 1) {
      stream << ", ";
    }
  }
  stream << "\n";
}

std::vector<std::pair<std::vector<char>, std::vector<char>>>
EnigmaMachine::ReadMap_(const std::string& line) {
  std::vector<std::pair<std::vector<char>, std::vector<char>>> map;
  std::istringstream iss(line);
  std::string token;

  while (std::getline(iss, token, ';')) {
    std::vector<char> left_side;
    std::vector<char> right_side;
    std::string sub_token;
    std::istringstream iss2(token);

    while (std::getline(iss2, sub_token, ',')) {
      if (sub_token.size() != std::string("A:B").size()) {
        throw std::invalid_argument("Invalid settings file format");
      }

      left_side.push_back(sub_token.front());
      right_side.push_back(sub_token.back());
    }
    map.emplace_back(left_side, right_side);
  }

  return map;
}

std::vector<uint> EnigmaMachine::ReadArray_(const std::string& line) {
  std::vector<uint> states;
  std::istringstream iss(line);
  std::string token;

  while (std::getline(iss, token, ',')) {
    try {
      std::size_t pos = 0;
      auto st = std::stoi(token, &pos);

      if (pos == token.size()) {
        states.push_back(st);
      } else {
        throw std::invalid_argument("Invalid settings file format");
      }
    } catch (const std::exception& e) {
      throw std::invalid_argument("Invalid settings file format");
    }
  }

  return states;
}

}  // namespace s21
