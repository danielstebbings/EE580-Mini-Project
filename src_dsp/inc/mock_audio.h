/**
 * @file mock_audio.h
 * @brief Get audio from disk
 */

 #ifndef MOCK_AUDIO_H_
 #define MOCK_AUDIO_H_


#define DANIEL
//#define PREBEN

// length of llvl.bin
#define LLVL_N_SAMPLES 273105

#ifdef DANIEL
    #define LLVL_FILE "../data/llvl.bin"
    #define BUFOUT_FILE "../data/bufout.csv"
#else 
    #ifdef PREBEN
    #define LLVL_FILE "" // Fill w. yours!
    #endif
#endif

// file it. Persistent
static FILE *llvl_fid = NULL;
static int llvl_fit   = 0;

void get_audio_buf(float* buf, int len);

static FILE *buf_foid = NULL;
void write_audio_buf(float* buf, int len) ;







#endif
