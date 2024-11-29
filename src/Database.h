#pragma once

#include <map>

/**
 * @brief A database that manages a given type of data and stores it on the heap.
 * 
 * @tparam T The type of the data to be stored.
 */
template<class T>
class Database
{
public:
  Database() = default;
  Database(const Database& db) = delete;
  Database& operator=(Database) = delete;

  ~Database()
  {
    clear();
  }

  /**
   * @brief Checks if an item is present in the database.
   * 
   * @param id Unique identifier of the item.
   * @return true 
   * @return false 
   */
  inline bool has(const unsigned int id) const
  {
    return objects.count(id) > 0;
  }

  /**
   * @brief Retrieves an item from the database.
   * 
   * @param id Unique identifier of the item.
   * @return T& A reference to the item.
   */
  inline T& get(const unsigned int id) const
  {
    return *objects.at(id);
  }

  /**
   * @brief Constructs and adds a new item to the database.
   * 
   * @return unsigned int Unique identifier of the item.
   */
  inline unsigned int add()
  {
    unsigned int id = nextID++;
    T* object = new T();
    object->id = id;
    objects[id] = object;
    return id;
  }

  /**
   * @brief Attempts to removes an item from the database. Returns success.
   * 
   * @param id Unique identifier of the item.
   * @return true Item sucessfully removed.
   * @return false Item still present in databse.
   */
  inline bool remove(const unsigned int id)
  {
    T& object = get(id);
    objects.erase(id);
    delete &object;
    return !(has(id));
  }

  /**
   * @brief Returns the number of items in the database.
   * 
   * @return size_t 
   */
  inline size_t size() const
  {
    return objects.size();
  }

  /**
   * @brief Removes all items from the database leaving it empty.
   * 
   */
  inline void clear()
  {
    for (const auto& keyValue : objects)
    {
      remove(keyValue.first);
    }
  }

private:
  /** The next available unique identifier to be given out. Does not recycle old
   * idetifiers. */
  unsigned int nextID = 0;

  /** Map of items. */
  std::map<unsigned int, T*> objects;
};