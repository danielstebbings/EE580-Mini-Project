/**
 * @file filter.c
 * @brief FIR filter structure definitions and functions. 
 */


#include "iir.h"
#include "data.h"

// Tapped delay line for calculating SOS output
// n, n-1, n-2 for 
static float32_t tdl[3][lp_N_SOS+1] = {0};

void sos_filter(
    float32_t* input, uint32_t input_length,
    float32_t* output,
    iir_filter_t* filt
) {

    int N = filt->nstages;
    float32_t g = filt->g;
    float32_t num[3];
    float32_t den[2];
    

    int xit, sit;
    for (xit = 0; xit < input_length; xit++) {
        tdl[1][1] = input[xit];

        for (sit = 0; sit < N; sit++) {
            // get the filter values 
            num = {
                filt->num[sit][1],
                filt->num[sit][2],
                filt->num[sit][3]
            };
            den = filt->den[sit];


            tdl[1,sit+1] = (
                    tdl[1,sit]*num[1]+
                    tdl[2,sit]*num[2]+
                    tdl[3,sit]*num[3]
                ) - (
                    tdl[2,sit+1]*den[1]+
                    tdl[3,sit+1]*den[2]
                )

        }


        // TDL update
    }

    //
    






};