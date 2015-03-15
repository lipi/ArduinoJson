// Copyright Benoit Blanchon 2014-2015
// MIT License
//
// Arduino JSON library
// https://github.com/bblanchon/ArduinoJson

#include "../include/ArduinoJson/JsonObject.hpp"

#include <string.h>  // for strcmp

#include "../include/ArduinoJson/Internals/StringBuilder.hpp"
#include "../include/ArduinoJson/JsonArray.hpp"
#include "../include/ArduinoJson/JsonBuffer.hpp"

using namespace ArduinoJson;
using namespace ArduinoJson::Internals;

JsonObject JsonObject::_invalid(NULL);

JsonVariant &JsonObject::createVariantAt(const char *key) {
  node_type *node = getNodeAt(key);
  if (node) goto KEY_ALREADY_EXISTS;

  node = createNode();
  if (!node) goto CREATE_FAILED;

  node->content.key = key;
  addNode(node);

KEY_ALREADY_EXISTS:
  return node->content.value;

CREATE_FAILED:
  return JsonVariant::invalid();
}

JsonVariant &JsonObject::getVariantAt(const char *key) const {
  node_type *node = getNodeAt(key);
  return node ? node->content.value : JsonVariant::invalid();
}

template <typename T>
T &JsonObject::createNested(const char *key) {
  if (!_buffer) return T::invalid();
  T &array = _buffer->create<T>();
  add(key, array);
  return array;
}

template JsonArray &JsonObject::createNested<JsonArray>(const char *key);
template JsonObject &JsonObject::createNested<JsonObject>(const char *key);

JsonObject::node_type *JsonObject::getNodeAt(const char *key) const {
  for (node_type *node = _firstNode; node; node = node->next) {
    if (!strcmp(node->content.key, key)) return node;
  }
  return NULL;
}

void JsonObject::writeTo(JsonWriter &writer) const {
  writer.beginObject();

  const node_type *node = _firstNode;
  while (node) {
    writer.writeString(node->content.key);
    writer.writeColon();
    node->content.value.writeTo(writer);

    node = node->next;
    if (!node) break;

    writer.writeComma();
  }

  writer.endObject();
}
