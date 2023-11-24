#ifndef LMDB_H
#define LMDB_H

#include <string>
#include <unordered_map>
#include <optional>

namespace rnlmdb {

    void open(std::string dbPath, double mapSize);
    void put(std::string key, std::string value);
    std::optional<std::string> get(std::string key);
    void del(std::string key);
}

#endif /* LMDB_H */
