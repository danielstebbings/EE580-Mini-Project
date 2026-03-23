/**
 * @file filter.c
 * @brief FIR filter structure definitions and functions. 
 */


#include "iir.h"

// Tapped delay line for calculating SOS output

// [ x[n],   s0[n],   ..., sN[n] -> y[n] ]
// [ x[n-1], s0[n-1], ..., sN[n-1] -> y[n-1] ]
// [ x[n-2], s0[n-2], ..., sN[n-2] -> y[n-2] ]



void sos_filter(
    float input[], uint32_t input_length,
    float output[],
    const iir_filter_t* filt
) {
    // TODO: Change this to match the highest of all 3, then use N sos in filt within.
    //       This will allow filters of different length.
    float tdl[3][SOS_N+1] = {0};
    float num[N_NUM];
    float den[N_DEN];

    unsigned int N = filt->nstages;
    float g = filt->g;  

    unsigned int xit, sit,cit, tit;
    for (xit = 0; xit < input_length; xit++) {
        tdl[0][0] = input[xit];

        for (sit = 0; sit < N; sit++) {
            // get the filter values 
            // Stored as 1D array of numerator1, denominator1, num2, den2, etc
            for (cit = 0; cit < N_NUM; cit++) {
            num[cit] = filt->coeffs[sit*(N_NUM+N_DEN)         + cit];
            };
            for (cit = 0; cit < N_DEN; cit++) {
            den[cit] = filt->coeffs[sit*(N_NUM+N_DEN) + N_NUM + cit];
            };           


            // y[n] = b0x[n] + b1x[n-1] + b2x[n-2] -a1y[n-1] - a2y[n-2]
            // In TDL: n is row 0, n-1 row 1, n-2 row 2, 
            // x[k] is tdl[][sit], y[k] is tdl[][sit+1]
            tdl[0][sit+1] = num[0]*tdl[0][sit]
                            + num[1]*tdl[1][sit]
                            + num[2]*tdl[2][sit]
                            - den[0]*tdl[1][sit+1]
                            - den[1]*tdl[2][sit+1];
        }

        // y[n] is output of last stage
        // multiply by gain
        output[xit] = g*tdl[0][SOS_N];

        // TDL update
        // TDL[0][] -> TDL[1][], TDL[1][] -> TDL[2][]
        // set TDL[0][] -> 0
        // We're gonna do this the slow way :))
        for (tit = 0; tit < SOS_N+1;  tit++) {
            tdl[2][tit] = tdl[1][tit];
            tdl[1][tit] = tdl[0][tit];
            tdl[0][tit] = 0.0f;
        };


};
}