// ArduinoJson - arduinojson.org
// Copyright Benoit Blanchon 2014-2020
// MIT License

#pragma once

#include <ArduinoJson/Deserialization/deserialize.hpp>
#include <ArduinoJson/Memory/MemoryPool.hpp>
#include <ArduinoJson/MsgPack/endianess.hpp>
#include <ArduinoJson/MsgPack/ieee754.hpp>
#include <ArduinoJson/Polyfills/type_traits.hpp>
#include <ArduinoJson/Variant/VariantData.hpp>

namespace ARDUINOJSON_NAMESPACE {

template <typename TReader, typename TStringStorage>
class MsgPackDeserializer {
 public:
  MsgPackDeserializer(MemoryPool &pool, TReader reader,
                      TStringStorage stringStorage)
<<<<<<< HEAD
      : _pool(&pool),
        _reader(reader),
        _stringStorage(stringStorage),
        _error(DeserializationError::Ok),
        _foundSomething(false) {}

  template <typename TFilter>
  DeserializationError parse(VariantData &variant, TFilter filter,
                             NestingLimit nestingLimit) {
    parseVariant(variant, filter, nestingLimit);
    return _foundSomething ? _error : DeserializationError::EmptyInput;
  }

 private:
  // Prevent VS warning "assignment operator could not be generated"
  MsgPackDeserializer &operator=(const MsgPackDeserializer &);
=======
      : _pool(&pool), _reader(reader), _stringStorage(stringStorage) {}

  // TODO: add support for filter
  DeserializationError parse(VariantData &variant, AllowAllFilter,
                             NestingLimit nestingLimit) {
    return parse(variant, nestingLimit);
  }

  DeserializationError parse(VariantData &variant, NestingLimit nestingLimit) {
    uint8_t code;
    if (!readByte(code))
      return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

  bool invalidInput() {
    _error = DeserializationError::InvalidInput;
    return false;
  }

  bool notSupported() {
    _error = DeserializationError::NotSupported;
    return false;
  }

  template <typename TFilter>
  bool parseVariant(VariantData &variant, TFilter filter,
                    NestingLimit nestingLimit) {
    uint8_t code = 0;  // TODO: why do we need to initialize this variable?
    if (!readByte(code))
      return false;

<<<<<<< HEAD
    _foundSomething = true;

    bool allowValue = filter.allowValue();
=======
    if ((code & 0xf0) == 0x90) {
      return readArray(variant.toArray(), code & 0x0F, nestingLimit);
    }

    if ((code & 0xf0) == 0x80) {
      return readObject(variant.toObject(), code & 0x0F, nestingLimit);
    }
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    switch (code) {
      case 0xc0:
        // already null
        return true;

      case 0xc1:
        return invalidInput();

      case 0xc2:
        if (allowValue)
          variant.setBoolean(false);
        return true;

      case 0xc3:
        if (allowValue)
          variant.setBoolean(true);
        return true;

      case 0xc4:  // bin 8
        if (allowValue)
          return notSupported();
        else
          return skipString<uint8_t>();

      case 0xc5:  // bin 16
        if (allowValue)
          return notSupported();
        else
          return skipString<uint16_t>();

      case 0xc6:  // bin 32
        if (allowValue)
          return notSupported();
        else
          return skipString<uint32_t>();

      case 0xc7:  // ext 8
        if (allowValue)
          return notSupported();
        else
          return skipExt<uint8_t>();

      case 0xc8:  // ext 16
        if (allowValue)
          return notSupported();
        else
          return skipExt<uint16_t>();

      case 0xc9:  // ext 32
        if (allowValue)
          return notSupported();
        else
          return skipExt<uint32_t>();

      case 0xca:
        if (allowValue)
          return readFloat<float>(variant);
        else
          return skipBytes(4);

      case 0xcb:
        if (allowValue)
          return readDouble<double>(variant);
        else
          return skipBytes(8);

      case 0xcc:
        if (allowValue)
          return readInteger<uint8_t>(variant);
        else
          return skipBytes(1);

      case 0xcd:
        if (allowValue)
          return readInteger<uint16_t>(variant);
        else
          return skipBytes(2);

      case 0xce:
        if (allowValue)
          return readInteger<uint32_t>(variant);
        else
          return skipBytes(4);

      case 0xcf:
        if (allowValue)
#if ARDUINOJSON_USE_LONG_LONG
          return readInteger<uint64_t>(variant);
#else
          return notSupported();
#endif
        else
          return skipBytes(8);

      case 0xd0:
        if (allowValue)
          return readInteger<int8_t>(variant);
        else
          return skipBytes(1);

      case 0xd1:
        if (allowValue)
          return readInteger<int16_t>(variant);
        else
          return skipBytes(2);

      case 0xd2:
        if (allowValue)
          return readInteger<int32_t>(variant);
        else
          return skipBytes(4);

      case 0xd3:
        if (allowValue)
#if ARDUINOJSON_USE_LONG_LONG
          return readInteger<int64_t>(variant);
#else
          return notSupported();
#endif
        else
          return skipBytes(8);

      case 0xd4:  // fixext 1
        if (allowValue)
          return notSupported();
        else
          return skipBytes(2);

      case 0xd5:  // fixext 2
        if (allowValue)
          return notSupported();
        else
          return skipBytes(3);

      case 0xd6:  // fixext 4
        if (allowValue)
          return notSupported();
        else
          return skipBytes(5);

      case 0xd7:  // fixext 8
        if (allowValue)
          return notSupported();
        else
          return skipBytes(9);

      case 0xd8:  // fixext 16
        if (allowValue)
          return notSupported();
        else
          return skipBytes(17);

      case 0xd9:
        if (allowValue)
          return readString<uint8_t>(variant);
        else
          return skipString<uint8_t>();

      case 0xda:
        if (allowValue)
          return readString<uint16_t>(variant);
        else
          return skipString<uint16_t>();

      case 0xdb:
        if (allowValue)
          return readString<uint32_t>(variant);
        else
          return skipString<uint32_t>();

      case 0xdc:
<<<<<<< HEAD
        return readArray<uint16_t>(variant, filter, nestingLimit);

      case 0xdd:
        return readArray<uint32_t>(variant, filter, nestingLimit);

      case 0xde:
        return readObject<uint16_t>(variant, filter, nestingLimit);

      case 0xdf:
        return readObject<uint32_t>(variant, filter, nestingLimit);
    }
=======
        return readArray<uint16_t>(variant.toArray(), nestingLimit);

      case 0xdd:
        return readArray<uint32_t>(variant.toArray(), nestingLimit);

      case 0xde:
        return readObject<uint16_t>(variant.toObject(), nestingLimit);

      case 0xdf:
        return readObject<uint32_t>(variant.toObject(), nestingLimit);
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    switch (code & 0xf0) {
      case 0x80:
        return readObject(variant, code & 0x0F, filter, nestingLimit);

      case 0x90:
        return readArray(variant, code & 0x0F, filter, nestingLimit);
    }

    if ((code & 0xe0) == 0xa0) {
      if (allowValue)
        return readString(variant, code & 0x1f);
      else
        return skipBytes(code & 0x1f);
    }

    if (allowValue)
      variant.setInteger(static_cast<int8_t>(code));

    return true;
  }

  bool readByte(uint8_t &value) {
    int c = _reader.read();
<<<<<<< HEAD
    if (c < 0) {
      _error = DeserializationError::IncompleteInput;
      return false;
    }
=======
    if (c < 0)
      return false;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    value = static_cast<uint8_t>(c);
    return true;
  }

  bool readBytes(uint8_t *p, size_t n) {
    if (_reader.readBytes(reinterpret_cast<char *>(p), n) == n)
      return true;
    _error = DeserializationError::IncompleteInput;
    return false;
  }

  template <typename T>
  bool readBytes(T &value) {
    return readBytes(reinterpret_cast<uint8_t *>(&value), sizeof(value));
  }

  bool skipBytes(size_t n) {
    for (; n; --n) {
      if (_reader.read() < 0) {
        _error = DeserializationError::IncompleteInput;
        return false;
      }
    }
    return true;
  }

  template <typename T>
  bool readInteger(T &value) {
    if (!readBytes(value))
      return false;
    fixEndianess(value);
    return true;
  }

  template <typename T>
  bool readInteger(VariantData &variant) {
    T value;
    if (!readInteger(value))
<<<<<<< HEAD
      return false;
=======
      return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    variant.setInteger(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readFloat(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
<<<<<<< HEAD
      return false;
=======
      return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 8, bool>::type readDouble(
      VariantData &variant) {
    T value;
    if (!readBytes(value))
<<<<<<< HEAD
      return false;
=======
      return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  typename enable_if<sizeof(T) == 4, bool>::type readDouble(
      VariantData &variant) {
    uint8_t i[8];  // input is 8 bytes
    T value;       // output is 4 bytes
    uint8_t *o = reinterpret_cast<uint8_t *>(&value);
    if (!readBytes(i, 8))
<<<<<<< HEAD
      return false;
=======
      return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    doubleToFloat(i, o);
    fixEndianess(value);
    variant.setFloat(value);
    return true;
  }

  template <typename T>
  bool readString(VariantData &variant) {
    T size;
    if (!readInteger(size))
<<<<<<< HEAD
      return false;
=======
      return DeserializationError::IncompleteInput;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
    return readString(variant, size);
  }

  template <typename T>
  bool readString() {
    T size;
    if (!readInteger(size))
<<<<<<< HEAD
      return false;
    return readString(size);
  }

  template <typename T>
  bool skipString() {
    T size;
    if (!readInteger(size))
      return false;
    return skipBytes(size);
  }

  bool readString(VariantData &variant, size_t n) {
    if (!readString(n))
      return false;
    variant.setStringPointer(_stringStorage.save(),
                             typename TStringStorage::storage_policy());
    return true;
=======
      return DeserializationError::IncompleteInput;
    return readString(str, size);
  }

  DeserializationError readString(VariantData &variant, size_t n) {
    const char *s = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
    DeserializationError err = readString(s, n);
    if (!err)
      variant.setOwnedString(make_not_null(s));
    return err;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
  }

  bool readString(size_t n) {
    _stringStorage.startString();
    for (; n; --n) {
      uint8_t c;
      if (!readBytes(c))
<<<<<<< HEAD
        return false;
      _stringStorage.append(static_cast<char>(c));
    }
    _stringStorage.append('\0');
    if (!_stringStorage.isValid()) {
      _error = DeserializationError::NoMemory;
      return false;
    }

    return true;
  }

  template <typename TSize, typename TFilter>
  bool readArray(VariantData &variant, TFilter filter,
                 NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readArray(variant, size, filter, nestingLimit);
  }

  template <typename TFilter>
  bool readArray(VariantData &variant, size_t n, TFilter filter,
                 NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    bool allowArray = filter.allowArray();

    CollectionData *array = allowArray ? &variant.toArray() : 0;

    TFilter memberFilter = filter[0U];

    for (; n; --n) {
      VariantData *value;

      if (memberFilter.allow()) {
        value = array->addElement(_pool);
        if (!value) {
          _error = DeserializationError::NoMemory;
          return false;
        }
      } else {
        value = 0;
      }

      if (!parseVariant(*value, memberFilter, nestingLimit.decrement()))
        return false;
    }

    return true;
  }

  template <typename TSize, typename TFilter>
  bool readObject(VariantData &variant, TFilter filter,
                  NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return false;
    return readObject(variant, size, filter, nestingLimit);
  }

  template <typename TFilter>
  bool readObject(VariantData &variant, size_t n, TFilter filter,
                  NestingLimit nestingLimit) {
    if (nestingLimit.reached()) {
      _error = DeserializationError::TooDeep;
      return false;
    }

    CollectionData *object = filter.allowObject() ? &variant.toObject() : 0;

    for (; n; --n) {
      if (!readKey())
        return false;

      const char *key = _stringStorage.c_str();
      TFilter memberFilter = filter[key];
      VariantData *member;

      if (memberFilter.allow()) {
        // Save key in memory pool.
        // This MUST be done before adding the slot.
        key = _stringStorage.save();

        VariantSlot *slot = object->addSlot(_pool);
        if (!slot) {
          _error = DeserializationError::NoMemory;
          return false;
        }

        slot->setKey(key, typename TStringStorage::storage_policy());

        member = slot->data();
      } else {
        member = 0;
      }

      if (!parseVariant(*member, memberFilter, nestingLimit.decrement()))
        return false;
    }

    return true;
=======
        return DeserializationError::IncompleteInput;
      builder.append(static_cast<char>(c));
    }
    result = builder.complete();
    if (!result)
      return DeserializationError::NoMemory;
    return DeserializationError::Ok;
  }

  template <typename TSize>
  DeserializationError readArray(CollectionData &array,
                                 NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return DeserializationError::IncompleteInput;
    return readArray(array, size, nestingLimit);
  }

  DeserializationError readArray(CollectionData &array, size_t n,
                                 NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    for (; n; --n) {
      VariantData *value = array.addElement(_pool);
      if (!value)
        return DeserializationError::NoMemory;

      DeserializationError err = parse(*value, nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
  }

  template <typename TSize>
  DeserializationError readObject(CollectionData &object,
                                  NestingLimit nestingLimit) {
    TSize size;
    if (!readInteger(size))
      return DeserializationError::IncompleteInput;
    return readObject(object, size, nestingLimit);
  }

  DeserializationError readObject(CollectionData &object, size_t n,
                                  NestingLimit nestingLimit) {
    if (nestingLimit.reached())
      return DeserializationError::TooDeep;

    for (; n; --n) {
      VariantSlot *slot = object.addSlot(_pool);
      if (!slot)
        return DeserializationError::NoMemory;

      const char *key = 0;  // <- mute "maybe-uninitialized" (+4 bytes on AVR)
      DeserializationError err = parseKey(key);
      if (err)
        return err;
      slot->setOwnedKey(make_not_null(key));

      err = parse(*slot->data(), nestingLimit.decrement());
      if (err)
        return err;
    }

    return DeserializationError::Ok;
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
  }

  bool readKey() {
    uint8_t code;
    if (!readByte(code))
<<<<<<< HEAD
      return false;

    if ((code & 0xe0) == 0xa0)
      return readString(code & 0x1f);
=======
      return DeserializationError::IncompleteInput;

    if ((code & 0xe0) == 0xa0)
      return readString(key, code & 0x1f);
>>>>>>> 45b52aec473bd7023203015b24e667856f836575

    switch (code) {
      case 0xd9:
        return readString<uint8_t>();

      case 0xda:
        return readString<uint16_t>();

      case 0xdb:
        return readString<uint32_t>();

      default:
        return notSupported();
    }
  }

  template <typename T>
  bool skipExt() {
    T size;
    if (!readInteger(size))
      return false;
    return skipBytes(size + 1);
  }

  MemoryPool *_pool;
  TReader _reader;
  TStringStorage _stringStorage;
<<<<<<< HEAD
  DeserializationError _error;
  bool _foundSomething;
=======
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
};

//
// deserializeMsgPack(JsonDocument&, const std::string&, ...)
//
// ... = NestingLimit
template <typename TString>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, const TString &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
<<<<<<< HEAD
}
// ... = Filter, NestingLimit
template <typename TString>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, const TString &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TString>
DeserializationError deserializeMsgPack(JsonDocument &doc, const TString &input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
=======
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
}

//
// deserializeMsgPack(JsonDocument&, std::istream&, ...)
//
// ... = NestingLimit
template <typename TStream>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TStream &input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
<<<<<<< HEAD
}
// ... = Filter, NestingLimit
template <typename TStream>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TStream &input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TStream>
DeserializationError deserializeMsgPack(JsonDocument &doc, TStream &input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
=======
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
}

//
// deserializeMsgPack(JsonDocument&, char*, ...)
//
// ... = NestingLimit
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
                                          AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
<<<<<<< HEAD
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
}
// ... = NestingLimit, Filter
template <typename TChar>
DeserializationError deserializeMsgPack(JsonDocument &doc, TChar *input,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit, filter);
=======
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          AllowAllFilter());
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
}

//
// deserializeMsgPack(JsonDocument&, char*, size_t, ...)
//
// ... = NestingLimit
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input, size_t inputSize,
    NestingLimit nestingLimit = NestingLimit()) {
<<<<<<< HEAD
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
=======
  return deserialize<MsgPackDeserializer>(doc, input, nestingLimit,
>>>>>>> 45b52aec473bd7023203015b24e667856f836575
                                          AllowAllFilter());
}
// ... = Filter, NestingLimit
template <typename TChar>
DeserializationError deserializeMsgPack(
    JsonDocument &doc, TChar *input, size_t inputSize, Filter filter,
    NestingLimit nestingLimit = NestingLimit()) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          filter);
}
// ... = NestingLimit, Filter
template <typename TChar>
DeserializationError deserializeMsgPack(JsonDocument &doc, TChar *input,
                                        size_t inputSize,
                                        NestingLimit nestingLimit,
                                        Filter filter) {
  return deserialize<MsgPackDeserializer>(doc, input, inputSize, nestingLimit,
                                          filter);
}

}  // namespace ARDUINOJSON_NAMESPACE
