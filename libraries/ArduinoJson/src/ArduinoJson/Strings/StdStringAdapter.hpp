// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/IsString.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

#include <string>

namespace ARDUINOJSON_NAMESPACE {

template <typename TString>
class StdStringAdapter {
 public:
  StdStringAdapter(const TString& str) : _str(&str) {}

<<<<<<< HEAD:libraries/ArduinoJson/src/ArduinoJson/Strings/StdStringAdapter.hpp
  void copyTo(char* p, size_t n) const {
    memcpy(p, _str->c_str(), n);
=======
  char* save(MemoryPool* pool) const {
    size_t n = _str->length() + 1;
    char* dup = pool->allocFrozenString(n);
    if (dup)
      memcpy(dup, _str->c_str(), n);
    return dup;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575:libraries/ArduinoJson/src/ArduinoJson/Strings/StlStringAdapter.hpp
  }

  bool isNull() const {
    return false;
  }

  int compare(const char* other) const {
    if (!other)
      return 1;
    return _str->compare(other);
  }

  bool equals(const char* expected) const {
    if (!expected)
      return false;
    return *_str == expected;
  }

  size_t size() const {
    return _str->size();
  }

<<<<<<< HEAD:libraries/ArduinoJson/src/ArduinoJson/Strings/StdStringAdapter.hpp
  const char* begin() const {
    return _str->c_str();
  }
=======
  typedef storage_policy::store_by_copy storage_policy;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575:libraries/ArduinoJson/src/ArduinoJson/Strings/StlStringAdapter.hpp

  typedef storage_policies::store_by_copy storage_policy;

 private:
  const TString* _str;
};

template <typename TCharTraits, typename TAllocator>
struct IsString<std::basic_string<char, TCharTraits, TAllocator> > : true_type {
};

template <typename TCharTraits, typename TAllocator>
inline StdStringAdapter<std::basic_string<char, TCharTraits, TAllocator> >
adaptString(const std::basic_string<char, TCharTraits, TAllocator>& str) {
  return StdStringAdapter<std::basic_string<char, TCharTraits, TAllocator> >(
      str);
}

}  // namespace ARDUINOJSON_NAMESPACE
