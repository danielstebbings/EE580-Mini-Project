/**
 * @file filter.h
 * @brief FIR filter structure definitions and functions. 
 */

#ifndef FILTER_H_
#define FILTER_H_

#include <stdint.h>
#include <stdbool.h>

typedef float float32_t;

typedef struct {
    uint8_t symmetric;
    uint32_t  coeff_b_len;
    uint32_t  coeff_a_len;
    float32_t *coeff_a_ptr;
    float32_t *coeff_b_ptr;
} FIR_filter_t;

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
void generate_signal(uint32_t *p_input, uint32_t p_input_len, float32_t *p_output, uint32_t p_output_len);

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
void calculate_mean(uint32_t *p_input, uint32_t p_input_len, float32_t *p_output);

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
 * @note Filter coefficients p_filter->coeff_a are assumed = [ 1 ].
 *
 * @return void
 */
void filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output);

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
 * @note Filter coefficients p_filter->coeff_a are assumed = [ 1 ].
 */
void symm_filter_signal(float32_t *p_input, uint32_t p_input_len, FIR_filter_t *p_filter, float32_t *p_output);

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
void print_statistics(float32_t *p_y1, float32_t *p_y2, uint32_t p_y_len, uint32_t p_print_start, uint32_t p_print_end);

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
void record_output(float32_t *p_output, uint32_t p_output_len, const char *p_filename);


#endif  /* FILTER_H_ */
