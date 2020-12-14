// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Json/EscapeSequence.hpp>
#include <ArduinoJson/Json/Latch.hpp>
#include <ArduinoJson/Json/Utf16.hpp>
#include <ArduinoJson/Json/Utf8.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/Numbers/parseNumber.hpp>
#include <ArduinoJson/Polyfills/assert.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader, typename TStringStorage>
class JsonDeserializer {
<<<<<<< HEAD
 public:
  JsonDeserializer(MemoryPool &pool, TReader reader,
                   TStringStorage stringStorage)
      : _stringStorage(stringStorage),
        _foundSomething(false),
        _latch(reader),
        _pool(&pool),
        _error(DeserializationError::Ok) {}
=======
  typedef typename remove_reference<TStringStorage>::type::StringBuilder
      StringBuilder;

  struct StringOrError {
    DeserializationError err;
    const char *value;

    StringOrError(DeserializationError e) : err(e) {}
    StringOrError(DeserializationError::Code c) : err(c) {}
    StringOrError(const char *s) : err(DeserializationError::Ok), value(s) {}
  };

 public:
  JsonDeserializer(MemoryPool &pool, TReader reader,
                   TStringStorage stringStorage)
      : _pool(&pool), _stringStorage(stringStorage), _latch(reader) {}
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
<<<<<<< HEAD
    parseVariant(variant, filter, nestingLimit);

    if (!_error && _latch.last() != 0 && !variant.isEnclosed()) {
=======
    DeserializationError err = parseVariant(variant, filter, nestingLimit);

    if (!err && _latch.last() != 0 && !variant.isEnclosed()) {
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
      // We don't detect trailing characters earlier, so we need to check now
      return DeserializationError::InvalidInput;
    }

    return _error;
  }

 private:
  JsonDeserializer &operator=(const JsonDeserializer &);  // non-copiable

  char current() {
    return _latch.current();
  }

  void move() {
    _latch.clear();
  }

  bool eat(char charToSkip) {
    if (current() != charToSkip)
      return false;
    move();
    return true;
  }

  template <typename TFilter>
<<<<<<< HEAD
  bool parseVariant(VariantData &variant, TFilter filter,
                    NestingLimit nestingLimit) {
    if (!skipSpacesAndComments())
      return false;
=======
  DeserializationError parseVariant(VariantData &variant, TFilter filter,
                                    NestingLimit nestingLimit) {
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    switch (current()) {
      case '[':
        if (filter.allowArray())
          return parseArray(variant.toArray(), filter, nestingLimit);
        else
          return skipArray(nestingLimit);

      case '{':
        if (filter.allowObject())
          return parseObject(variant.toObject(), filter, nestingLimit);
        else
          return skipObject(nestingLimit);

      case '\"':
      case '\'':
        if (filter.allowValue())
          return parseStringValue(variant);
        else
          return skipString();

      default:
        if (filter.allowValue())
          return parseNumericValue(variant);
        else
          return skipNumericValue();
    }
  }

<<<<<<< HEAD
  bool skipVariant(NestingLimit nestingLimit) {
    if (!skipSpacesAndComments())
      return false;
=======
  DeserializationError skipVariant(NestingLimit nestingLimit) {
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    switch (current()) {
      case '[':
        return skipArray(nestingLimit);

      case '{':
        return skipObject(nestingLimit);

      case '\"':
      case '\'':
        return skipString();

      default:
        return skipNumericValue();
    }
  }

  template <typename TFilter>
<<<<<<< HEAD
  bool parseArray(CollectionData &array, TFilter filter,
                  NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
=======
  DeserializationError parseArray(CollectionData &array, TFilter filter,
                                  NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Skip spaces
<<<<<<< HEAD
    if (!skipSpacesAndComments())
      return false;

    // Empty array?
    if (eat(']'))
      return true;
=======
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

    // Empty array?
    if (eat(']'))
      return DeserializationError::Ok;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    TFilter memberFilter = filter[0UL];

    // Read each value
    for (;;) {
      if (memberFilter.allow()) {
        // Allocate slot in array
        VariantData *value = array.addElement(_pool);
<<<<<<< HEAD
        if (!value) {
          _error = DeserializationError::NoMemory;
          return false;
        }

        // 1 - Parse value
        if (!parseVariant(*value, memberFilter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }

      // 2 - Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // 3 - More values?
      if (eat(']'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }

  bool skipArray(NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Read each value
    for (;;) {
      // 1 - Skip value
      if (!skipVariant(nestingLimit.decrement()))
        return false;

      // 2 - Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // 3 - More values?
      if (eat(']'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }

  template <typename TFilter>
  bool parseObject(CollectionData &object, TFilter filter,
                   NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

=======
        if (!value)
          return DeserializationError::NoMemory;

        // 1 - Parse value
        err = parseVariant(*value, memberFilter, nestingLimit.decrement());
        if (err)
          return err;
      } else {
        err = skipVariant(nestingLimit.decrement());
        if (err)
          return err;
      }

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // 3 - More values?
      if (eat(']'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;
    }
  }

  DeserializationError skipArray(NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    // Skip opening braket
    ARDUINOJSON_ASSERT(current() == '[');
    move();

    // Read each value
    for (;;) {
      // 1 - Skip value
      DeserializationError err = skipVariant(nestingLimit.decrement());
      if (err)
        return err;

      // 2 - Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // 3 - More values?
      if (eat(']'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;
    }
  }

  template <typename TFilter>
  DeserializationError parseObject(CollectionData &object, TFilter filter,
                                   NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    // Skip opening brace
    ARDUINOJSON_ASSERT(current() == '{');
    move();

    // Skip spaces
<<<<<<< HEAD
    if (!skipSpacesAndComments())
      return false;

    // Empty object?
    if (eat('}'))
      return true;
=======
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

    // Empty object?
    if (eat('}'))
      return DeserializationError::Ok;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    // Read each key value pair
    for (;;) {
      // Parse key
<<<<<<< HEAD
      if (!parseKey())
        return false;

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // Colon
      if (!eat(':')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }

      const char *key = _stringStorage.c_str();

      TFilter memberFilter = filter[key];

      if (memberFilter.allow()) {
        VariantData *variant = object.getMember(adaptString(key));
        if (!variant) {
          // Save key in memory pool.
          // This MUST be done before adding the slot.
          key = _stringStorage.save();

          // Allocate slot in object
          VariantSlot *slot = object.addSlot(_pool);
          if (!slot) {
            _error = DeserializationError::NoMemory;
            return false;
          }

          slot->setKey(key, typename TStringStorage::storage_policy());

          variant = slot->data();
        }

        // Parse value
        if (!parseVariant(*variant, memberFilter, nestingLimit.decrement()))
          return false;
      } else {
        if (!skipVariant(nestingLimit.decrement()))
          return false;
      }

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // More keys/values?
      if (eat('}'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;
    }
  }

  bool skipObject(NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }
=======
      StringOrError key = parseKey();
      err = key.err;  // <- this trick saves 62 bytes on AVR
      if (err)
        return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;  // Colon
      if (!eat(':'))
        return DeserializationError::InvalidInput;

      TFilter memberFilter = filter[key.value];

      if (memberFilter.allow()) {
        VariantData *variant = object.getMember(adaptString(key.value));
        if (!variant) {
          // Allocate slot in object
          VariantSlot *slot = object.addSlot(_pool);
          if (!slot)
            return DeserializationError::NoMemory;

          slot->setOwnedKey(make_not_null(key.value));

          variant = slot->data();
        }

        // Parse value
        err = parseVariant(*variant, memberFilter, nestingLimit.decrement());
        if (err)
          return err;
      } else {
        _stringStorage.reclaim(key.value);
        err = skipVariant(nestingLimit.decrement());
        if (err)
          return err;
      }

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // More keys/values?
      if (eat('}'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;
    }
  }

  DeserializationError skipObject(NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    // Skip opening brace
    ARDUINOJSON_ASSERT(current() == '{');
    move();

    // Skip spaces
<<<<<<< HEAD
    if (!skipSpacesAndComments())
      return false;

    // Empty object?
    if (eat('}'))
      return true;
=======
    DeserializationError err = skipSpacesAndComments();
    if (err)
      return err;

    // Empty object?
    if (eat('}'))
      return DeserializationError::Ok;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    // Read each key value pair
    for (;;) {
      // Skip key
<<<<<<< HEAD
      if (!skipVariant(nestingLimit.decrement()))
        return false;

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // Colon
      if (!eat(':')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }

      // Skip value
      if (!skipVariant(nestingLimit.decrement()))
        return false;

      // Skip spaces
      if (!skipSpacesAndComments())
        return false;

      // More keys/values?
      if (eat('}'))
        return true;
      if (!eat(',')) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
    }
  }

  bool parseKey() {
    _stringStorage.startString();
=======
      err = skipVariant(nestingLimit.decrement());
      if (err)
        return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // Colon
      if (!eat(':'))
        return DeserializationError::InvalidInput;

      // Skip value
      err = skipVariant(nestingLimit.decrement());
      if (err)
        return err;

      // Skip spaces
      err = skipSpacesAndComments();
      if (err)
        return err;

      // More keys/values?
      if (eat('}'))
        return DeserializationError::Ok;
      if (!eat(','))
        return DeserializationError::InvalidInput;
    }
  }

  StringOrError parseKey() {
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    if (isQuote(current())) {
      return parseQuotedString();
    } else {
      return parseNonQuotedString();
    }
  }

<<<<<<< HEAD
  bool parseStringValue(VariantData &variant) {
    _stringStorage.startString();
    if (!parseQuotedString())
      return false;
    const char *value = _stringStorage.save();
    variant.setStringPointer(value, typename TStringStorage::storage_policy());
    return true;
  }

  bool parseQuotedString() {
=======
  DeserializationError parseStringValue(VariantData &variant) {
    StringOrError result = parseQuotedString();
    if (result.err)
      return result.err;
    variant.setOwnedString(make_not_null(result.value));
    return DeserializationError::Ok;
  }

  StringOrError parseQuotedString() {
    StringBuilder builder = _stringStorage.startString();
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
#if ARDUINOJSON_DECODE_UNICODE
    Utf16::Codepoint codepoint;
#endif
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;

<<<<<<< HEAD
      if (c == '\0') {
        _error = DeserializationError::IncompleteInput;
        return false;
      }

      if (c == '\\') {
        c = current();

        if (c == '\0') {
          _error = DeserializationError::IncompleteInput;
          return false;
        }

=======
      if (c == '\0')
        return DeserializationError::IncompleteInput;

      if (c == '\\') {
        c = current();
        if (c == '\0')
          return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
        if (c == 'u') {
#if ARDUINOJSON_DECODE_UNICODE
          move();
          uint16_t codeunit;
<<<<<<< HEAD
          if (!parseHex4(codeunit))
            return false;
          if (codepoint.append(codeunit))
            Utf8::encodeCodepoint(codepoint.value(), _stringStorage);
=======
          DeserializationError err = parseHex4(codeunit);
          if (err)
            return err;
          if (codepoint.append(codeunit))
            Utf8::encodeCodepoint(codepoint.value(), builder);
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
          continue;
#else
          _error = DeserializationError::NotSupported;
          return false;
#endif
        }

        // replace char
        c = EscapeSequence::unescapeChar(c);
<<<<<<< HEAD
        if (c == '\0') {
          _error = DeserializationError::InvalidInput;
          return false;
        }
=======
        if (c == '\0')
          return DeserializationError::InvalidInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
        move();
      }

      _stringStorage.append(c);
    }

<<<<<<< HEAD
    _stringStorage.append('\0');

    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }
=======
    const char *result = builder.complete();
    if (!result)
      return DeserializationError::NoMemory;
    return result;
  }

  StringOrError parseNonQuotedString() {
    StringBuilder builder = _stringStorage.startString();
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    return true;
  }

  bool parseNonQuotedString() {
    char c = current();
    ARDUINOJSON_ASSERT(c);

    if (canBeInNonQuotedString(c)) {  // no quotes
      do {
        move();
        _stringStorage.append(c);
        c = current();
      } while (canBeInNonQuotedString(c));
    } else {
      _error = DeserializationError::InvalidInput;
      return false;
    }

<<<<<<< HEAD
    _stringStorage.append('\0');

    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }

    return true;
=======
    const char *result = builder.complete();
    if (!result)
      return DeserializationError::NoMemory;
    return result;
  }

  DeserializationError skipString() {
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;
      if (c == '\0')
        return DeserializationError::IncompleteInput;
      if (c == '\\') {
        if (current() != '\0')
          move();
      }
    }

    return DeserializationError::Ok;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
  }

  bool skipString() {
    const char stopChar = current();

    move();
    for (;;) {
      char c = current();
      move();
      if (c == stopChar)
        break;
      if (c == '\0') {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      if (c == '\\') {
        if (current() != '\0')
          move();
      }
    }

    return true;
  }

  bool parseNumericValue(VariantData &result) {
    uint8_t n = 0;

    char c = current();
    while (canBeInNonQuotedString(c) && n < 63) {
      move();
      _buffer[n++] = c;
      c = current();
    }
    _buffer[n] = 0;

    c = _buffer[0];
    if (c == 't') {  // true
      result.setBoolean(true);
      if (n != 4) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (c == 'f') {  // false
      result.setBoolean(false);
      if (n != 5) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }
    if (c == 'n') {  // null
      // the variant is already null
      if (n != 4) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      return true;
    }

    if (!parseNumber(_buffer, result)) {
      _error = DeserializationError::InvalidInput;
      return false;
    }

    return true;
  }

  bool skipNumericValue() {
    char c = current();
    while (canBeInNonQuotedString(c)) {
      move();
      c = current();
    }
    return true;
  }

<<<<<<< HEAD
  bool parseHex4(uint16_t &result) {
    result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      char digit = current();
      if (!digit) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
      uint8_t value = decodeHex(digit);
      if (value > 0x0F) {
        _error = DeserializationError::InvalidInput;
        return false;
      }
=======
  DeserializationError skipNumericValue() {
    char c = current();
    while (canBeInNonQuotedString(c)) {
      move();
      c = current();
    }
    return DeserializationError::Ok;
  }

  DeserializationError parseHex4(uint16_t &result) {
    result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
      char digit = current();
      if (!digit)
        return DeserializationError::IncompleteInput;
      uint8_t value = decodeHex(digit);
      if (value > 0x0F)
        return DeserializationError::InvalidInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
      result = uint16_t((result << 4) | value);
      move();
    }
    return true;
  }

  static inline bool isBetween(char c, char min, char max) {
    return min <= c && c <= max;
  }

  static inline bool canBeInNonQuotedString(char c) {
    return isBetween(c, '0', '9') || isBetween(c, '_', 'z') ||
           isBetween(c, 'A', 'Z') || c == '+' || c == '-' || c == '.';
  }

  static inline bool isQuote(char c) {
    return c == '\'' || c == '\"';
  }

  static inline uint8_t decodeHex(char c) {
    if (c < 'A')
      return uint8_t(c - '0');
    c = char(c & ~0x20);  // uppercase
    return uint8_t(c - 'A' + 10);
  }

  bool skipSpacesAndComments() {
    for (;;) {
      switch (current()) {
        // end of string
        case '\0':
          _error = _foundSomething ? DeserializationError::IncompleteInput
                                   : DeserializationError::EmptyInput;
          return false;

        // spaces
        case ' ':
        case '\t':
        case '\r':
        case '\n':
          move();
          continue;

#if ARDUINOJSON_ENABLE_COMMENTS
        // comments
        case '/':
          move();  // skip '/'
          switch (current()) {
            // block comment
            case '*': {
              move();  // skip '*'
              bool wasStar = false;
              for (;;) {
                char c = current();
<<<<<<< HEAD
                if (c == '\0') {
                  _error = DeserializationError::IncompleteInput;
                  return false;
                }
=======
                if (c == '\0')
                  return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
                if (c == '/' && wasStar) {
                  move();
                  break;
                }
                wasStar = c == '*';
                move();
              }
              break;
            }

            // trailing comment
            case '/':
              // no need to skip "//"
              for (;;) {
                move();
                char c = current();
<<<<<<< HEAD
                if (c == '\0') {
                  _error = DeserializationError::IncompleteInput;
                  return false;
                }
=======
                if (c == '\0')
                  return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
                if (c == '\n')
                  break;
              }
              break;

            // not a comment, just a '/'
            default:
              _error = DeserializationError::InvalidInput;
              return false;
          }
          break;
#endif

        default:
          _foundSomething = true;
          return true;
      }
    }
  }

<<<<<<< HEAD
  TStringStorage _stringStorage;
  bool _foundSomething;
  Latch<TReader> _latch;
  MemoryPool *_pool;
  char _buffer[64];  // using a member instead of a local variable because it
                     // ended in the recursive path after compiler inlined the
                     // code
  DeserializationError _error;
};

//
// deserializeJson(JsonDocument&, const std::string&, ...)
//
// ... = NestingLimit
template <typename TString>
=======
  MemoryPool *_pool;
  TStringStorage _stringStorage;
  Latch<TReader> _latch;
};

// deserializeJson(JsonDocument&, const std::string&, ...)
template <typename TInput>
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
DeserializationError deserializeJson(
    JsonDocument &doc, const TString &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
<<<<<<< HEAD
}
// ... = Filter, NestingLimit
template <typename TString>
DeserializationError deserializeJson(
    JsonDocument &doc, const TString &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TString>
DeserializationError deserializeJson(JsonDocument &doc, const TString &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

//
// deserializeJson(JsonDocument&, std::istream&, ...)
//
// ... = NestingLimit
template <typename TStream>
DeserializationError deserializeJson(
    JsonDocument &doc, TStream &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TStream>
DeserializationError deserializeJson(
    JsonDocument &doc, TStream &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TStream>
DeserializationError deserializeJson(JsonDocument &doc, TStream &input,
=======
}
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, const TInput &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TInput>
DeserializationError deserializeJson(JsonDocument &doc, const TInput &input,
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

<<<<<<< HEAD
//
// deserializeJson(JsonDocument&, char*, ...)
//
// ... = NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

//
// deserializeJson(JsonDocument&, char*, size_t, ...)
//
// ... = NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}
// ... = NestingLimit, Filter
=======
// deserializeJson(JsonDocument&, const std::istream&, ...)
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, TInput &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TInput>
DeserializationError deserializeJson(
    JsonDocument &doc, TInput &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TInput>
DeserializationError deserializeJson(JsonDocument &doc, TInput &input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

// deserializeJson(JsonDocument&, char*, ...)
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit,
                                       AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, nestingLimit, filter);
}

// deserializeJson(JsonDocument&, char*, size_t, ...)
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       AllowAllFilter());
}
template <typename TChar>
DeserializationError deserializeJson(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
template <typename TChar>
DeserializationError deserializeJson(JsonDocument &doc, TChar *input,
                                     size_t inputSize,
                                     NestingLimit nestingLimit, Filter filter) {
  return deserialize<JsonDeserializer>(doc, input, inputSize, nestingLimit,
                                       filter);
}

}  // namespace ARDUINOJSON_NAMESPACE
