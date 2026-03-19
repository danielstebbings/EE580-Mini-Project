#include "filter.h"
#include <math.h>
#include <stdio.h>
//#include <unistd.h>


void print_statistics(float32_t *p_y1, float32_t *p_y2, uint32_t p_y_len, uint32_t p_print_start, uint32_t p_print_n)
{
    uint32_t p_print_end = (p_print_start + p_print_n < p_y_len) ? (p_print_start + p_print_n) : p_y_len;

    printf("| Index |      y1      |      y2      |   Difference  |\n");
    printf("|-------|--------------|--------------|----------------|\n");
    uint32_t i;
    for(i = p_print_start; i < p_print_end; i++)
    {
        printf("| %3d | %12.6f | %12.6f | %12.6f |\n", i, p_y1[i], p_y2[i], fabsf(p_y1[i] - p_y2[i]));
    }

    // Statistics summary
    float32_t mean_y1 = 0, mean_y2 = 0, mean_diff = 0, total_diff = 0;
    for(i = 0; i < p_y_len; i++)
    {
        mean_y1 += p_y1[i];
        mean_y2 += p_y2[i];
        total_diff += fabsf(p_y1[i] - p_y2[i]);
    }
    mean_y1 /= p_y_len;
    mean_y2 /= p_y_len;
    mean_diff = total_diff / p_y_len;
    printf("Statistics Summary:\n");
    printf("Mean of y1: %f\n", mean_y1);
    printf("Mean of y2: %f\n", mean_y2);
    printf("Mean of Differences: %f\n", mean_diff);
    printf("Total of Differences: %f\n", total_diff);

}

void calculate_mean(uint32_t *p_input, uint32_t p_input_len, float32_t *p_output)
{
    uint32_t l_sum = 0;

    uint32_t i;
    for(i = 0; i < p_input_len; i++)
    {
        l_sum += p_input[i];
    }

    *p_output = (float32_t) l_sum / p_input_len;
}


void generate_signal(uint32_t *p_input, uint32_t p_input_len, float32_t *p_output, uint32_t p_output_len)
{
    float32_t l_mean = 0;
    calculate_mean(p_input, p_input_len, &l_mean);

    uint32_t l_repetitions = p_output_len / p_input_len;

    uint32_t i;
    for(i = 0; i < l_repetitions; i++)
    {
        uint32_t j;
        uint32_t l_ith = (uint32_t)i * p_input_len;
        for(j = 0; j < p_input_len; j++)
        {
            p_output[l_ith + j] = (float32_t) p_input[j] - l_mean;
            // printf("x1[%d] = %f \n", l_ith + j, p_output[l_ith + j]);
        }
    }
}


void filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output)
{
    uint32_t N = p_filter->coeff_b_len;
    float32_t *h = p_filter->coeff_b_ptr;

    uint32_t n;
    for (n = 0; n < p_input_len; n++)
    {
        float32_t l_acc = 0.0f;

        uint32_t k;
        for (k = 0; k < N; k++) {
            if (n >= k) {
                l_acc += p_input[n - k] * h[k];
            }
            else break;
        }

        p_output[n] = l_acc;
    }
}


void symm_filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output) 
{
    uint32_t N = p_filter->coeff_b_len;
    float32_t *h = p_filter->coeff_b_ptr;
    bool l_is_odd = (N % 2 != 0);

    uint32_t l_half_len = (N / 2);
    
    uint32_t n;
    for (n = 0; n < p_input_len; n++)
    {
        float32_t l_acc = 0.0f;
        
        // Loop through the first half of coefficients
        uint32_t k;
        for (k = 0; k < l_half_len; k++) {
            // Add symmetric taps before multiplying
            float32_t tap_sum = 0.0f;
            if (n >= k) 
            {
                tap_sum += p_input[n - k];
            }
            if (n >= (N - 1 - k)) 
            {
                tap_sum += p_input[n - (N - 1 - k)];
            }
            
            l_acc += tap_sum * h[k];
        }
        
        // Handle the middle tap if N is odd
        if (l_is_odd) {
            if (n >= l_half_len) {
                l_acc += p_input[n - l_half_len] * h[l_half_len];
            }
        }

        p_output[n] = l_acc;
    }
}



void record_output(float32_t *p_output, uint32_t p_output_len, const char *p_filename)
{
    FILE *l_file = fopen(p_filename, "w");

    if (l_file == NULL) 
    {
        printf("Error. Not able to open file %s for writing.\n", p_filename);
        return;
    }

    uint32_t i;
    for (i = 0; i < p_output_len; i++)
    {
        fprintf(l_file, "%f,", p_output[i]);
    }
    
    // remove trailing comma
    fseek(l_file, -1, SEEK_END);
//    ftruncate(fileno(l_file), ftell(l_file));

    fclose(l_file);
}


