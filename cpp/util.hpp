//
//  util.hpp
//  react-native-lmdb
//
//  Created by Richard Willis on 26/11/2023.
//

#ifndef util_hpp
#define util_hpp

#include <stdio.h>
#include <string>

namespace rnlmdb {
int mkdirs(const char* path);
bool cleanDirectory(const std::string& path);
} // namespace rnlmdb

#endif /* util_hpp */
