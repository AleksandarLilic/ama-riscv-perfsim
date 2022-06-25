#include "../include/op_fwd.h"

op_fwd::op_fwd(sys_intf_t *sys_intf, id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf)
{
    this->sys_intf = sys_intf;
    this->id_intf = id_intf;
    this->ex_intf = ex_intf;
    this->mem_intf = mem_intf;
}

void op_fwd::update()
{
    dependency_detection(id_intf, ex_intf, mem_intf);
#if LOG_DBG 
    LOG("    Dependency detection: ");
    LOG("        dd_rs1_ex: " << dd_rs1_ex);
    LOG("        dd_rs2_ex: " << dd_rs2_ex);
    LOG("        dd_rs1_mem: " << dd_rs1_mem);
    LOG("        dd_rs2_mem: " << dd_rs2_mem);
#endif
    op_fwd_rf(id_intf);
    op_fwd_bcs(id_intf);
    op_fwd_alu(id_intf);
}

void op_fwd::dependency_detection(id_intf_t *id_intf, ex_intf_t *ex_intf, mem_intf_t *mem_intf)
{
    // EX stage
    dd_rs1_ex = ((id_intf->rs1_addr_id != uint32_t(rf_t::x0_zero)) && 
        (id_intf->rs1_addr_id == ex_intf->rd_addr_ex) &&
        (ex_intf->rd_we_ex));
    dd_rs2_ex = ((id_intf->rs2_addr_id != uint32_t(rf_t::x0_zero)) &&
        (id_intf->rs2_addr_id == ex_intf->rd_addr_ex) &&
        (ex_intf->rd_we_ex));

    // MEM stage
    dd_rs1_mem = ((id_intf->rs1_addr_id != uint32_t(rf_t::x0_zero)) &&
        (id_intf->rs1_addr_id == mem_intf->rd_addr_mem) &&
        (mem_intf->rd_we_mem));
    dd_rs2_mem = ((id_intf->rs2_addr_id != uint32_t(rf_t::x0_zero)) &&
        (id_intf->rs2_addr_id == mem_intf->rd_addr_mem) &&
        (mem_intf->rd_we_mem));
}

void op_fwd::op_fwd_rf(id_intf_t *id_intf)
{
    // RF A
    id_intf->of_rf_a_sel_fwd_id = dd_rs1_mem &&
        ((id_intf->dec_alu_a_sel_id == uint32_t(alu_op_a_sel_t::rs1)) ||
            (id_intf->dec_branch_inst_id));

    // RF B
    id_intf->of_rf_b_sel_fwd_id = dd_rs2_mem &&
        ((id_intf->dec_alu_b_sel_id == uint32_t(alu_op_b_sel_t::rs2)) ||
            (id_intf->dec_branch_inst_id) || 
            (id_intf->dec_store_inst_id));
}

void op_fwd::op_fwd_bcs(id_intf_t *id_intf)
{
    // BC A
    id_intf->of_bc_a_sel_fwd_id = dd_rs1_ex &&
        (id_intf->dec_branch_inst_id) /* &&
        !bubble_load */;

    // BC B or DMEM din for store
    id_intf->of_bcs_b_sel_fwd_id = dd_rs2_ex &&
        (id_intf->dec_branch_inst_id || id_intf->dec_store_inst_id) /* &&
        !bubble_load */;
}

void op_fwd::op_fwd_alu(id_intf_t *id_intf)
{
    // Operand A
    if ((dd_rs1_ex) && ((id_intf->dec_alu_a_sel_id == uint32_t(alu_op_a_sel_t::rs1))) /* && (!bubble_load) */)
        id_intf->of_alu_a_sel_fwd_id = uint32_t(alu_op_a_sel_fwd_t::fwd_mem);   // forward previous ALU result
    else
        id_intf->of_alu_a_sel_fwd_id = id_intf->dec_alu_a_sel_id;   // don't forward

    // Operand B
    if ((dd_rs2_ex) && ((id_intf->dec_alu_b_sel_id == uint32_t(alu_op_b_sel_t::rs2))) /* && (!bubble_load) */)
        id_intf->of_alu_b_sel_fwd_id = uint32_t(alu_op_b_sel_fwd_t::fwd_mem);   // forward previous ALU result
    else
        id_intf->of_alu_b_sel_fwd_id = id_intf->dec_alu_b_sel_id;   // don't forward
}
