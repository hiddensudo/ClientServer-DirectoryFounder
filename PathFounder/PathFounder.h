#ifndef PATH_FOUNDER_H
#define PATH_FOUNDER_H

#include <iostream>
#include <filesystem>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

class PathFounder {
private:
    std::string startPath;
    std::string wantedDir;
    
    std::atomic<bool> isNotFound;

public:
    PathFounder(const std::string &startDir, const std::string &wantedDir);
    void searchDirectory(const std::string &startDir, const std::string &wantedDir);
    void processDirectory(const std::string &startDir, const std::string &wantedDir);
    void processing();
};

#endif //PATH_FOUNDER_H