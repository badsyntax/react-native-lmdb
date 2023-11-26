#ifndef LMDB_H
#define LMDB_H

#include <jsi/jsi.h>
#include <jsi/jsilib.h>

#include "../lmdb/libraries/liblmdb/lmdb.h"

namespace rnlmdb {
void install(facebook::jsi::Runtime& jsiRuntime, const char* docPath);
void cleanUp();
} // namespace rnlmdb
//
#endif /* LMDB_H */
