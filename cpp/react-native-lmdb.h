#ifndef LMDB_H
#define LMDB_H

#include <string>

namespace rnlmdb {
    void open(std::string dbName);
    void put(std::string key, std::string value);
    std::string get(std::string key);
}

#endif /* LMDB_H */
