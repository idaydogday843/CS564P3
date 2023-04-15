#include "join.hpp"

#include <array>
#include <cstdint>
#include <vector>

JoinAlgorithm getJoinAlgorithm() {
  // TODO: Return your chosen algorithm.
  // return JOIN_ALGORITHM_BNLJ;
  // return JOIN_ALGORITHM_SMJ;
  // return JOIN_ALGORITHM_HJ;
  throw std::runtime_error("not implemented: getJoinAlgorithm");
};

int join(File &file, int numPagesR, int numPagesS, char *buffer,
         int numFrames) {
  // TODO: Implement your chosen algorithm.
  // Currently, this function performs a nested loop join in memory. While it
  // will pass the correctness and I/O cost tests, it ignores the provided
  // buffer and instead reads table data into vectors. It will not satisfy the
  // constraints on peak heap memory usage for large input tables. You should
  // delete this code and replace it with your disk-based join algorithm
  // implementation.

  int pageIndexR = 0;
  int pageIndexS = pageIndexR + numPagesR;
  int pageIndexOut = pageIndexS + numPagesS;

  std::vector<Tuple> tuplesR(numPagesR * 512);
  file.read(tuplesR.data(), pageIndexR, numPagesR);

  std::vector<Tuple> tuplesS(numPagesS * 512);
  file.read(tuplesS.data(), pageIndexS, numPagesS);

  std::vector<Tuple> tuplesOut;

  for (const Tuple &tupleR : tuplesR) {
    for (const Tuple &tupleS : tuplesS) {
      if (tupleR.first == tupleS.first) {
        tuplesOut.emplace_back(tupleR.second, tupleS.second);
        break;
      }
    }
  }

  int numTuplesOut = (int)tuplesOut.size();
  int numPagesOut = numTuplesOut / 512 + (numTuplesOut % 512 != 0);
  file.write(tuplesOut.data(), pageIndexOut, numPagesOut);

  return numTuplesOut;
}
