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

    std::atomic<unsigned int> activeThreads; //counts the number of threads that process directories
    std::queue<std::string> directoriesQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    
    std::atomic<bool> isNotFound;

public:
    PathFounder(const std::string &startDirectory, const std::string &wantedDir);
    void searchDirectory();
    void run();
    void processDirectory(const std::string &startDirectory);
    void processing();
    void reset();
    std::string getResultPath();
};

#endif //PATH_FOUNDER_H