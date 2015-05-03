// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#pragma once

#include "../JsonBuffer.hpp"
#include "../JsonVariant.hpp"

namespace ArduinoJson {
namespace Internals {

// Parse JSON string to create JsonArrays and JsonObjects
// This internal class is not indended to be used directly.
// Instead, use JsonBuffer.parseArray() or .parseObject()
class JsonParser {
 public:
  JsonParser(JsonBuffer *buffer, char *json, uint8_t nestingLimit)
      : _buffer(buffer), _ptr(json), _nestingLimit(nestingLimit) {}

  JsonArray &parseArray();
  JsonObject &parseObject();

 private:
  bool skip(char charToSkip);
  bool skip(const char *wordToSkip);
  void skipSpaces();

  bool parseAnythingTo(JsonVariant *destination);
  bool doParseAnythingTo(JsonVariant *destination);

  const char *parseString();

  inline bool parseArrayTo(JsonVariant *destination);
  inline bool parseBooleanTo(JsonVariant *destination);
  inline bool parseNullTo(JsonVariant *destination);
  inline bool parseNumberTo(JsonVariant *destination);
  inline bool parseObjectTo(JsonVariant *destination);
  inline bool parseStringTo(JsonVariant *destination);

  JsonBuffer *_buffer;
  char *_ptr;
  uint8_t _nestingLimit;
};
}
}
