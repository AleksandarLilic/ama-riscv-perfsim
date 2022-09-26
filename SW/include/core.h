#pragma once

#include <unordered_map>

#include "defines.h"
#include "intf.h"
#include "intf_cfg.h"
#include "modules.h"
#include "perf_cpu.h"
#include "vector_export.h"

#if RISCV_SANITY_TESTS
extern std::vector<uint32_t> global_committed_instructions;
#endif;

class core
{
private:
    vector_export v_exp;
    std::vector<vector_export *> v_exp_array;
    std::unordered_map<std::string, uint32_t*> internal_signals = {
        {"imem_addr", &if_intf.imem_addr}, {"inst_id", &id_intf.inst_id}
    };

public:
    void update_vectors();

private:
    intf_cfg intf_cfg;

    uint32_t *imem_dout_ptr;
    uint32_t *dmem_dout_ptr;

    sys_intf_t sys_intf{};

    if_intf_t if_intf{};
    id_intf_t id_intf{};
    ex_intf_t ex_intf{};
    mem_intf_t mem_intf{};
    wb_intf_t wb_intf{};
    
    reg_file_intf_t reg_file_intf{};
    csr_file_intf_t csr_file_intf{};

public:
    core() = delete;
    //core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr);
    core(seq_queue *q, core_intf_t *core_intf);
    ~core();

    void reset(bool rst_in);
    
    void update_system();
    void update_if();
    void update_id();
    void update_ex();
    void update_mem();
    void update_wb();

    void status_log();

private:
    // id
    control control;
    reg_file reg_file;
    imm_gen imm_gen;
    // ex
    branch_compare branch_compare;
    alu alu;
    store_shift store_shift;
    // mem
    load_shift_mask load_shift_mask;
    csr_file csr_file;
};
