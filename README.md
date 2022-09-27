# RISC-V Core Cycle Accurate model

## **Overview**
**Description:**  
Cycle Accurate C++ implementation of RISC-V RV32I ISA  
Based on [ama-riscv Verilog simulator](https://github.com/AleksandarLilic/ama-riscv/blob/dev/verif/direct_tb/ama_riscv_core_dut_m.v)

**Use-cases:**
 - Stand-alone simulator
 - Performance model
 - Application development environment
 - Golden Model for RTL implementation through DPI or other

**Status:**   
In development  

**Milestones:**  
Passing all [RISC-V ISA tests](https://github.com/riscv-software-src/riscv-tests)  
Vector Export for [ama-riscv](https://github.com/AleksandarLilic/ama-riscv) core and [SystemVerilog testbench](https://github.com/AleksandarLilic/ama-riscv/blob/dev/verif/direct_tb/ama_riscv_core_top_tb.sv)

**Further development:**  
Performance, ease of use and architecture exploration capabilities comparison with Verilog simulator    
  
## **Project Structure**  
Header files: /SW/include/  
Source files: /SW/src/   
Documentation: /docs/  

## **Branches**  
**Main - stable release:** *main*  
**Documentation:** *doc*
