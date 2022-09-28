#pragma once

#include "defines.h"
#include <vector>

typedef struct core_intf_t
{
    uint32_t *imem_addr;    // core output  - cpu input
    uint32_t imem_dout;     // core input   - cpu output
    uint32_t *dmem_addr;    // core output  - cpu input
    uint32_t *dmem_din;     // core output  - cpu input
    uint32_t *dmem_en;      // core output  - cpu input
    uint32_t *dmem_we;      // core output  - cpu input
    uint32_t dmem_dout;     // core input   - cpu output
} core_intf_t;

// stage interfaces
typedef struct if_intf_t
{
    uint32_t imem_addr = NOT_RESET & (0x0FFF);
    uint32_t imem_addr_word_aligned = NOT_RESET & (0x0FFF);
    uint32_t pc_prepared = NOT_RESET & (0x0FFF);
    uint32_t pc_sel_if;
    uint32_t pc_we_if;
    uint32_t alu_out_ex;

} if_intf_t;

typedef struct id_intf_t
{
    uint32_t inst_id = NOT_RESET;

    uint32_t pc = NOT_RESET & (0x0FFF);
    uint32_t store_inst;
    uint32_t imm_gen_out;
    // reg file out
    uint32_t rf_data_a;
    uint32_t rf_data_b;
    uint32_t rf_data_a_fwd;
    uint32_t rf_data_b_fwd;

    uint32_t in_store_mask_offset;

    uint32_t opc7_id;
    uint32_t funct3_id;
    uint32_t funct7_id;

    // pipeline
    uint32_t stall_if_id;
    uint32_t stall_if_id_d;
    uint32_t clear_if_id;
    uint32_t clear_id_ex;
    uint32_t clear_ex_mem;
    uint32_t clear_mem_wb;

    // from control, as datapath control
    uint32_t dec_branch_inst_id;
    uint32_t dec_jump_inst_id;
    uint32_t dec_store_inst_id;
    uint32_t dec_load_inst_id;
    // fetch stage
    uint32_t dec_pc_sel_if;
    uint32_t dec_pc_we_if;
    uint32_t dec_ig_sel_id;
    // csr control
    uint32_t dec_csr_en_id;
    uint32_t dec_csr_we_id;
    uint32_t dec_csr_ui_id;
    // branching
    uint32_t dec_bc_uns_id;
    // alu
    uint32_t dec_alu_a_sel_id;
    uint32_t dec_alu_b_sel_id;
    uint32_t dec_alu_op_sel_id;
    // dmem
    uint32_t dec_store_mask_ex;
    uint32_t dec_dmem_en_id;
    uint32_t dec_load_sm_en_id;
    // writeback
    uint32_t dec_wb_sel_id;
    uint32_t dec_rd_we_id;

    // register addresses
    uint32_t rs1_addr_id;
    uint32_t rs2_addr_id;
    uint32_t rd_addr_id;
    // uint32_t rd_we_id; // as decoder output

    // csr
    uint32_t csr_data;
    uint32_t csr_uimm;
    
    // forwarding
    uint32_t of_rf_a_sel_fwd_id;
    uint32_t of_rf_b_sel_fwd_id;
    uint32_t of_bc_a_sel_fwd_id;
    uint32_t of_bcs_b_sel_fwd_id;
    uint32_t of_alu_a_sel_fwd_id;
    uint32_t of_alu_b_sel_fwd_id;

} id_intf_t;

typedef struct ex_intf_t
{
    uint32_t inst_ex = NOT_RESET;
    uint32_t pc_ex;
    uint32_t funct3_ex;
    uint32_t rs1_addr_ex;
    uint32_t rs2_addr_ex;
    uint32_t rd_addr_ex;
    uint32_t rd_we_ex;
    uint32_t rf_data_a_ex;
    uint32_t rf_data_b_ex;
    uint32_t imm_gen_out_ex;

    uint32_t csr_we_ex;
    uint32_t csr_ui_ex;
    uint32_t csr_uimm_ex;
    uint32_t csr_data_ex;

    uint32_t alu_a_sel_ex;
    uint32_t alu_b_sel_ex;
    uint32_t alu_op_sel_ex;

    uint32_t store_inst_ex;
    uint32_t branch_inst_ex;
    uint32_t jump_inst_ex;

    uint32_t bc_a_sel_ex;
    uint32_t bcs_b_sel_ex;
    uint32_t bc_uns_ex;
    uint32_t bc_in_a;
    uint32_t bcs_in_b;
    uint32_t bc_a_eq_b;
    uint32_t bc_a_lt_b;
    
    uint32_t alu_in_a = NOT_RESET;
    uint32_t alu_in_b = NOT_RESET;
    uint32_t alu_out = NOT_RESET;
    uint32_t store_offset;
    uint32_t dmem_addr = NOT_RESET & (0x0FFF);
    uint32_t dmem_din = NOT_RESET;
    uint32_t dmem_en_ex;

    uint32_t load_sm_en_ex;
    uint32_t wb_sel_ex;
} ex_intf_t;

typedef struct mem_intf_t
{
    uint32_t inst_mem = NOT_RESET;
    uint32_t alu_mem;
    uint32_t pc_mem;
    uint32_t funct3_mem;
    uint32_t rs1_addr_mem;
    uint32_t rs2_addr_mem;
    uint32_t rd_addr_mem;
    uint32_t rd_we_mem;

    uint32_t alu_in_a_mem = NOT_RESET;

    uint32_t csr_we_mem;
    uint32_t csr_ui_mem;
    uint32_t csr_uimm_mem;
    uint32_t csr_data_mem;

    uint32_t dmem_dout = NOT_RESET;
    uint32_t load_sm_en_mem;
    uint32_t wb_sel_mem;

    uint32_t load_sm_out = NOT_RESET;
} mem_intf_t;

typedef struct wb_intf_t
{
    uint32_t inst_wb = NOT_RESET;
    uint32_t data_d = NOT_RESET;

} wb_intf_t;

typedef struct reg_file_intf_t
{
    std::vector<uint32_t> in;
    std::vector<uint32_t> out;
    reg_file_intf_t() : in(32, NOT_RESET), out(32, NOT_RESET) { out[0] = 0u; } // Hardwired zero
} reg_file_t;

typedef struct csr_file_intf_t
{
    uint32_t tohost_in = NOT_RESET;
    uint32_t tohost_out = NOT_RESET;
} csr_file_intf_t;

// System interfaces
typedef struct sys_intf_t
{
    uint32_t rst;
    uint32_t rst_seq;
    uint32_t rst_seq_d;

    uint32_t rst_seq_id_ex;
    uint32_t rst_seq_ex_mem;
    uint32_t rst_seq_mem_wb;
} sys_intf_t;
