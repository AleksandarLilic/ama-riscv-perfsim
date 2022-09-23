#include "../include/vector_export.h"

vector_export::vector_export()
{
    path_test = "test_" + global_test_name;
    std::filesystem::create_directory(path_test);
    name_table = base_name_table + ".txt";
    table.open(name_table);
    table << ""; // reset table
    table.close();
}

vector_export::vector_export(std::string signal_id)
{
    chk_signal_id = "chk_" + signal_id;
    std::string vector_export_name = path_test + "/" + chk_signal_id + ".txt";
    vector_txt.open(vector_export_name);
}

void vector_export::log_table()
{
    table.open(name_table, std::ios_base::app);
    if (!table) {
        LOGE("Failed to open vector_table log for signal: " << chk_signal_id);
    }
    table << chk_signal_id << std::endl;
    table.close();
}

void vector_export::log_vector_txt(uint32_t value)
{
    vector_txt << value << std::endl;
}
