#ifndef LMDB_H
#define LMDB_H

#include <string>

namespace rnlmdb {

    void open(std::string dbName, long mapSize);
    void put(std::string key, std::string value);
    void putBatch(std::unordered_map<std::string, std::string> batch);
    std::string get(std::string key);
}

#endif /* LMDB_H */
