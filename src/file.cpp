#include "file.hpp"

#include <cstring>

File::File(const std::string &name) : numReads_(0), numWrites_(0) {
  stream_.rdbuf()->pubsetbuf(nullptr, 0);
  stream_.open(name, std::ios::binary | std::ios::in | std::ios::out |
                         std::ios::trunc);

  if (!stream_) {
    throw std::runtime_error(std::strerror(errno));
  }
}

void File::read(void *buffer, int pageIndex, int numPages) {
  stream_.seekg(pageIndex * PAGE_SIZE, std::ios::beg);
  stream_.read((char *)buffer, numPages * PAGE_SIZE);
  numReads_ += numPages;
}

void File::write(void *buffer, int pageIndex, int numPages) {
  stream_.seekp(pageIndex * PAGE_SIZE, std::ios::beg);
  stream_.write((char *)buffer, numPages * PAGE_SIZE);
  numWrites_ += numPages;
}

int File::getNumReads() const { return numReads_; }

int File::getNumWrites() const { return numWrites_; }
