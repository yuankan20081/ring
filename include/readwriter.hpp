#pragma once

#include <cstdint>

class ReadWriter
{
  virtual uint32_t Read(char buf[], uint32_t length) = 0;
  virtual uint32_t Write(const char data[], uint32_t length) = 0;
};
