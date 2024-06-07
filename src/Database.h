#pragma once

#include <map>

template<class T>
class Database
{
public:
  Database()
  {
    nextID = 0;
  }

  inline bool has(unsigned int id)
  {
    return objects.count(id) > 0;
  }

  inline T& get(unsigned int id)
  {
    return *objects[id];
  }

  inline unsigned int add()
  {
    unsigned int id = nextID++;
    T* object = new T();
    object->id = id;
    objects[id] = object;
    return id;
  }

  inline bool remove(unsigned int id)
  {
    T& object = get(id);
    objects.erase(id);
    delete &object;
    return !(has(id));
  }

  inline size_t size()
  {
    return objects.size();
  }

  inline void clear()
  {
    for (const auto& keyValue : objects)
    {
      remove(keyValue.first);
    }
  }

private:
  unsigned int nextID;
  std::map<unsigned int, T*> objects;
};