/**
 * @file iir.h
 * @brief IIR filter structure definitions and functions.
 */

#ifndef IIR_H_
#define IIR_H_

#include <stdint.h>

//typedef float float;

#define N_NUM 3
#define N_DEN 2
#define SOS_N 5

#define BUFFER_SIZE 500

/// @brief SOS IIR Filter
/// coeffs stored as:
/// 
/// b_0, b_1, ..., b_(lnum-1), a1, a2, ..., a(lden-1) 
typedef struct iir_filter_t {
    int nstages;
    int lnum;
    int lden;
    float  g;
    float* coeffs;
    } iir_filter_t;


void sos_filter(
    float input[], uint32_t input_length,
    float output[],
    iir_filter_t* filt
);

void opt_sos_filter(
    float input[],// uint32_t input_length,
    float output[],
    iir_filter_t* filt
);

void clear_tdl();


#endif
