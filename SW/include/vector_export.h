#pragma once
#include <fstream>

class vector_export
{
public:
    inline static std::ofstream table;
    std::ofstream vector_txt;
private:
    const std::string name_table = "vector_table.txt";
    std::string chk_signal_id;
public:
    vector_export(std::string signal_id);
    void log_table();
    void log_vector_txt(uint32_t value);
};
