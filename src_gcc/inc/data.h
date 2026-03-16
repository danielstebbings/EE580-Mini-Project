/**
 * @file data.h
 * @brief Filter coefficient data
 */
#ifndef DATA_H_
#define DATA_H_

#include <stdint.h>
#include <stdbool.h>

#include "iir.h"

// Low PASS ------------------
#define lp_N_SOS 5
#define lp_G_SOS 0.0001165f

const float32_t lp_num[5][3] = { {1.0000000f, 2.0000000f, 1.0000000f}, {1.0000000f, 2.0000000f, 1.0000000f}, {1.0000000f, 2.0000000f, 1.0000000f}, {1.0000000f, 2.0000000f, 1.0000000f}, {1.0000000f, 2.0000000f, 1.0000000f},  };
const float32_t lp_den[5][2] = { {-0.8806878f, 0.7613755f}, {-0.7177889f, 0.4355778f}, {-0.6202041f, 0.2404082f}, {-0.5644506f, 0.1289012f}, {-0.5389780f, 0.0779561f},  };

const iir_filter_t lp_filt = {
      lp_N_SOS,
      lp_num,
      lp_den,
      lp_G_SOS
   };
// End Low PASS ------------------

#endif  /* DATA_H_ */
