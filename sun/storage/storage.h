/**
 * @file storage.h
 * @author sun-moon-star-star
 */

#ifndef SUN_STORAGE_STORAGE_H_
#define SUN_STORAGE_STORAGE_H_

namespace sun {

struct storage {
  virtual void create(void*) = 0;
  virtual void update(void*) = 0;
  virtual void insert(void*) = 0;
  virtual void search(void*) = 0;
  virtual void delete_(void*) = 0;
};

}  // namespace sun

#endif  // SUN_STORAGE_STORAGE_H_