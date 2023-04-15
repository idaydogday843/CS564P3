#ifndef CS564_PROJECT_3_FILE_HPP
#define CS564_PROJECT_3_FILE_HPP

#include <cstdint>
#include <fstream>
#include <memory>
#include <string>

#define PAGE_SIZE 4096

class File {
public:
  explicit File(const std::string &name);

  void read(void *buffer, int pageIndex, int numPages = 1);

  void write(void *buffer, int pageIndex, int numPages = 1);

  int getNumReads() const;

  int getNumWrites() const;

private:
  std::fstream stream_;

  int numReads_;
  int numWrites_;
};

#endif // CS564_PROJECT_3_FILE_HPP
