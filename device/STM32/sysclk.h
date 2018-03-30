#pragma once

#ifdef __cplusplus
extern "C" {
#endif

uint32_t lmcu_hardware_clock();
uint32_t lmcu_apb1_clock();
uint32_t lmcu_apb2_clock();
uint32_t lmcu_adc_clock();

#ifdef __cplusplus
} // extern "C"
#endif
