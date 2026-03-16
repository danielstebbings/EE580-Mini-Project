/**
 * @file filter.h
 * @brief FIR filter structure definitions and functions. 
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <stdint.h>
#include <stdbool.h>

typedef float float32_t;

typedef struct {
    bool symmetric;
    uint32_t  coeff_b_len;
    uint32_t  coeff_a_len;
    float32_t *coeff_a_ptr;
    float32_t *coeff_b_ptr;
} FIR_filter_t;


void generate_signal(uint32_t *p_input, uint32_t p_input_len, float32_t *p_output, uint32_t p_output_len);
void calculate_mean(uint32_t *p_input, uint32_t p_input_len, float32_t *p_output);
void filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output);
void symm_filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output);
void print_statistics(float32_t *p_y1, float32_t *p_y2, uint32_t p_y_len, uint32_t p_print_start, uint32_t p_print_end);
void record_output(float32_t *p_output, uint32_t p_output_len, const char *p_filename);


#endif  /* FILTER_H_ */
