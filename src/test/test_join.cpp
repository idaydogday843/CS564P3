#include "join.hpp"
#include "test.hpp"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static int seed = 0;

void testJoinGetAlgorithm() {
  try {
    getJoinAlgorithm();
  } catch (const std::exception &e) {
    TEST_ASSERT(false, e.what());
  }
}

void testJoin(const std::string &name, int numPagesR, int numPagesS,
              int numFrames, int numTuplesShared, bool testIO = false) {
  // ---------------------------------------------------------------------------
  // Step 1: Generate the data.
  // ---------------------------------------------------------------------------

  std::minstd_rand rng(seed);
  std::uniform_int_distribution<uint32_t> dis(1, UINT32_MAX);

  int numTuplesR = numPagesR * 512;
  int numTuplesS = numPagesS * 512;

  std::vector<Tuple> tupleVectorShared;
  tupleVectorShared.reserve(numTuplesShared);

  std::unordered_set<uint32_t> keysShared;
  keysShared.reserve(numTuplesShared);
  while (keysShared.size() < numTuplesShared) {
    keysShared.emplace(dis(rng));
  }

  std::unordered_map<uint32_t, uint32_t> tupleSetR;
  tupleSetR.reserve(numTuplesR);

  std::unordered_map<uint32_t, uint32_t> tupleSetS;
  tupleSetS.reserve(numTuplesS);

  for (uint32_t a : keysShared) {
    uint32_t bR = dis(rng);
    uint32_t bS = dis(rng);
    tupleVectorShared.emplace_back(bR, bS);
    tupleSetR.emplace(a, bR);
    tupleSetS.emplace(a, bS);
  }

  while (tupleSetR.size() < numTuplesR) {
    uint32_t a = dis(rng);
    if (tupleSetR.find(a) == tupleSetR.end()) {
      tupleSetR.emplace(a, dis(rng));
    }
  }

  while (tupleSetS.size() < numTuplesS) {
    uint32_t a = dis(rng);
    if (tupleSetR.find(a) == tupleSetR.end() &&
        tupleSetS.find(a) == tupleSetS.end()) {
      tupleSetS.emplace(a, dis(rng));
    }
  }

  std::vector<Tuple> tupleVectorR(tupleSetR.begin(), tupleSetR.end());
  std::shuffle(tupleVectorR.begin(), tupleVectorR.end(), rng);

  std::vector<Tuple> tupleVectorS(tupleSetS.begin(), tupleSetS.end());
  std::shuffle(tupleVectorS.begin(), tupleVectorS.end(), rng);

  File file(name);
  file.write(tupleVectorR.data(), 0, numPagesR);
  file.write(tupleVectorS.data(), numPagesR, numPagesS);

  // ---------------------------------------------------------------------------
  // Step 2: Perform the join.
  // ---------------------------------------------------------------------------

  char *buffer = new char[numFrames * 4096];
  int numTuplesOut = join(file, numPagesR, numPagesS, buffer, numFrames);
  int numPagesOut = numTuplesOut / 512 + (numTuplesOut % 512 != 0);
  int numReads = file.getNumReads();
  int numWrites = file.getNumWrites() - numPagesR - numPagesS;

  // ---------------------------------------------------------------------------
  // Step 3: Validate result.
  // ---------------------------------------------------------------------------

  std::vector<Tuple> tupleVectorOut(numPagesOut * 512);
  file.read(tupleVectorOut.data(), numPagesR + numPagesS, numPagesOut);
  tupleVectorOut.resize(numTuplesOut);

  std::sort(tupleVectorOut.begin(), tupleVectorOut.end());
  std::sort(tupleVectorShared.begin(), tupleVectorShared.end());

  TEST_ASSERT(tupleVectorOut == tupleVectorShared, "incorrect result");

  // ---------------------------------------------------------------------------
  // Step 4: Validate I/O cost.
  // ---------------------------------------------------------------------------

  if (testIO) {
    JoinAlgorithm algorithm;
    try {
      algorithm = getJoinAlgorithm();
    } catch (const std::exception &e) {
      TEST_ASSERT(false, e.what());
    }

    int maxNumReads;
    int maxNumWrites;

    switch (algorithm) {
    case JOIN_ALGORITHM_BNLJ:
      maxNumReads = numPagesR + numPagesS * (int)std::ceil((double)numPagesR /
                                                           (numFrames - 2));
      maxNumWrites = numPagesR;
      break;
    case JOIN_ALGORITHM_SMJ:
    case JOIN_ALGORITHM_HJ:
      maxNumReads = 2 * (numPagesR + numPagesS);
      maxNumWrites = 2 * numPagesR + numPagesS;
      break;
    }

    TEST_ASSERT(numReads <= maxNumReads, "exceeded read limit");
    TEST_ASSERT(numWrites <= maxNumWrites, "exceeded write limit");
  }
}

void testCorrectness1() { testJoin("test.tbl", 1, 1, 4, 100); }

void testCorrectness2() { testJoin("test.tbl", 4, 4, 5, 400); }

void testCorrectness3() { testJoin("test.tbl", 16, 16, 8, 1600); }

void testCorrectness4() { testJoin("test.tbl", 1, 2, 4, 100); }

void testCorrectness5() { testJoin("test.tbl", 4, 8, 6, 400); }

void testCorrectness6() { testJoin("test.tbl", 16, 32, 9, 1600); }

void testIO1() { testJoin("test.tbl", 1, 1, 4, 100, true); }

void testIO2() { testJoin("test.tbl", 4, 4, 5, 400, true); }

void testIO3() { testJoin("test.tbl", 16, 16, 8, 1600, true); }

void testIO4() { testJoin("test.tbl", 1, 2, 4, 100, true); }

void testIO5() { testJoin("test.tbl", 4, 8, 6, 400, true); }

void testIO6() { testJoin("test.tbl", 16, 32, 9, 1600, true); }

int main(int argc, char **argv) {
  if (argc > 1) {
    seed = std::stoi(argv[1]);
  }

  TEST_RUN(testJoinGetAlgorithm);
  TEST_RUN(testCorrectness1);
  TEST_RUN(testCorrectness2);
  TEST_RUN(testCorrectness3);
  TEST_RUN(testCorrectness4);
  TEST_RUN(testCorrectness5);
  TEST_RUN(testCorrectness6);
  TEST_RUN(testIO1);
  TEST_RUN(testIO2);
  TEST_RUN(testIO3);
  TEST_RUN(testIO4);
  TEST_RUN(testIO5);
  TEST_RUN(testIO6);
  return TEST_EXIT_CODE;
}
