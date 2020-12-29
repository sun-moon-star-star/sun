/**
 * @file json.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_JSON_H_
#define SUN_UTIL_JSON_H_

#include "json/json.h"

#include <iostream>
#include <string>

namespace sun::util {

int updateJsonField(const std::string& in, const std::string& out,
                    const std::vector<std::string>& fields,
                    std::string* result) {
  Json::Value in_root, out_root;

  Json::CharReaderBuilder read_builder;
  JSONCPP_STRING err;
  const std::unique_ptr<Json::CharReader> reader(read_builder.newCharReader());
  if (!reader->parse(in.c_str(), in.c_str() + in.length(), &in_root, &err)) {
    return -1;
  }
  if (!reader->parse(out.c_str(), out.c_str() + out.length(), &out_root,
                     &err)) {
    return -1;
  }

  for (auto const& field : fields) {
    out_root[field] = in_root[field];
  }

  Json::StreamWriterBuilder write_builder;
  *result = Json::writeString(write_builder, out_root);
  return 0;
}

}  // namespace sun::util

#endif  // SUN_UTIL_JSON_H_