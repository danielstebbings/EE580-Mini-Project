/**
 * @file mock_audio.h
 * @brief Get audio from disk
 */

#include <stdio.h>
#include "mock_audio.h"


void get_audio_buf(float* buf, int len) {
    // Open file if not already open
    if (llvl_fid == NULL) {
        llvl_fid = fopen(LLVL_FILE, "rb");
        if (llvl_fid == NULL) {
            printf("Error. Not able to open file %s for writing.\n", LLVL_FILE);
        return;
        }
    }
    // Ensure we stay within buffer.
    if (llvl_fit + len > LLVL_N_SAMPLES) {
        if (len > LLVL_N_SAMPLES) {
            printf("Error. Requested buffer too large, max supported is %d.\n", LLVL_N_SAMPLES);
            return;
        } else {
            fclose(llvl_fid);
            llvl_fit = 0;
            llvl_fid = fopen(LLVL_FILE, "rb");
        };
    }

    size_t ret;
    ret = fread(buf, sizeof(float), len, llvl_fid);
    // cppreference 
    if (ret != len) {
         if (feof(llvl_fid)) {
            printf("Error reading .bin: unexpected end of file\n");
            fclose(llvl_fid);
            llvl_fid = NULL;
            llvl_fit = 0;
         }
        else if(ferror(llvl_fid)) {
            perror("Error reading .bin");
            fclose(llvl_fid);
            llvl_fid = NULL;
            llvl_fit = 0;
        }
    } else {
        // Successfully Read from file
        // Keep track of how many things we've read so far
        llvl_fit += len;
    }
}

void write_audio_buf(float* buf, int len) {
    if (buf_foid == NULL) {
        buf_foid = fopen(BUFOUT_FILE, "w");
        if (buf_foid == NULL) {
            printf("Error. Not able to open file %s for writing.\n", BUFOUT_FILE);
            return;
        }
    };

    int i;
    for (i = 0; i < len; i++)
    {
        fprintf(buf_foid, "%f,", buf[i]);
    }


}
