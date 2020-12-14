// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Namespace.hpp>
#include <ArduinoJson/Strings/StoragePolicy.hpp>

namespace ARDUINOJSON_NAMESPACE {

class StringMover {
 public:
  StringMover(char* ptr) : _writePtr(ptr) {}

  void startString() {
    _startPtr = _writePtr;
  }

  const char* save() const {
    return _startPtr;
  }

  void append(char c) {
    *_writePtr++ = c;
  }

  bool isValid() const {
    return true;
  }

  const char* c_str() const {
    return _startPtr;
  }

<<<<<<< HEAD
  typedef storage_policies::store_by_address storage_policy;
=======
  // recover memory from last string
  void reclaim(const char* str) {
    _ptr = const_cast<char*>(str);
  }
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

 private:
  char* _writePtr;
  char* _startPtr;
};
}  // namespace ARDUINOJSON_NAMESPACE
