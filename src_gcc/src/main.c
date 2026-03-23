/**
 * @file main.c
 * @brief Main application file for FIR filtering of generated signals.
*/

#include <stdint.h>
#include "filter.h"
#include "data.h"
#include "iir.h"

#define LP_FILE "./lp.csv"
#define BP_FILE "./bp.csv"
#define HP_FILE "./hp.csv"

float32_t output[chirp_N] = {0};

const iir_filter_t lp_filt = {
      lp_N_SOS,
      lp_N_NUM,
      lp_N_DEN,
      lp_G_SOS,
      lp_coeffs,
   };

const iir_filter_t hp_filt = {
      hp_N_SOS,
      lp_N_NUM,
      lp_N_DEN,
      hp_G_SOS,
      hp_coeffs,
   };


const iir_filter_t bp_filt = {
      bp_N_SOS,
      lp_N_NUM,
      lp_N_DEN,
      bp_G_SOS,
      &bp_coeffs,
   };

/**
 * main.c
 */
int main(void)
{
    sos_filter(chirp_coeffs, chirp_N, output,&lp_filt);
    record_output(output, chirp_N, LP_FILE);

    sos_filter(chirp_coeffs, chirp_N, output,&hp_filt);
    record_output(output, chirp_N, HP_FILE);

    sos_filter(chirp_coeffs, chirp_N, output,&bp_filt);
    record_output(output, chirp_N, BP_FILE);

	return 0;
}
