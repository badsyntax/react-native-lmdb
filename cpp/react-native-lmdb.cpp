#include <string>
#include <sstream>
#include <sys/stat.h>
#include <jsi/jsi.h>
#include <dirent.h>

#include "react-native-lmdb.h"

using namespace facebook;

namespace rnlmdb {

    MDB_env* env = nullptr;
    std::string docPathStr;
    std::vector<MDB_dbi> dbs;
    std::vector<MDB_txn*> txns;
//    MDB_dbi db;
    MDB_txn* rtxn = nullptr;

    bool folder_exists(const std::string &folderName)
    {
      struct stat buffer;
      return (stat(folderName.c_str(), &buffer) == 0);
    }

    int _mkdir(const char *path)
    {
        #if _POSIX_C_SOURCE
            return mkdir(path);
        #else
            return mkdir(path, 0755);
        #endif
    }

    int mkdirs(const char *path)
    {
        std::string current_level = "/";
        std::string level;
        std::stringstream ss(path);
        // First line is empty because it starts with /User
        getline(ss, level, '/');
        while (getline(ss, level, '/'))
        {
            current_level += level;
            if (!folder_exists(current_level) && _mkdir(current_level.c_str()) != 0)
            {
                return -1;
            }
            current_level += "/";
        }

        return 0;
    }

    bool cleanDirectory(const std::string &path)
    {
        struct dirent *ent;
        auto *dir = opendir(path.c_str());
        if (dir != NULL)
        {
            /* remove all the files and directories within directory */
            while ((ent = readdir(dir)) != NULL)
            {
                std::remove((path + ent->d_name).c_str());
            }
            closedir (dir);
        }
        else
        {
            /* could not open directory */
            return false;
        }
        return true;
    }

    void install(jsi::Runtime &jsiRuntime, const char *docPath)
    {
        docPathStr = std::string(docPath);
        
        auto init = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "init"),
            2, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isString() || !args[1].isNumber())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }
                
                auto dbName = args[0].asString(runtime).utf8(runtime);
                auto mapSize = args[1].asNumber();
                
                auto fullDocPathStr = docPathStr + "/" + dbName + "/";
                mkdirs(fullDocPathStr.c_str());
                
                int rc = mdb_env_create(&env);
                rc = mdb_env_set_mapsize(env, (long)mapSize);
                rc = mdb_env_set_maxdbs(env, 50);
                rc = mdb_env_open(env, fullDocPathStr.c_str(), MDB_WRITEMAP, 0644);

                return jsi::Value(nullptr);
            }
        );

        auto open = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "open"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isString())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto dbName = args[0].asString(runtime).utf8(runtime);
                
                MDB_dbi dbi;
                mdb_txn_begin(env, nullptr, 0, &rtxn);
                mdb_dbi_open(rtxn, dbName.c_str(), MDB_CREATE, &dbi);
                mdb_txn_commit(rtxn);
                
                dbs.push_back({dbi});
                int idx = (int)dbs.size() - 1;
                
                return jsi::Value(idx);
            }
        );
        
        auto beginTransaction = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "beginTransaction"),
            0, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
                MDB_txn* txn;
                mdb_txn_begin(env, nullptr, MDB_RDONLY, &txn);
                txns.push_back({txn});
                int idx = (int)txns.size() - 1;
                return jsi::Value(idx);
            }
        );
        
        auto resetTransaction = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "resetTransaction"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
                
                if(!args[0].isNumber())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto idx = args[0].asNumber();
                auto txn = txns[idx];
                mdb_txn_reset(txn);
                
                return nullptr;
            }
        );
        
        auto commitTransaction = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "commitTransaction"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
                
                if(!args[0].isNumber())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto idx = args[0].asNumber();
                auto txn = txns[idx];
                mdb_txn_commit(txn);
                return nullptr;
            }
        );

        auto put = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "put"),
            3, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isNumber()|| !args[1].isString() || !args[2].isString())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }
                
                auto dbiIdx = args[0].asNumber();
                auto key = args[1].asString(runtime).utf8(runtime);
                auto data = args[2].asString(runtime).utf8(runtime);
              
                auto dbi = dbs[dbiIdx];
//                auto dbi = db;
                
                MDB_txn* txn = nullptr;
                mdb_txn_begin(env, nullptr, 0, &txn);
                
                MDB_val keyV{key.size(), key.data()};
                MDB_val dataV{data.size(), data.data()};
                
                mdb_put(txn, dbi, &keyV, &dataV, MDB_APPEND);
                mdb_txn_commit(txn);

                return jsi::Value(nullptr);
            }
        );
        
        auto putBatch = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "putBatch"),
            2, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isNumber() || !args[1].isObject())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }
                
                auto dbiKey = args[0].asNumber();
                auto batchObj = args[1].asObject(runtime);
                
                if (!batchObj.isArray(runtime))
                {
                    throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto batchArr = batchObj.asArray(runtime);
                auto batchArrLength = batchArr.getProperty(runtime, "length").asNumber();
                
//                auto dbi = dbs[dbiKey];
//                auto dbi = db;
                
                MDB_txn* txn = nullptr;
                mdb_txn_begin(env, nullptr, 0, &txn);
                
                for (int i = 0; i < batchArrLength; i++)
                {
                    auto item = batchArr.getValueAtIndex(runtime, i);
                    if (!item.isObject())
                    {
                        throw jsi::JSError(runtime, "Invalid batch data object");
                    }
                    auto dataObj = item.asObject(runtime);
                    auto key = dataObj.getProperty(runtime, "key");
                    if (!key.isString())
                    {
                        throw jsi::JSError(runtime, "Invalid batch data key");
                    }
                    auto keyStr = key.asString(runtime).utf8(runtime);
                    
                    auto value = dataObj.getProperty(runtime, "value");
                    if (!value.isString())
                    {
                        throw jsi::JSError(runtime, "Invalid batch data value");
                    }
                    auto valueStr = value.asString(runtime).utf8(runtime);
                    MDB_val keyV{keyStr.size(), keyStr.data()};
                    MDB_val dataV{valueStr.size(), valueStr.data()};
//                    mdb_put(txn, dbi, &keyV, &dataV, 0);
                }
                
                mdb_txn_commit(txn);
                
                return jsi::Value(nullptr);
            }
        );
        
        auto get = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "get"),
            3, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isNumber() || !args[1].isString() || !args[2].isNumber())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto dbiIdx = args[0].asNumber();
                auto key = args[1].asString(runtime).utf8(runtime);
                auto tidx = args[2].asNumber();
                
                auto dbi = dbs[dbiIdx];
                auto txn = txns[tidx];
                
                MDB_val keyV{key.size(), key.data()};
                MDB_val dataV;
                int rc = ::mdb_get(txn, dbi, &keyV, &dataV);
                if (rc == MDB_SUCCESS) {
                    auto data = std::string{std::string_view(static_cast<char*>(dataV.mv_data), dataV.mv_size)};
                    return jsi::String::createFromUtf8(runtime, data);
                }
                if (rc != MDB_NOTFOUND) {
                    throw jsi::JSError(runtime, "Unable to get value");
                }
                return jsi::Value(nullptr);
            }

         );
    

        auto del = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "del"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isNumber() || !args[1].isString())
                {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }
                auto dbiKey = args[0].asNumber();
                auto key = args[1].asString(runtime).utf8(runtime);
                
                auto dbi = dbs[dbiKey];
                
                MDB_txn* txn = nullptr;
                MDB_val keyV{key.size(), key.data()};
                mdb_del(txn, dbi, &keyV, nullptr);
                mdb_txn_commit(txn);
               
                return jsi::Value(nullptr);
            }
        );
        
        auto drop = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "drop"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
//                env.close();
//                std::string fullDocPathStr = docPathStr + "/" + dbName + "/";
//                cleanDirectory(fullDocPathStr);
//
//                mkdirs(fullDocPathStr.c_str());
//                env = lmdb::env::create();
//                env.set_mapsize((long)mapSize);
//                env.open(fullDocPathStr.c_str(), MDB_CREATE, 0664);
        
                return jsi::Value(nullptr);
            }
        );
        
        auto jsiRuntimeGlobal = jsiRuntime.global();

        jsiRuntimeGlobal.setProperty(jsiRuntime, "init", std::move(init));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "open", std::move(open));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "put", std::move(put));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "putBatch", std::move(putBatch));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "get", std::move(get));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "del", std::move(del));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "drop", std::move(drop));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "beginTransaction", std::move(beginTransaction));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "resetTransaction", std::move(resetTransaction));
        jsiRuntimeGlobal.setProperty(jsiRuntime, "commitTransaction", std::move(commitTransaction));
    }

    void cleanUp()
    {
    }

}
