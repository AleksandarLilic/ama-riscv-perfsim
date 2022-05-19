#pragma once

#include <iostream>

#define LOG(x) std::cout << x << std::endl
#define LOGW(x) std::cout << " >>> WARNING: "<< x << std::endl
#define LOGE(x) std::cout << " >>> ERROR: "<< x << std::endl

#define CL_UNUSED uint32_t(0xFFFF'FFFF)