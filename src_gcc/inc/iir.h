/**
 * @file iir.h
 * @brief IIR filter structure definitions and functions.
 */

#ifndef IIR_H_
#define IIR_H_

#include <stdint.h>

typedef float float32_t;

/// @brief SOS IIR Filter
typedef struct
{
    int nstages;
    float32_t (*num)[3];
    float32_t (*den)[2];
    float32_t  g;

} iir_filter_t;

void sos_filter(
    float32_t *input, uint32_t input_length,
    float32_t *output,
    iir_filter_t *filt
);

#endif