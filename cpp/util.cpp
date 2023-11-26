//
//  util.cpp
//  react-native-lmdb
//
//  Created by Richard Willis on 26/11/2023.
//
#include "util.hpp"
#include <dirent.h>
#include <sstream>
#include <string>
#include <sys/stat.h>

namespace rnlmdb {

bool folder_exists(const std::string& folderName) {
  struct stat buffer;
  return (stat(folderName.c_str(), &buffer) == 0);
}

int _mkdir(const char* path) {
#if _POSIX_C_SOURCE
  return mkdir(path);
#else
  return mkdir(path, 0755);
#endif
}

int mkdirs(const char* path) {
  std::string current_level = "/";
  std::string level;
  std::stringstream ss(path);
  // First line is empty because it starts with /User
  getline(ss, level, '/');
  while (getline(ss, level, '/')) {
    current_level += level;
    if (!folder_exists(current_level) && _mkdir(current_level.c_str()) != 0) {
      return -1;
    }
    current_level += "/";
  }

  return 0;
}

bool cleanDirectory(const std::string& path) {
  struct dirent* ent;
  auto* dir = opendir(path.c_str());
  if (dir != NULL) {
    /* remove all the files and directories within directory */
    while ((ent = readdir(dir)) != NULL) {
      std::remove((path + ent->d_name).c_str());
    }
    closedir(dir);
  } else {
    /* could not open directory */
    return false;
  }
  return true;
}
} // namespace rnlmdb
