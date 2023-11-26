#ifndef LMDB_H
#define LMDB_H

#include <jsi/jsilib.h>
#include <jsi/jsi.h>

#include "../lmdb/libraries/liblmdb/lmdb.h"

namespace rnlmdb {
  void install(facebook::jsi::Runtime& jsiRuntime, const char *docPath);
  void cleanUp();
}
//
#endif /* LMDB_H */
