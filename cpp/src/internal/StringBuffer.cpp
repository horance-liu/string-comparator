#include "internal/StringBuffer.h"

StringBuffer::StringBuffer(const std::string& buff) : buff(buff) {
}

bool StringBuffer::isInRange(int pos) const {
  return pos < buff.size();
}

bool StringBuffer::startWithLetter(int pos) const {
  return isLetter(pos) || (isSlash(pos) && isDigit(pos + 1));
}

bool StringBuffer::startWithDigit(int pos) const {
  return isDigit(pos) || (isSlash(pos) && isLetter(pos + 1));
}

bool StringBuffer::isSlash(int pos) const {
  return isInRange(pos) && buff[pos] == '\\';
}

bool StringBuffer::isLetter(int pos) const {
  return isInRange(pos) && std::isalpha(buff[pos]);
}

bool StringBuffer::isDigit(int pos) const {
  return isInRange(pos) && std::isdigit(buff[pos]);
}

char StringBuffer::operator[](int pos) const {
  return buff[pos];
}

