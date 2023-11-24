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

    std::string get(std::string key)
    {
        auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
        auto dbi = lmdb::dbi::open(rtxn, nullptr);
        auto keyValue = lmdb::val(key);
        auto dataValue = lmdb::val();
        auto rc = ::mdb_get(rtxn, dbi, keyValue, dataValue);
        rtxn.abort();
        if (rc != 0) {
            return nullptr;
        } else {
            return dataValue.data();
        }
    }

    void put(std::string key, std::string value)
    {
        auto wtxn = lmdb::txn::begin(env);
        auto dbi = lmdb::dbi::open(wtxn, nullptr);
        dbi.put(wtxn, key.c_str(), value.c_str());
        wtxn.commit();
    }

    void del(std::string key)
    {
        auto wtxn = lmdb::txn::begin(env);
        auto dbi = lmdb::dbi::open(wtxn, nullptr);
        dbi.del(wtxn, lmdb::val(key));
        wtxn.commit();
    }
}
