#include <jni.h>
#include <string>
#include <jsi/jsi.h>
#include <sys/types.h>
#include "react-native-lmdb.h"

using namespace facebook;

extern "C" JNIEXPORT void JNICALL
Java_com_lmdb_LmdbModule_nativeInitialize(JNIEnv *env, jclass clazz, jlong jsiPtr, jstring docPath) {
    jboolean isCopy;
    const char *docPathString = (env)->GetStringUTFChars(docPath, &isCopy);
    auto runtime = reinterpret_cast<facebook::jsi::Runtime*>(jsiPtr);
    if (runtime) {
        rnlmdb::install(*runtime, docPathString);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_lmdb_LmdbModule_nativeDestruct(JNIEnv *env, jclass clazz)
{
    rnlmdb::cleanUp();
}
