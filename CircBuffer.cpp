#include <CircBuffer.h>


CircBuffer::CircBuffer(void* myBuffer, int myBufferLen, int myBlockSize) :
  buffer((uint8_t*)myBuffer),
  bufferLen(myBufferLen),
  writePtr((uint8_t*)myBuffer),
  readPtr((uint8_t*)myBuffer)
{
  if (blockSize > 1)
    blockSize = myBlockSize;
  else
    // Fixed no fixed-size blocks
    blockSize = 0; 
}


int CircBuffer::write(const uint8_t* src, int srcLen)
{
  if (blockSize)
    // Round down to a multiple of the blockSize
    srcLen -= (srcLen % blockSize);
  
  for (int i = 0; i < srcLen; ++i) {
    *writePtr++ = *src++;
    if (writePtr >= getEndOfBuffer())
      writePtr = buffer;
  }
  size += srcLen;
  if (size > bufferLen) {
    // Overwrote existing data, adjust readPtr
    size = bufferLen;
    readPtr = writePtr;
    if (blockSize && size % blockSize) {
      // Skip some data to keep size a multiple of blockSize
      int tmp = size % blockSize;
      size -= tmp;
      readPtr += tmp;
      if (readPtr >= getEndOfBuffer())
	readPtr -= bufferLen; // Wrap around
    }
  }
  return srcLen;
}


int CircBuffer::read(uint8_t* dest, int destLen)
{
  if (destLen > size)
    destLen = size;

  if (blockSize)
    // Round down to a multiple of the blockSize
    destLen -= (destLen % blockSize);

  if (destLen > size)
    destLen = size;
  
  for (int i = 0; i < destLen; ++i) {
    *dest++ = *readPtr++;
    if (readPtr >= getEndOfBuffer())
      readPtr = buffer;
  }
  
  size -= destLen;
  return destLen;
}


int CircBuffer::read(Print& stream, int len)
{
  if (len > size)
    len = size;

  if (blockSize)
    // Round down to a multiple of the blockSize
    len -= (len % blockSize);

  
  for (int i = 0; i < len; ++i) {
    stream.write(readPtr++, 1);
    if (readPtr >= getEndOfBuffer())
      readPtr = buffer;
  }
  
  size -= len;
  return len;
}


int CircBuffer::peek(uint8_t* dest, int destLen)
{
  int s = size;
  uint8_t *p = readPtr;
  int r = read(dest, destLen);
  // Restore to original state
  size = s;
  readPtr = p;
  return r;
}


int CircBuffer::skip(int len)
{
  if (len > size)
    len = size;
  
  if (blockSize)
    // Round down to a multiple of the blockSize
    len -= (len % blockSize);

  readPtr += len;
  if (readPtr >= getEndOfBuffer())
    readPtr -= bufferLen;
  
  size -= len;
  return len;
}

