#include <jni.h>
#include "react-native-lmdb.h"

std::string getString(JNIEnv *env, jstring jStr) {
    const char *cstr = env->GetStringUTFChars(jStr, NULL);
    std::string str = std::string(cstr);
    env->ReleaseStringUTFChars(jStr, cstr);
    return str;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lmdb_LmdbModule_nativeOpen(JNIEnv *env, jclass type, jstring dbPath, jdouble mapSize) {
    std::string dbPathStr = getString(env, dbPath);
    rnlmdb::open(dbPathStr, mapSize);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lmdb_LmdbModule_nativePut(JNIEnv *env, jclass type, jstring key, jstring value) {
    std::string keyStr = getString(env, key);
    std::string valueStr = getString(env, value);
    rnlmdb::put(keyStr, valueStr);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_lmdb_LmdbModule_nativeGet(JNIEnv *env, jclass type, jstring key) {
    std::string keyStr = getString(env, key);
    std::string result = rnlmdb::get(keyStr);
    return (env)->NewStringUTF(result.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_lmdb_LmdbModule_nativeDel(JNIEnv *env, jclass type, jstring key) {
    std::string keyStr = getString(env, key);
    rnlmdb::del(keyStr);
}
