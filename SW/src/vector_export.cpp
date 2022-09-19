#include "../include/vector_export.h"

vector_export::vector_export(std::string signal_id)
{
    chk_signal_id = "chk_" + signal_id;
    std::string vector_export_name = chk_signal_id + ".txt";
    vector_txt.open(vector_export_name);
}


void vector_export::log_table()
{
    table.open(name_table, std::ios_base::app);
    table << chk_signal_id << std::endl;
    table.close();
}

void vector_export::log_vector_txt(uint32_t value)
{
    vector_txt << value << std::endl;
}
