#include "react-native-lmdb.h"
#include "util.hpp"
#include <jsi/jsi.h>
#include <string>

using namespace facebook;

namespace rnlmdb {

std::string docPathStr;
std::vector<MDB_dbi> dbs;
std::vector<MDB_txn*> txns;

MDB_env* env = nullptr;
MDB_txn* rtxn = nullptr;

void install(jsi::Runtime& jsiRuntime, const char* docPath) {
  docPathStr = std::string(docPath);

  auto init = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "init"),
      2, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isString() || !args[1].isNumber()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }

        auto dbName = args[0].asString(runtime).utf8(runtime);
        auto mapSize = args[1].asNumber();

        auto fullDocPathStr = docPathStr + "/" + dbName + "/";
        mkdirs(fullDocPathStr.c_str());

        int rc = mdb_env_create(&env);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to create env");
        }
        rc = mdb_env_set_mapsize(env, (long)mapSize);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to set map size");
        }
        // @TODO
        rc = mdb_env_set_maxdbs(env, 50);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to set max dbs");
        }
        rc = mdb_env_open(env, fullDocPathStr.c_str(), 0, 0644);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to open env");
        }

        return nullptr;
      });

  auto open = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "open"),
      1, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isString()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }

        auto dbName = args[0].asString(runtime).utf8(runtime);

        MDB_dbi dbi;
        int rc = ::mdb_txn_begin(env, nullptr, 0, &rtxn);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to begin transaction");
        }
        rc = ::mdb_dbi_open(rtxn, dbName.c_str(), MDB_CREATE, &dbi);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to open dbi");
        }
        rc = ::mdb_txn_commit(rtxn);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to commit transaction");
        }

        dbs.push_back({dbi});
        int idx = (int)dbs.size() - 1;

        return jsi::Value(idx);
      });

  auto beginTransaction = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "beginTransaction"),
      1, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (count == 1 && !args[0].isBool()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }

        auto write = count == 1 ? args[0].asBool() : false;
        MDB_txn* txn;
        int rc = ::mdb_txn_begin(env, nullptr, write ? 0 : MDB_RDONLY, &txn);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to begin transaction");
        }
        txns.push_back({txn});
        int idx = (int)txns.size() - 1;
        return jsi::Value(idx);
      });

  auto resetTransaction = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "resetTransaction"),
      1, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isNumber()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }

        auto idx = args[0].asNumber();
        auto txn = txns[idx];
        ::mdb_txn_reset(txn);
        return nullptr;
      });

  auto commitTransaction = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "commitTransaction"),
      1, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isNumber()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }

        auto idx = args[0].asNumber();
        auto txn = txns[idx];
        int rc = ::mdb_txn_commit(txn);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to commit transaction");
        }
        return nullptr;
      });

  auto put = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "put"),
      4, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isNumber() || !args[1].isString() || !args[2].isString() ||
            !args[3].isNumber()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }

        auto dbiIdx = args[0].asNumber();
        auto key = args[1].asString(runtime).utf8(runtime);
        auto data = args[2].asString(runtime).utf8(runtime);
        auto tidx = args[3].asNumber();

        auto dbi = dbs[dbiIdx];
        auto txn = txns[tidx];

        MDB_val keyV{key.size(), key.data()};
        MDB_val dataV{data.size(), data.data()};

        int rc = ::mdb_put(txn, dbi, &keyV, &dataV, 0);

        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to put value");
        }

        return jsi::Value(nullptr);
      });

  auto get = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "get"),
      3, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isNumber() || !args[1].isString() || !args[2].isNumber()) {
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
          auto data =
              std::string{std::string_view(static_cast<char*>(dataV.mv_data), dataV.mv_size)};
          return jsi::String::createFromUtf8(runtime, data);
        }
        if (rc != MDB_NOTFOUND) {
          throw jsi::JSError(runtime, "Unable to get value");
        }
        return jsi::Value(nullptr);
      }

  );

  auto del = jsi::Function::createFromHostFunction(
      jsiRuntime, jsi::PropNameID::forAscii(jsiRuntime, "del"),
      2, // Number of arguments in function
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args,
         size_t count) -> jsi::Value {
        if (!args[0].isNumber() || !args[1].isString()) {
          throw jsi::JSError(runtime, "Invalid arguments");
        }
        auto dbiIdx = args[0].asNumber();
        auto key = args[1].asString(runtime).utf8(runtime);

        auto dbi = dbs[dbiIdx];

        MDB_txn* txn = nullptr;
        MDB_val keyV{key.size(), key.data()};
        int rc = ::mdb_del(txn, dbi, &keyV, nullptr);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to del");
        }
        rc = ::mdb_txn_commit(txn);
        if (rc != MDB_SUCCESS) {
          throw jsi::JSError(runtime, "Unable to commit transaction");
        }

        return jsi::Value(nullptr);
      });

  auto jsiRuntimeGlobal = jsiRuntime.global();

  jsiRuntimeGlobal.setProperty(jsiRuntime, "init", std::move(init));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "open", std::move(open));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "put", std::move(put));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "get", std::move(get));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "del", std::move(del));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "beginTransaction", std::move(beginTransaction));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "resetTransaction", std::move(resetTransaction));
  jsiRuntimeGlobal.setProperty(jsiRuntime, "commitTransaction", std::move(commitTransaction));
}

void cleanUp() {}

} // namespace rnlmdb
