#pragma once
#include <fstream>
#include <filesystem>

#include "defines.h"

#if RISCV_ISA_REGR
extern std::string global_test_name;
#endif

class vector_export
{
public:
    std::ofstream vector_txt;
private:
    inline static std::string path_test;
    inline static std::ofstream table;
    inline static std::string name_test;
    const std::string base_name_table = "vector_table";
    inline static std::string name_table;
    std::string chk_signal_id;
public:
    vector_export();
    vector_export(std::string signal_id);
    void log_table();
    void log_vector_txt(uint32_t value);
};
