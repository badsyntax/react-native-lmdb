#include <jni.h>
#include "react-native-lmdb.h"

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_lmdb_LmdbModule_nativeMultiply(JNIEnv *env, jclass type, jdouble a, jdouble b) {
    return lmdb::multiply(a, b);
}
