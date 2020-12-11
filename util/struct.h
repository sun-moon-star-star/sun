/**
 * @file struct.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_STRUCT_H_
#define SUN_UTIL_STRUCT_H_

namespace sun {

namespace util {

template <typename StructType, typename FieldType, FieldType StructType::*sign>
void update_struct_member_field(StructType* const obj, StructType const* const value) {
    obj->*sign = value->*sign;
}

} // namespace util

} // namespace sun

#endif // SUN_UTIL_STRUCT_H_