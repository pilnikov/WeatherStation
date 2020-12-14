// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Numbers/FloatTraits.hpp>
#include <ArduinoJson/Numbers/convertNumber.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/ctype.hpp>
#include <ArduinoJson/Polyfills/math.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantAs.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename A, typename B>
struct choose_largest : conditional<(sizeof(A) > sizeof(B)), A, B> {};

inline bool parseNumber(const char* s, VariantData& result) {
  typedef FloatTraits<Float> traits;
  typedef choose_largest<traits::mantissa_type, UInt>::type mantissa_t;
  typedef traits::exponent_type exponent_t;

  ARDUINOJSON_ASSERT(s != 0);

  bool is_negative = false;
  switch (*s) {
    case '-':
      is_negative = true;
      s++;
      break;
    case '+':
      s++;
      break;
  }

#if ARDUINOJSON_ENABLE_NAN
<<<<<<< HEAD
  if (*s == 'n' || *s == 'N') {
    result.setFloat(traits::nan());
    return true;
  }
=======
  if (*s == 'n' || *s == 'N')
    return traits::nan();
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
#endif

#if ARDUINOJSON_ENABLE_INFINITY
  if (*s == 'i' || *s == 'I') {
    result.setFloat(is_negative ? -traits::inf() : traits::inf());
    return true;
  }
#endif

  if (!isdigit(*s) && *s != '.')
<<<<<<< HEAD
    return false;
=======
    return return_type();
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

  mantissa_t mantissa = 0;
  exponent_t exponent_offset = 0;
  const mantissa_t maxUint = UInt(-1);

  while (isdigit(*s)) {
    uint8_t digit = uint8_t(*s - '0');
    if (mantissa > maxUint / 10)
      break;
    mantissa *= 10;
    if (mantissa > maxUint - digit)
      break;
    mantissa += digit;
    s++;
  }

<<<<<<< HEAD
  if (*s == '\0') {
    if (is_negative)
      result.setNegativeInteger(UInt(mantissa));
    else
      result.setPositiveInteger(UInt(mantissa));
    return true;
  }
=======
  if (*s == '\0')
    return return_type(TUInt(mantissa), is_negative);
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

  // avoid mantissa overflow
  while (mantissa > traits::mantissa_max) {
    mantissa /= 10;
    exponent_offset++;
  }

  // remaing digits can't fit in the mantissa
  while (isdigit(*s)) {
    exponent_offset++;
    s++;
  }

  if (*s == '.') {
    s++;
    while (isdigit(*s)) {
      if (mantissa < traits::mantissa_max / 10) {
        mantissa = mantissa * 10 + uint8_t(*s - '0');
        exponent_offset--;
      }
      s++;
    }
  }

  int exponent = 0;
  if (*s == 'e' || *s == 'E') {
    s++;
    bool negative_exponent = false;
    if (*s == '-') {
      negative_exponent = true;
      s++;
    } else if (*s == '+') {
      s++;
    }

    while (isdigit(*s)) {
      exponent = exponent * 10 + (*s - '0');
      if (exponent + exponent_offset > traits::exponent_max) {
        if (negative_exponent)
          result.setFloat(is_negative ? -0.0f : 0.0f);
        else
          result.setFloat(is_negative ? -traits::inf() : traits::inf());
        return true;
      }
      s++;
    }
    if (negative_exponent)
      exponent = -exponent;
  }
  exponent += exponent_offset;

  // we should be at the end of the string, otherwise it's an error
  if (*s != '\0')
<<<<<<< HEAD
    return false;

  Float final_result =
      traits::make_float(static_cast<Float>(mantissa), exponent);
=======
    return return_type();
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

  result.setFloat(is_negative ? -final_result : final_result);
  return true;
}

template <typename T>
inline T parseNumber(const char* s) {
  VariantData value;
  value.init();  // VariantData is a POD, so it has no constructor
  parseNumber(s, value);
  return variantAs<T>(&value);
}
}  // namespace ARDUINOJSON_NAMESPACE
