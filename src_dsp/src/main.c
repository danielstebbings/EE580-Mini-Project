/**
 * main.c
 */
#include <stdint.h>
#include <stdio.h>

#include "filter.h"
#include "data.h"
#include "iir.h"
//#include "chirp.h"
#include "mock_audio.h"

#define LP_FILE "../data/lp.csv"
#define BP_FILE "../data/bp.csv"
#define HP_FILE "../data/hp.csv"

iir_filter_t lp_filt = {
      lp_N_SOS,
      lp_N_NUM,
      lp_N_DEN,
      lp_G_SOS,
      lp_coeffs,
   };


iir_filter_t hp_filt = {
      hp_N_SOS,
      lp_N_NUM,
      lp_N_DEN,
      hp_G_SOS,
      hp_coeffs,
   };


iir_filter_t bp_filt = {
      bp_N_SOS,
      lp_N_NUM,
      lp_N_DEN,
      bp_G_SOS,
      bp_coeffs,
   }; 

// mock audiofile
extern FILE *llvl_fid;
extern FILE *buf_foid;


#define AUDIOBUF_LEN 512
float audiobufin[AUDIOBUF_LEN] = {0};
float audiobufout[AUDIOBUF_LEN] = {0};


int main(void)
{
   printf("Main \n");
   int bufit;
   for (bufit = 0; bufit < LLVL_N_SAMPLES / AUDIOBUF_LEN; bufit++) {
      printf("Buffer: %d \r",bufit);
      get_audio_buf(audiobufin, AUDIOBUF_LEN);
      sos_filter(audiobufin, AUDIOBUF_LEN, audiobufout,&lp_filt);
      write_audio_buf(audiobufout, AUDIOBUF_LEN);
   }
   printf("\n");

   fclose(llvl_fid);
   fclose(buf_foid);

   printf("Done! \n");

	return 0;
}
