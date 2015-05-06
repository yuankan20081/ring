#pragma once

#include "readwriter.hpp"
#include <memory>
#include <mutex>
#include <cstring>
#include <iostream>


class Block : public ReadWriter
{
public:
  typedef std::shared_ptr<Block> POINTER;
  POINTER Next;
private:
  char *buf;
  uint32_t size;
  bool reof;
  bool weof;
  uint32_t r;
  uint32_t w;
  std::mutex locker;
public:
  Block(uint32_t block_size);
  ~Block();
  void RWInitialize();
  bool ReadEOF() const;
  bool WriteEOF() const;
  virtual uint32_t Read(char buf[], uint32_t length) override;
  virtual uint32_t Write(const char data[], uint32_t length) override;
  static POINTER New(uint32_t block_size);
private:
  Block() = delete;
  Block(const Block &other) = delete;
};

Block::Block(uint32_t block_size)
  :Next(nullptr), buf(nullptr), size(block_size), reof(false)
  , weof(false)
  , locker(), r(0), w(0)
{
  this->buf = new char[this->size];
  std::cout << "ctor\n";
}

Block::~Block()
{
  if(nullptr!=this->buf)
    delete[] this->buf;
  std::cout << "dtor\n";
}

Block::POINTER Block::New(uint32_t block_size)
{
  return std::make_shared<Block>(block_size);
}

void Block::RWInitialize()
{
  this->r = this->w = 0;
  this->reof = this->weof = false;
}

uint32_t Block::Read(char buf[], uint32_t length)
{
  std::lock_guard<std::mutex> guard(this->locker);
  if(this->reof && (this->r==this->w)) return 0;
  uint32_t cached = this->w - this->r;
  uint32_t real = cached>=length ? length : cached;
  std::memmove(buf, this->buf+this->r, real);
  this->r += real;
  if(this->r>=this->size) this->reof = true;
  return real;
}

uint32_t Block::Write(const char data[], uint32_t length)
{
  std::lock_guard<std::mutex> guard(this->locker);
  if(this->weof) return 0;
  uint32_t left = this->size - this->w;
  uint32_t real = left>=length ? length : left;
  std::memcpy(this->buf+this->w, data, real);
  this->w += real;
  if(this->w>=this->size) this->weof = true;
  return real;
}

bool Block::ReadEOF() const
{
  return this->reof;
}

bool Block::WriteEOF() const
{
  return this->weof;
}
