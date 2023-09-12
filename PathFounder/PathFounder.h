#ifndef PATH_FOUNDER_H
#define PATH_FOUNDER_H

#include <iostream>
#include <filesystem>

class PathFounder {
private:
    std::string startPath;
    std::string wantedDir;

public:
    PathFounder(const std::string &startDir, const std::string &wantedDir);
    void processDirectory(const std::string &startDir, const std::string &wantedDir);
};

#endif PATH_FOUNDER_H