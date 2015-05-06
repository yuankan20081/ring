#pragma once

#include "buffer.hpp"
#include "block.hpp"

class Ring : public Buffer
{
public:
  Ring(uint32_t node_size);
  ~Ring();
  virtual uint32_t Read(char buf[], uint32_t length) override;
  virtual uint32_t Write(const char data[], uint32_t length) override;
private:
  Block::POINTER reader;
  Block::POINTER writer;
  uint32_t node_size;
};

Ring::Ring(uint32_t node_size)
  :reader(nullptr), writer(nullptr), node_size(node_size)
{
  this->writer = Block::New(this->node_size);
  this->reader = this->writer;
  this->writer->Next = Block::New(this->node_size);
  this->writer->Next->Next = this->reader;
}

Ring::~Ring()
{
  this->reader->Next = nullptr;
}

uint32_t Ring::Read(char buf[], uint32_t length)
{
  uint32_t get = this->reader->Read(buf, length);
  uint32_t ex = 0;
  if(get<length){
    if(this->reader->ReadEOF() && this->reader!=this->writer){
      this->reader->RWInitialize();
      this->reader = this->reader->Next;
      ex = this->Read(buf+get, length-get);
    }
  }
  return get+ex;
}

uint32_t Ring::Write(const char data[], uint32_t length)
{
  uint32_t set = this->writer->Write(data, length);
  uint32_t ex = 0;
  if(set<length && this->writer->WriteEOF()){
    if(this->writer->Next==this->reader){
      this->writer->Next = Block::New(this->node_size);
      this->writer->Next->Next = this->reader;
    }
    this->writer = this->writer->Next;
    ex = this->Write(data+set, length-set);
  }
  return set+ex;
}

