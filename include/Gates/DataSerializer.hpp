#ifndef GATES_DATASERIALIZER_H
#define GATES_DATASERIALIZER_H

#include "pch.hpp"

namespace Gates {
  class LogicCircuit;

  enum class WriteStatus : uint8_t {
    UNSET           = 0,
    OK              = 1,
    ERR_EMPTY_NAME  = 2,
    ERR_FILE_EXISTS = 3,
    ERR_CREATE_DIR  = 4,
    ERR_OPEN_FILE   = 5,
    ERR_OTHER       = 6
  };

  enum class ReadStatus : uint8_t {
    UNSET           = 0,
    OK              = 1,
    ERR_EMPTY_NAME  = 2,
    ERR_OPEN_FILE   = 3,
    ERR_CREATE_DIR  = 4,
    ERR_FILE_FORMAT = 5,
    ERR_OTHER       = 6
  };

  inline bool is_error(const WriteStatus& status) { return (uint8_t)status > 1; }
  inline bool is_error(const ReadStatus& status) { return (uint8_t)status > 1; }

  static std::map<WriteStatus, const char*> WriteStatusReadableName = {
      {WriteStatus::UNSET, "UNSET"},
      {WriteStatus::OK, "Archivo guardado correctamente"},
      {WriteStatus::ERR_EMPTY_NAME, "El nombre no puede estar vacio"},
      {WriteStatus::ERR_FILE_EXISTS, "El archivo ya existe"},
      {WriteStatus::ERR_CREATE_DIR, "No se pudo crear la carpeta"},
      {WriteStatus::ERR_OPEN_FILE, "No se pudo abrir el archivo especificado"},
      {WriteStatus::ERR_OTHER, "Error guardando el archivo"},
  };

  static std::map<ReadStatus, const char*> ReadStatusReadableName = {
      {ReadStatus::UNSET, "UNSET"},
      {ReadStatus::OK, "Archivo cargado correctamente"},
      {ReadStatus::ERR_EMPTY_NAME, "El nombre no puede estar vacio"},
      {ReadStatus::ERR_OPEN_FILE, "No se pudo abrir el archivo especificado"},
      {ReadStatus::ERR_CREATE_DIR, "No se pudo crear la carpeta"},
      {ReadStatus::ERR_FILE_FORMAT, "El archivo tiene un formato no válido"},
      {ReadStatus::ERR_OTHER, "Error leyendo el archivo"},
  };

  class DataSerializer {
   public:
    static ReadStatus  ParseCircuitFile(const std::string& name, LogicCircuit& circuit);
    static WriteStatus WriteCircuitFile(const std::string& name, const LogicCircuit& circuit, bool overwrite);

    static std::vector<std::string> ListCircuitFiles();

    static constexpr const char* circuits_dir = "circuits/";
    static constexpr const char* circuits_ext = ".gates";

    static constexpr const char  file_separator = ';';
    static constexpr const char* file_newline   = "\n";
  };
}

#endif
