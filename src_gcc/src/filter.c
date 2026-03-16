#include "filter.h"
#include <math.h>
#include <stdio.h>
#include <unistd.h>


/**
 * @brief Prints a formatted table of two float arrays with their differences and statistical summary.
 * 
 * @param p_y1 Pointer to the first float32_t array
 * @param p_y2 Pointer to the second float32_t array
 * @param p_y_len Total length of both arrays
 * @param p_print_start Starting index for the table output (inclusive)
 * @param p_print_n Number of elements to print in the table
 * 
 * @details
 * This function displays:
 * - A formatted table with columns for Index, y1, y2, and their absolute difference
 *   for elements from p_print_start to min(p_print_start + p_print_n, p_y_len)
 * - A statistics summary including:
 *   - Mean value of y1 (calculated over entire array)
 *   - Mean value of y2 (calculated over entire array)
 *   - Mean absolute difference between arrays
 *   - Total absolute difference across all elements
 */
void print_statistics(float32_t *p_y1, float32_t *p_y2, uint32_t p_y_len, uint32_t p_print_start, uint32_t p_print_n)
{
    uint32_t p_print_end = (p_print_start + p_print_n < p_y_len) ? (p_print_start + p_print_n) : p_y_len;

    printf("| Index |      y1      |      y2      |   Difference  |\n");
    printf("|-------|--------------|--------------|----------------|\n");
    for(uint32_t i = p_print_start; i < p_print_end; i++)
    {
        printf("| %3d | %12.6f | %12.6f | %12.6f |\n", i, p_y1[i], p_y2[i], fabsf(p_y1[i] - p_y2[i]));
    }

    // Statistics summary
    float32_t mean_y1 = 0, mean_y2 = 0, mean_diff = 0, total_diff = 0;
    for(uint32_t i = 0; i < p_y_len; i++)
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

/**
 * @brief Calculates the mean (average) of an array of unsigned integers.
 *
 * @param[in] p_input Pointer to an array of uint32_t values to be averaged.
 * @param[in] p_input_len The number of elements in the input array.
 * @param[out] p_output Pointer to a float32_t variable where the calculated mean will be stored.
 *
 * @return void
 *
 * @note The result is cast to float32_t to preserve fractional parts of the average.
 * @note No validation is performed on input parameters; ensure p_input is valid and p_input_len > 0.
 */
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


/**
 * @brief Generates a signal by repeating and mean-centering an input array.
 *
 * This function calculates the mean of the input array, then repeats the
 * mean-centered input values across the output buffer. Each element of the
 * input is subtracted by the mean and stored in the output array for each
 * repetition cycle.
 *
 * @param p_input Pointer to the input array of uint32_t values.
 * @param p_input_len Length of the input array.
 * @param p_output Pointer to the output array of float32_t values where
 *                 the generated signal will be stored.
 * @param p_output_len Length of the output array. Should be a multiple of
 *                      p_input_len.
 *
 * @return void
 *
 */
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

/**
 * @brief Applies a FIR (Finite Impulse Response) filter to an input signal.
 *
 * This function performs convolution of the input signal with the filter coefficients
 * to produce a filtered output signal.
 *
 * @param[in] p_input Pointer to the input signal array
 * @param[in] p_input_len Length of the input signal
 * @param[in] p_filter Pointer to the FIR filter structure containing coefficients
 * @param[out] p_output Pointer to the output signal array where filtered results are stored
 *
 * @note The filter coefficients are accessed from p_filter->coeff_b_ptr with length p_filter->coeff_b_len.
 *
 * @return void
 */
void filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output)
{
    uint32_t N = p_filter->coeff_b_len;
    float32_t *h = p_filter->coeff_b_ptr;

    for (uint32_t n = 0; n < p_input_len; n++) 
    {
        float32_t l_acc = 0.0f;

        for (uint32_t k = 0; k < N; k++) {
            if (n >= k) {
                l_acc += p_input[n - k] * h[k];
            }
            else break;
        }

        p_output[n] = l_acc;
    }
}

/**
 * @brief Applies a symmetric FIR filter to an input signal.
 * 
 * This function implements a symmetric finite impulse response (FIR) filter,
 * taking advantage of coefficient symmetry to process the input signal.
 * For each output sample, the filter computes a weighted sum using symmetric
 * tap pairs.
 * 
 * @param[in] p_input Pointer to the input signal array.
 * @param[in] p_input_len Length of the input signal.
 * @param[in] p_filter Pointer to the FIR filter structure containing coefficients
 *                      and filter length information.
 * @param[out] p_output Pointer to the output signal array where filtered results
 *                       are stored.
 * 
 * @return void
 *
 * @note Filter coefficients must exhibit symmetry: h[k] = h[N-1-k].
 */
void symm_filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output) 
{
    uint32_t N = p_filter->coeff_b_len;
    float32_t *h = p_filter->coeff_b_ptr;
    bool l_is_odd = (N % 2 != 0);

    uint32_t l_half_len = (N / 2);
    
    for (uint32_t n = 0; n < p_input_len; n++) 
    {
        float32_t l_acc = 0.0f;
        
        // Loop through the first half of coefficients
        for (uint32_t k = 0; k < l_half_len; k++) {
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


/**
 * @brief Records an array of floating-point values to a CSV file.
 * 
 * Writes the provided array of float32_t values to a file in comma-separated format,
 * removing the trailing comma from the output.
 * 
 * @param p_output Pointer to the array of float32_t values to be written.
 * @param p_output_len The number of elements in the p_output array.
 * @param p_filename The path to the output file. The file will be created or overwritten.
 * 
 * @return void
 * 
 * @note If the file cannot be opened, an error message is printed to stdout and the
 *       function returns without writing any data.
 * 
 * @warning This function uses platform-specific functions (ftruncate, fileno) that may
 *          not be portable to all systems (e.g., Windows).
 */
void record_output(float32_t *p_output, uint32_t p_output_len, const char *p_filename)
{
    FILE *l_file = fopen(p_filename, "w");

    if (l_file == NULL) 
    {
        printf("Error. Not able to open file %s for writing.\n", p_filename);
        return;
    }

    for (uint32_t i = 0; i < p_output_len; i++) 
    {
        fprintf(l_file, "%f,", p_output[i]);
    }
    
    // remove trailing comma
    fseek(l_file, -1, SEEK_END);
    ftruncate(fileno(l_file), ftell(l_file));

    fclose(l_file);
}


