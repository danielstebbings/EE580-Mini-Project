/**
 * @file main.c
 * @brief Main application file for FIR filtering of generated signals.
*/

#include <stdint.h>
#include "filter.h"
#include "data.h"

#define DATA_FILE_1 "./data1.txt"
#define DATA_FILE_2 "./data2.txt"

#define REG_LENGTH    9U
#define BUFF_SIZE     900U
#define REG1_LAST4    8874U
#define REG2_LAST4    4642U

FIR_filter_t g_FIR_1 = 
{
    .symmetric = true,
    .coeff_b_len = Filter_1_N_FIR_B,
    .coeff_a_len = Filter_1_N_FIR_A,
    .coeff_a_ptr = Filter_1_a_fir,
    .coeff_b_ptr = Filter_1_b_fir
};

FIR_filter_t g_FIR_2 = 
{
    .symmetric = true,
    .coeff_b_len = Filter_2_N_FIR_B,
    .coeff_a_len = Filter_2_N_FIR_A,
    .coeff_a_ptr = Filter_2_a_fir,
    .coeff_b_ptr = Filter_2_b_fir
};

/**
 * main.c
 */
int main(void)
{
    uint32_t l_reg1[REG_LENGTH] = { 2, 0, 2, 1, 1, 8, 8, 7, 4 }; // 202118874
    uint32_t l_reg2[REG_LENGTH] = { 2, 0, 2, 1, 1, 4, 6, 4, 2 }; // 202114642

    float32_t l_x1[BUFF_SIZE] = { 0 };
    float32_t l_x2[BUFF_SIZE] = { 0 };
    float32_t l_y1[BUFF_SIZE] = { 0 };
    float32_t l_y2[BUFF_SIZE] = { 0 };

    generate_signal(l_reg1, REG_LENGTH, l_x1, BUFF_SIZE);
    generate_signal(l_reg2, REG_LENGTH, l_x2, BUFF_SIZE);
    
    if(g_FIR_1.symmetric == true) 
        symm_filter_signal(l_x1, BUFF_SIZE, &g_FIR_1, l_y1);
    else
        filter_signal(l_x1, BUFF_SIZE, &g_FIR_1, l_y1);

    if(g_FIR_2.symmetric == true) 
        symm_filter_signal(l_x2, BUFF_SIZE, &g_FIR_2, l_y2);
    else
        filter_signal(l_x2, BUFF_SIZE, &g_FIR_2, l_y2);    
    
    record_output(l_y1, BUFF_SIZE, DATA_FILE_1);
    record_output(l_y2, BUFF_SIZE, DATA_FILE_2);

    print_statistics(l_y1, l_y2, BUFF_SIZE, 860, 50);
    
	return 0;
}
