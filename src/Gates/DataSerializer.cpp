#include "SimX/DataSerializer.hpp"

namespace SimX {
  std::unordered_map<std::string, std::string> DataSerializer::ParseConfigFile(const std::string& filepath) {
    std::unordered_map<std::string, std::string> config_data;

    if (std::ifstream config_fs(filepath); config_fs) {
      for (std::string line; std::getline(config_fs, line);) {
        std::istringstream line_ss(line);

        if (std::string id, value; std::getline(std::getline(line_ss, id, '='), value)) {
          config_data[id] = value;
        }
      }

    } else {
      throw std::runtime_error("Error reading file " + filepath);
    }

    return config_data;
  }

  void DataSerializer::WriteConfigFile(const std::string&                                  filepath,
                                       const std::unordered_map<std::string, std::string>& config) {
    std::ofstream of(filepath);

    if (of.is_open()) {
      for (const auto& [id, val] : config) of << id << "=" << val << "\n";

      of.close();

    } else {
      throw std::runtime_error("Error opening file " + filepath);
    }
  }
}
