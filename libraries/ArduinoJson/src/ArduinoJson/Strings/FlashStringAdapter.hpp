// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Polyfills/pgmspace.hpp>
<<<<<<< HEAD
#include <ArduinoJson/Strings/FlashStringIterator.hpp>
=======
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class FlashStringAdapter {
 public:
  FlashStringAdapter(const __FlashStringHelper* str) : _str(str) {}

  int compare(const char* other) const {
    if (!other && !_str)
      return 0;
    if (!_str)
      return -1;
    if (!other)
      return 1;
    return -strcmp_P(other, reinterpret_cast<const char*>(_str));
  }

  bool equals(const char* expected) const {
    return compare(expected) == 0;
  }

  bool isNull() const {
    return !_str;
  }

<<<<<<< HEAD
  void copyTo(char* p, size_t n) const {
    memcpy_P(p, reinterpret_cast<const char*>(_str), n);
=======
  char* save(MemoryPool* pool) const {
    if (!_str)
      return NULL;
    size_t n = size() + 1;  // copy the terminator
    char* dup = pool->allocFrozenString(n);
    if (dup)
      memcpy_P(dup, reinterpret_cast<const char*>(_str), n);
    return dup;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
  }

  size_t size() const {
    if (!_str)
      return 0;
    return strlen_P(reinterpret_cast<const char*>(_str));
  }

<<<<<<< HEAD
  FlashStringIterator begin() const {
    return FlashStringIterator(_str);
  }
=======
  typedef storage_policy::store_by_copy storage_policy;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const __FlashStringHelper* _str;
};

inline FlashStringAdapter adaptString(const __FlashStringHelper* str) {
  return FlashStringAdapter(str);
}

template <>
struct IsString<const __FlashStringHelper*> : true_type {};
}  // namespace ARDUINOJSON_NAMESPACE
