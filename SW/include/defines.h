#pragma once

#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define LOGW(x) std::cout << " >>> WARNING: "<< x << std::endl
#define LOGE(x) std::cout << " >>> ERROR: "<< x << std::endl

#define CL_UNUSED uint32_t(0xFFFF'FFFF)

// Memory sizes
#define IMEM_SIZE 16'384
#define DMEM_SIZE 16'384

// Instruction field masks
#define OPC7_M uint32_t(0x0000'007f)
#define FUNCT7_M uint32_t(0xfe00'0000)


