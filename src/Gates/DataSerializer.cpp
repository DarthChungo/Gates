#include "Gates/DataSerializer.hpp"
#include "Gates/LogicCircuit.hpp"
#include "Util/Logger.hpp"

namespace Gates {
  bool DataSerializer::ParseCircuitFile(const std::string& name, LogicCircuit& circuit) {
    if (name.empty()) return true;

    if (!std::filesystem::is_directory(circuits_dir))
      if (!std::filesystem::create_directory(circuits_dir)) return true;

    std::ifstream ifs(circuits_dir + name + circuits_ext);
    if (!ifs.is_open()) return true;

    for (std::string line; std::getline(ifs, line);) {
      std::istringstream line_ss(line);

      if (std::string id, value; std::getline(std::getline(line_ss, id, '='), value)) {
        Logger::Info(id + " -> " + value);
      }
    }

    ifs.close();
    return ifs.fail();
  }

  WriteStatus DataSerializer::WriteCircuitFile(const std::string& name, const LogicCircuit& circuit, bool overwrite) {
    if (name.empty()) return WriteStatus::ERR_EMPTY_NAME;
    const std::string filepath = circuits_dir + name + circuits_ext;

    if (std::filesystem::is_regular_file(filepath) && !overwrite) return WriteStatus::ERR_FILE_EXISTS;

    if (!std::filesystem::is_directory(circuits_dir))
      if (!std::filesystem::create_directory(circuits_dir)) return WriteStatus::ERR_CREATE_DIR;

    std::ofstream ofs(filepath);
    if (!ofs.is_open()) return WriteStatus::ERR_OPEN_FILE;

    ofs << "GATES" << file_newline;

    for (auto&& gate : circuit.gates) {
      ofs << gate->id << file_separator << gate->getName() << file_separator << std::to_string(gate->pos)
          << file_newline;
    }

    ofs << "CONECTIONS" << file_newline;

    for (auto&& gate : circuit.gates) {
      for (auto&& output : gate->outputs) {
        ofs << gate->id << file_separator << output->id << file_newline;
      }
    }

    ofs.close();
    return ofs.fail() ? WriteStatus::ERR_OTHER : WriteStatus::OK;
  }
}
