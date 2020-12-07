/**
 * @file common.h
 * @author sun-moon-star-star
 */

#ifndef SUN_UTIL_COMMON_H_
#define SUN_UTIL_COMMON_H_

#define likely(x)  __builtin_expect(!!(x), 1)
#define unlikely(x)  __builtin_expect(!!(x), 0)
#define IF_TRUE_RETURN(condition, statement) do { if ((condition)) return (statement);} while(0)
#define IF_FALSE_RETURN(condition, statement) do { if (!(condition)) return (statement);} while(0)

#endif // SUN_UTIL_COMMON_H_