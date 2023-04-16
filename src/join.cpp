#include "join.hpp"

#include <array>
#include <cstdint>
#include <vector>

JoinAlgorithm getJoinAlgorithm() {
  // TODO: Return your chosen algorithm.
  // return JOIN_ALGORITHM_BNLJ;
  return JOIN_ALGORITHM_SMJ;
  // return JOIN_ALGORITHM_HJ;
  throw std::runtime_error("not implemented: getJoinAlgorithm");
};

std::vector<Tuple> merge(std::vector<Tuple> unsorted1, std::vector<Tuple> unsorted2) {
  std::vector<Tuple> sortedTuples;
  int i, j;
  i = 0;
  j = 0;
  
  while (i < unsorted1.size() && j < unsorted2.size()) {
      if (unsorted1.at(i).first > unsorted2.at(j).first) {
          sortedTuples.emplace_back(unsorted2.at(j));
          j++;
      }
      else {
          sortedTuples.emplace_back(unsorted1.at(i));
          i++;
      }
  }
  if (i == unsorted1.size()) {
      while (j < unsorted2.size()) {
          sortedTuples.emplace_back(unsorted2.at(j));
          j++;
      }
  }
  else if (j == unsorted2.size()) {
      while (i < unsorted1.size()) {
          sortedTuples.emplace_back(unsorted1.at(i));
          i++;
      }
  }
  return sortedTuples;
}

std::vector<Tuple> mergeSort(std::vector<Tuple> unsortedTuples) {
    if (unsortedTuples.size() == 1) {
        return unsortedTuples;
    }
    std::vector<Tuple> unsorted1(unsortedTuples.size() / 2);
    std::vector<Tuple> unsorted2(unsortedTuples.size() / 2);

    for (int i = 0; i < unsortedTuples.size(); i++) {
        if (i < unsorted1.size()) {
            unsorted1.at(i) = unsortedTuples.at(i);
        }
        else {
            unsorted2.at(i - unsortedTuples.size() / 2) = unsortedTuples.at(i);
        }
    }
    unsorted1 = mergeSort(unsorted1);
    unsorted2 = mergeSort(unsorted2);

    return merge(unsorted1, unsorted2);

}

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
  tuplesR = mergeSort(tuplesR);
  tuplesS = mergeSort(tuplesS);
  std::vector<Tuple> tuplesOut;

  Tuple &tupleR = tuplesR.at(0);
  Tuple &tupleS = tuplesS.at(0);
  int indexR, indexS;
  indexR = 0;
  indexS = 0;
  while (tupleR != tuplesR.at(tuplesR.size() - 1) && tupleS != tuplesS.at(tuplesS.size() - 1)) {
    while (tupleR.first < tupleS.first) {
      indexR++;
      tupleR = tuplesR.at(indexR);
    }
    while (tupleR.first > tupleS.first) {
      indexS++;
      tupleS = tuplesS.at(indexS);
    }
    Tuple &tupleT = tupleS;
    int indexT = indexS;
    while (tupleR.first == tupleS.first) {
      tuplesOut.emplace_back(tupleR.second, tupleS.second);
      indexS++;
      tupleS = tuplesS.at(indexS);
    }
    tupleS = tupleT;
    indexR++;
    indexS = indexT;
    tupleR = tuplesR.at(indexR);
  }
  // for (const Tuple &tupleR : tuplesR) {
  //   for (const Tuple &tupleS : tuplesS) {
  //     if (tupleR.first == tupleS.first) {
  //       tuplesOut.emplace_back(tupleR.second, tupleS.second);
  //       break;
  //     }
  //   }
  // }

  int numTuplesOut = (int)tuplesOut.size();
  int numPagesOut = numTuplesOut / 512 + (numTuplesOut % 512 != 0);
  file.write(tuplesOut.data(), pageIndexOut, numPagesOut);

  return numTuplesOut;
}