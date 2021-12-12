#ifndef GATES_DATASERIALIZER_H
#define GATES_DATASERIALIZER_H

#include "pch.hpp"

namespace Gates {
  class DataSerializer {
   public:
    static std::unordered_map<std::string, std::string> ParseConfigFile(const std::string& filepath);
    static void                                         WriteConfigFile(const std::string&                                  filepath,
                                                                        const std::unordered_map<std::string, std::string>& config);
  };
}

#endif
