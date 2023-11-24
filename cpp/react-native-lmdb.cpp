#include <string>

#include "react-native-lmdb.h"
#include "lmdb++.h"

namespace rnlmdb {
    lmdb::env env = nullptr;

    void open(std::string dbPath, double mapSize) {
        if (env != nullptr) {
            throw std::runtime_error("environment already open");
        }
        env = lmdb::env::create();
        env.set_mapsize((long)mapSize);
        env.open(dbPath.c_str(), MDB_CREATE, 0664);
    }

    std::optional<std::string> get(std::string key)
    {
        auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
        auto dbi = lmdb::dbi::open(rtxn, nullptr);

        std::string_view value;
        if (dbi.get(rtxn, key, value)) {
            return std::string{value};
        }
        return std::nullopt;
    }

    void put(std::string key, std::string value)
    {
        auto wtxn = lmdb::txn::begin(env);
        auto dbi = lmdb::dbi::open(wtxn, nullptr);
        dbi.put(wtxn, key, value);
        wtxn.commit();
    }

    void del(std::string key)
    {
        auto wtxn = lmdb::txn::begin(env);
        auto dbi = lmdb::dbi::open(wtxn, nullptr);
        dbi.del(wtxn, key);
        wtxn.commit();
    }
}
