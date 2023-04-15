#include "file.hpp"
#include "test.hpp"

#include <cstring>

void fileWriteRead() {
  File file("test.tbl");

  char data1[PAGE_SIZE];
  std::strcpy(data1, "abc");
  file.write(data1, 0);

  char data2[PAGE_SIZE];
  file.read(data2, 0);
  TEST_ASSERT(std::strncmp(data1, data2, 3) == 0, "expected equal strings");
}

void fileWriteReadOffset() {
  File file("test.tbl");

  char data1[PAGE_SIZE];
  std::strcpy(data1, "def");
  file.write(data1, 1);

  char data2[PAGE_SIZE];
  file.read(data2, 1);
  TEST_ASSERT(std::strncmp(data1, data2, 3) == 0, "expected equal strings");
}

void fileWriteReadMultiple() {
  File file("test.tbl");

  char data1[2 * PAGE_SIZE];
  std::strcpy(data1, "ghi");
  std::strcpy(&data1[PAGE_SIZE], "jkl");
  file.write(data1, 0, 2);

  char data2[2 * PAGE_SIZE];
  file.read(data2, 0, 2);
  TEST_ASSERT(std::strncmp(data1, data2, 3) == 0, "expected equal strings");
  TEST_ASSERT(std::strncmp(&data1[PAGE_SIZE], &data2[PAGE_SIZE], 3) == 0,
              "expected equal strings");
}

void fileNumReadsWrites() {
  File file("test.tbl");
  TEST_ASSERT(file.getNumReads() == 0, "expected 0");
  TEST_ASSERT(file.getNumWrites() == 0, "expected 0");

  char data[2 * PAGE_SIZE];

  file.write(data, 0);
  TEST_ASSERT(file.getNumWrites() == 1, "expected 1");

  file.read(data, 0);
  TEST_ASSERT(file.getNumReads() == 1, "expected 1");

  file.write(data, 0, 2);
  TEST_ASSERT(file.getNumWrites() == 3, "expected 3");

  file.read(data, 0, 2);
  TEST_ASSERT(file.getNumReads() == 3, "expected 3");
}

int main() {
  TEST_RUN(fileWriteRead);
  TEST_RUN(fileWriteReadOffset);
  TEST_RUN(fileWriteReadMultiple);
  TEST_RUN(fileNumReadsWrites);
  return TEST_EXIT_CODE;
}
