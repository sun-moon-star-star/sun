/**
 * @file serialize.h
 * @author sun-moon-star-star
 */

#ifndef SUN_SERIALIZE_SERIALIZE_H_
#define SUN_SERIALIZE_SERIALIZE_H_

#include <cstring>
#include <string>

namespace sun {

template <typename T>
std::string serialize(const T& t) {
  return std::string(reinterpret_cast<const char*>(&t), sizeof(T));
}

template <typename T>
void unserialize(const std::string& data, T* t) {
  std::memcpy(&t, data.data(), sizeof(T));
}

}  // namespace sun

#endif  // SUN_SERIALIZE_SERIALIZE_H_