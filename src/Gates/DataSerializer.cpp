#include "Pixel/Pixel.hpp"
#include "Gates/DataSerializer.hpp"
#include "Gates/LogicCircuit.hpp"

namespace Gates {
  ReadStatus DataSerializer::ParseCircuitFile(const std::string& name, LogicCircuit& circuit) {
    if (name.empty()) return ReadStatus::ERR_EMPTY_NAME;

    if (!std::filesystem::is_directory(circuits_dir))  // Crear directorio si no exíste
      if (!std::filesystem::create_directory(circuits_dir)) return ReadStatus::ERR_CREATE_DIR;

    std::ifstream ifs(circuits_dir + name + circuits_ext);
    if (!ifs.is_open()) return ReadStatus::ERR_OPEN_FILE;

    // Resetear el circuito anterior

    circuit.gates_update_pending.clear();
    circuit.gates_update_forced.clear();
    circuit.gates_output.clear();
    circuit.gates_input.clear();
    circuit.gates.clear();

    circuit.outputing_gate.reset();
    circuit.selected_gate.reset();
    circuit.dragging_gate.reset();
    circuit.hovered_gate.reset();
    circuit.adding_gate.reset();

    circuit.outputing = false;
    circuit.dragging  = false;
    circuit.adding    = false;

    uint8_t stage = 0;

    // Leer línea por línea
    for (std::string line; std::getline(ifs, line);) {
      if (line == "GATES" || line == "CONNECTIONS") {
        stage++;
        continue;
      }

      std::vector<std::string> line_parsed;
      std::istringstream       line_stream(line);

      // Separar la línea actual con el separador
      for (std::string current_token; std::getline(line_stream, current_token, file_separator);) {
        line_parsed.push_back(current_token);
      }

      if (stage == 1) {  // Cargar puertas
        if (line_parsed.size() != 3) return ReadStatus::ERR_FILE_FORMAT;

        try {
          const px::UUID     uuid      = std::stoull(line_parsed[0]);
          const std::string& gate_type = line_parsed[1];

          const float pos_x =
              std::stof(line_parsed[2].substr(0, line_parsed[2].find_first_of(',')));
          const float pos_y =
              std::stof(line_parsed[2].substr(line_parsed[2].find_first_of(',') + 1));

          const glm::vec2 pos = {pos_x, pos_y};

          if (gate_type == "input") {
            circuit.AddGateDirect<InputGate>(pos, uuid);

          } else if (gate_type == "output") {
            circuit.AddGateDirect<OutputGate>(pos, uuid);

          } else if (gate_type == "or") {
            circuit.AddGateDirect<OrGate>(pos, uuid);

          } else if (gate_type == "and") {
            circuit.AddGateDirect<AndGate>(pos, uuid);

          } else if (gate_type == "not") {
            circuit.AddGateDirect<NotGate>(pos, uuid);

          } else if (gate_type == "xor") {
            circuit.AddGateDirect<XorGate>(pos, uuid);

          } else {
            return ReadStatus::ERR_FILE_FORMAT;
          }

        } catch (...) {
          return ReadStatus::ERR_FILE_FORMAT;
        }

      } else if (stage == 2) {  // Cargar conexiones
        if (line_parsed.size() != 2) return ReadStatus::ERR_FILE_FORMAT;

        // El uso del try permite capturar una excepción sin cerrar el progama
        try {
          // La función std::stoull transforma una std::string a un entero, lo que puede generar un
          // error si se intenta convertir un texto que no es un número La indexación de un array
          // (p. ej. line_parsed[1]) generará un error si no tiene tantos elementos
          const px::UUID from = std::stoull(line_parsed[0]);
          const px::UUID to   = std::stoull(line_parsed[1]);

          circuit.MakeConnection(from, to);

          // El bloque catch será llamado si ocurre una excepción
          // Se usa la elípsis (...) para capturar cualquier tipo de excepción
        } catch (...) {
          // Se notifica a la aplicación principal a través de un valor de retorno de error, en vez
          // de terminar la ejecución, para que muestre un mensaje a el usuario
          return ReadStatus::ERR_FILE_FORMAT;
        }

      } else {
        return ReadStatus::ERR_FILE_FORMAT;
      }
    }

    ifs.clear();
    ifs.close();

    return ifs.fail() ? ReadStatus::ERR_OTHER : ReadStatus::OK;
  }

  WriteStatus DataSerializer::WriteCircuitFile(const std::string&  name,
                                               const LogicCircuit& circuit,
                                               bool                overwrite) {
    if (name.empty()) return WriteStatus::ERR_EMPTY_NAME;
    const std::string filepath = circuits_dir + name + circuits_ext;

    if (std::filesystem::is_regular_file(filepath) && !overwrite)
      return WriteStatus::ERR_FILE_EXISTS;

    if (!std::filesystem::is_directory(circuits_dir))
      if (!std::filesystem::create_directory(circuits_dir)) return WriteStatus::ERR_CREATE_DIR;

    std::ofstream ofs(filepath);
    if (!ofs.is_open()) return WriteStatus::ERR_OPEN_FILE;

    ofs << "GATES" << file_newline;

    // Se escriben todas las puertas en el mismo formato
    for (auto&& gate : circuit.gates) {
      ofs << gate->id << file_separator << gate->getName() << file_separator
          << std::to_string(gate->pos) << file_newline;
    }

    ofs << "CONNECTIONS" << file_newline;

    // Se escriben todas las conexiones
    for (auto&& gate : circuit.gates) {
      for (auto&& output : gate->outputs) {
        ofs << gate->id << file_separator << output->id << file_newline;
      }
    }

    ofs.close();
    return ofs.fail() ? WriteStatus::ERR_OTHER : WriteStatus::OK;
  }

  // Listado simple de archivos que tengan la extensión ".gates"
  std::vector<std::string> DataSerializer::ListCircuitFiles() {
    std::vector<std::string> list;

    if (!std::filesystem::is_directory(circuits_dir))
      if (!std::filesystem::create_directory(circuits_dir))
        px::Logger::Die("Error creando la carpeta de circuitos");

    for (const auto& entry : std::filesystem::directory_iterator(circuits_dir)) {
      std::string name = entry.path().filename();

      if (!name.ends_with(".gates")) continue;

      name.erase(
          std::find_if(name.rbegin(), name.rend(), [](unsigned char ch) { return ch == '.'; })
                  .base() -
              1,
          name.end());

      if (!name.empty()) list.push_back(name);
    }

    return list;
  }
}
