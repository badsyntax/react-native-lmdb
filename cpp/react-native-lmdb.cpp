#include "react-native-lmdb.h"
#include "lmdb++.h"

namespace rnlmdb {

    lmdb::env env = lmdb::env::create();

    void open(std::string dbName, long mapsize) {
        env.set_mapsize(mapsize);
        env.open(dbName.c_str(), MDB_CREATE, 0664);
    }

    std::optional<std::string> get(std::string key)
    {
        auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
        auto dbi = lmdb::dbi::open(rtxn, nullptr);
        auto keyValue = lmdb::val(key);
        auto dataValue = lmdb::val();
        auto rc = ::mdb_get(rtxn, dbi, keyValue, dataValue);
        rtxn.abort();
        if (rc != 0) {
            return std::nullopt;
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

    void putBatch(std::unordered_map<std::string, std::string> batch)
    {
        auto wtxn = lmdb::txn::begin(env);
        auto dbi = lmdb::dbi::open(wtxn, nullptr);
        for (auto const& [key, value] : batch)
        {
            dbi.put(wtxn, key.c_str(), value.c_str());
        }
        wtxn.commit();
    }
}
