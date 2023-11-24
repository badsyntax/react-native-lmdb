#include <string>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

#include "react-native-lmdb.h"
#include "lmdb++.h"

// call: jsi::Function instead of facebook::jsi::Function
using namespace facebook;

namespace rnlmdb {

    bool folder_exists(const std::string &foldername)
    {
      struct stat buffer;
      return (stat(foldername.c_str(), &buffer) == 0);
    }
    /**
     * Portable wrapper for mkdir. Internally used by mkdir()
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int _mkdir(const char *path)
    {
        #if _POSIX_C_SOURCE
            return mkdir(path);
        #else
            return mkdir(path, 0755); // not sure if this works on mac
        #endif
    }

    /**
     * Recursive, portable wrapper for mkdir.
     * @param[in] path the full path of the directory to create.
     * @return zero on success, otherwise -1.
     */
    int mkdir(const char *path)
    {
        std::string current_level = "/";
        std::string level;
        std::stringstream ss(path);
        // First line is empty because it starts with /User
        getline(ss, level, '/');
        // split path using slash as a separator
        while (getline(ss, level, '/'))
        {
            current_level += level; // append folder to the current level
                                // create current level
            if (!folder_exists(current_level) && _mkdir(current_level.c_str()) != 0) {
                return -1;
            }

            current_level += "/"; // don't forget to append a slash
        }

        return 0;
    }

    lmdb::env env = nullptr;
    std::string docPathStr;

    void install(jsi::Runtime& jsiRuntime, const char *docPath) {
        docPathStr = std::string(docPath);

        auto open = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "open"),
            2, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
              
                if(!args[0].isString() || !args[1].isNumber()) {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }
              
                auto dbName = args[0].asString(runtime).utf8(runtime);
                auto mapSize = args[1].asNumber();
                
                std::string fullDocPathStr = docPathStr + "/" + dbName;
                mkdir(fullDocPathStr.c_str());
                
                if (env != nullptr) {
                  env.close();
                }
                env = lmdb::env::create();
                env.set_mapsize((long)mapSize);
                env.open(fullDocPathStr.c_str(), MDB_CREATE, 0664);

                return jsi::Value();
            }
        );
        
        auto put = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "put"),
            2, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {
              
                if(!args[0].isString() || !args[1].isString()) {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }
              
                auto key = args[0].asString(runtime).utf8(runtime);
                auto value = args[1].asString(runtime).utf8(runtime);
                
                auto wtxn = lmdb::txn::begin(env);
                auto dbi = lmdb::dbi::open(wtxn, nullptr);
                dbi.put(wtxn, key, value);
                wtxn.commit();

                return jsi::Value();
            }
        );

        auto get = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "get"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isString()) {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto key = args[0].asString(runtime).utf8(runtime);

                auto rtxn = lmdb::txn::begin(env, nullptr, MDB_RDONLY);
                auto dbi = lmdb::dbi::open(rtxn, nullptr);

                std::string_view value;
                if (dbi.get(rtxn, key, value)) {
                    return jsi::String::createFromUtf8(runtime, std::string{value});
                }

                return jsi::Value(nullptr);
            }
        );

        auto del = jsi::Function::createFromHostFunction(
            jsiRuntime,
            jsi::PropNameID::forAscii(jsiRuntime, "get"),
            1, // Number of arguments in function
            [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) -> jsi::Value {

                if(!args[0].isString()) {
                  throw jsi::JSError(runtime, "Invalid arguments");
                }

                auto key = args[0].asString(runtime).utf8(runtime);

                auto wtxn = lmdb::txn::begin(env);
                auto dbi = lmdb::dbi::open(wtxn, nullptr);
                dbi.del(wtxn, key);
                wtxn.commit();

                return {};
            }
        );

        jsiRuntime.global().setProperty(jsiRuntime, "open", std::move(open));
        jsiRuntime.global().setProperty(jsiRuntime, "put", std::move(put));
        jsiRuntime.global().setProperty(jsiRuntime, "get", std::move(get));
        jsiRuntime.global().setProperty(jsiRuntime, "del", std::move(del));
    }

    void cleanUp() {
        // intentionally left blank
    }

}
