#ifndef CIRCBUFFER_H
#define CIRCBUFFER_H

#include <Arduino.h>

// Allow debugging/regression testing under normal g++ environment.
#ifdef CB_DEBUG
#include <iostream>
using namespace std;
#endif

class CircBuffer {
public:
  CircBuffer(void* myBuffer, int myBufferLen, int myBlockSize = 0);

  inline int getCapacity(void) const;
  inline int getSize(void) const;
  inline int getSizeRemaining(void) const;
  inline int getBlockSize(void) const;

  // @return number of bytes written
  int write(const uint8_t* src, int srcLen);
  inline int write(const void* src, int srcLen);

  // @return number of bytes actually read
  int read(uint8_t* dest, int destLen);
  inline int read(void* dest, int destLen);
  int read(Print& stream, int destLen);

  // @return number of bytes copied
  int peek(uint8_t* dest, int destLen);
  int skip(int len);

  void clear(void);

#ifdef CB_DEBUG
  inline void status(const char *file = NULL, int line = 0) const;
#endif

private:
  uint8_t* buffer;
  int bufferLen;
  int blockSize;
  uint8_t*  writePtr;
  uint8_t* readPtr;
  int size;
  
  inline const uint8_t* getEndOfBuffer(void) const;
  // Declare but do not define, copying not permitted
  CircBuffer(CircBuffer const &a);
  const CircBuffer& operator=(CircBuffer const &a);
};


inline int CircBuffer::getCapacity(void) const
{
  return bufferLen;
}


inline int CircBuffer::getSize(void) const
{
  return size;
}


inline int CircBuffer::getSizeRemaining(void) const
{
  return bufferLen - size;
}


inline int CircBuffer::getBlockSize(void) const
{
  if (blockSize > 1)
    return blockSize;
  return 1;
}

inline int CircBuffer::write(const void* src, int srcLen)
{
  return write((const uint8_t*)src, srcLen);
}


inline int CircBuffer::read(void* dest, int destLen)
{
  return read((uint8_t*)dest, destLen);
}


inline const uint8_t* CircBuffer::getEndOfBuffer(void) const
{
  return buffer + bufferLen;
}


#ifdef CB_DEBUG
inline void CircBuffer::status(const char *file, int line) const
{
  cout << "--------------- ";
  if (file != NULL) {
    cout << "File: " << file;
    if (line)
      cout << ':' << line;
  }
  cout << endl;

  cout << "Capacity:        " << bufferLen << endl
       << "read ptr: " << (readPtr-buffer) << endl 
       << "write ptr: " << (writePtr-buffer) << endl;
}
#endif


#endif

