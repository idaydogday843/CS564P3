#ifndef CS564_PROJECT_3_JOIN_HPP
#define CS564_PROJECT_3_JOIN_HPP

#include "file.hpp"

enum JoinAlgorithm {
  JOIN_ALGORITHM_BNLJ, // Block nested loop join
  JOIN_ALGORITHM_SMJ,  // Sort merge join
  JOIN_ALGORITHM_HJ    // Hash join
};

using Tuple = std::pair<uint32_t, uint32_t>;
static_assert(sizeof(Tuple) == 8);

/**
 * Get the join algorithm.
 * @return Your chosen join algorithm.
 */
JoinAlgorithm getJoinAlgorithm();

/**
 * Join tables R and S. Refer to the README for more details.
 * @param file The database file. Refer to the documentation for more details.
 * @param numPagesR Number of pages in `R`.
 * @param numPagesS Number of pages in `S`.
 * @param buffer Contiguous memory region of buffer frames.
 * @param numFrames Number of frames in the buffer.
 * @return Number of tuples in the result.
 */
int join(File &file, int numPagesR, int numPagesS, char *buffer, int numFrames);

#endif // CS564_PROJECT_3_JOIN_HPP
