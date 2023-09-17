#ifndef PATH_FOUNDER_H
#define PATH_FOUNDER_H

#include <iostream>
#include <filesystem>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

class PathFounder {
private:
    std::string startPath;
    std::string wantedDir;
    std::string resultPath;

    std::atomic<int> activeThreads{0};
    std::queue<std::string> directoriesQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    
    std::atomic<bool> isNotFound;

public:
    PathFounder(const std::string &startDirectory, const std::string &wantedDir);
    void searchDirectory();
    void run(const std::string &startDirectory);
    void processDirectory(const std::string &startDirectory);
    void processing();
    
    std::string getResultPath();
};

#endif //PATH_FOUNDER_H