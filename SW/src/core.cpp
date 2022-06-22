#include "../include/core.h"

void core::mem_init(uint32_t *imem_ptr, uint32_t *dmem_ptr) 
{ 
    LOG("mem_init called");
    this->imem_ptr = imem_ptr;  
    this->dmem_ptr = dmem_ptr;
    LOG("imem_ptr = " << this->imem_ptr);
    LOG("dmem_ptr = " << this->dmem_ptr);
};

void core::reset(bool rst_in)
{
    sys_intf.rst = rst_in;
}

void core::reset_seq(sys_intf_t *sys_intf)
{
    if(sys_intf->rst)
        sys_intf->rst_seq = 0b0111;
    else
        sys_intf->rst_seq = sys_intf->rst_seq>>1;
    
    sys_intf->rst_seq_d1 = (sys_intf->rst_seq & 0b100) >> 2;
    sys_intf->rst_seq_d2 = (sys_intf->rst_seq & 0b10) >> 1;
    sys_intf->rst_seq_d3 = sys_intf->rst_seq & 0b1;

    // LOG("sys_intf->rst_seq: " << sys_intf->rst_seq);
    // LOG("sys_intf->rst_seq_d1: " << sys_intf->rst_seq_d1);
    // LOG("sys_intf->rst_seq_d2: " << sys_intf->rst_seq_d2);
    // LOG("sys_intf->rst_seq_d3: " << sys_intf->rst_seq_d3);
}


void core::update_fe()
{
    front_end(&if_intf, &id_intf);
}

void core::update()
{
    reset_seq(&sys_intf);
    inst_parsing(&id_intf);
    control.update();
    reg_file.read();

    LOG("---------- inst in ID stage: " << std::hex << id_intf.inst_id << std::dec);
    reg_file.write();
}

#ifndef MULTI_LOGIC
core::core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr) :
    control(&sys_intf, &if_intf, &id_intf, &ex_intf, &mem_intf)
{
    mem_init(imem_ptr, dmem_ptr);
    intf_cfg.init_sys(q, &sys_intf);
    intf_cfg.init_if_id(q, &sys_intf, &if_intf, &id_intf, imem_ptr);
    intf_cfg.init_id_ex(q, &sys_intf, &id_intf, &ex_intf);
    intf_cfg.init_ex_mem(q, &sys_intf, &id_intf, &ex_intf, &mem_intf);
    //init(q);
    LOG("core queue constructor called");
}
#else // !MULTI_LOGIC
core::core(seq_queue *q, uint32_t *imem_ptr, uint32_t *dmem_ptr) :
    control(&sys_intf, &if_intf, &id_intf, &ex_intf, &mem_intf, &wb_intf),
    reg_file(&reg_file_intf, &id_intf, &mem_intf, &wb_intf)
{
    mem_init(imem_ptr, dmem_ptr);
    intf_cfg.init_regs(q, &sys_intf, &reg_file_intf, &if_intf, &id_intf, &ex_intf, &mem_intf, imem_ptr);
    //init(q);
    LOG("core queue constructor called");
}
#endif

// Datapath logic
#define ALU_OUT_MOCK 12

void core::front_end(if_intf_t *if_intf, id_intf_t *id_intf)
{
    LOG("Current PC: " << if_intf->pc << "; NX PC: " << id_intf->nx_pc);
    
    if_intf->imem_addr = id_intf->nx_pc;
    //if_intf->imem_addr = cl::mux2(uint32_t(id_intf->dec_pc_sel_if), id_intf->nx_pc, ALU_OUT_MOCK);
    if_intf->pc = id_intf->nx_pc + 1;

    //LOG("After update - PC: " << if_intf->pc << "; NX PC: " << id_intf->nx_pc);
}


void core::inst_parsing(id_intf_t *id_intf)
{
    LOG("--- inst_parsing");

    id_intf->opc7_id = inst_field::opc7(id_intf->inst_id);
    id_intf->funct3_id = inst_field::funct3(id_intf->inst_id);
    id_intf->funct7_id = inst_field::funct7(id_intf->inst_id);
    id_intf->rs1_addr_id = inst_field::rs1_addr(id_intf->inst_id);
    id_intf->rs2_addr_id = inst_field::rs2_addr(id_intf->inst_id);
    id_intf->rd_addr_id = inst_field::rd_addr(id_intf->inst_id);
}

// Core ID stage


//void core::ex_stage()
//{
//   uint32_t mux_out = cl::mux4(ctrl_if_ex.test_logic_type.get(), seq_dp_id_ex_if.rf_read_op_a, fwd_mem, pc_ex, CL_UNUSED);
//   
//   alu_in_a = mux_out;
//   core::alu(alu_in_a, alu_in_b, alu_sel, &alu_out);
//
//   dp_ex_mem.mux_out_ex = mux_out;
//}
//
//uint32_t core::alu()
//{
//    uint32_t local_alu = 0;
//    switch(alu_sel)
//        case 0x00: *alu_out = alu_in_a + alu_in_b; break;
//        case 0x08: *alu_out = alu_in_a - alu_in_b; break;
//
//}
