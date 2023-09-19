#include "PathFounder.h"

void PathFounder::run() {
    reset();

    const unsigned int maxThreads = std::thread::hardware_concurrency();
    
    std::vector<std::thread> threads;
    for(unsigned int i = 0; i < maxThreads - 1; ++i) {
        threads.emplace_back(&PathFounder::searchDirectory, this);
    }

    std::thread processingThread(&PathFounder::processing, this);

    for (auto &thread : threads) {
        thread.join();
    }
    processingThread.join();

    if(this->resultPath.empty() && directoriesQueue.empty()) {
        this->resultPath = "Directory was not found!";
    }
}

// Adds directories to the queue and checks if this directory is the one the server wants to find.
void PathFounder::processDirectory(const std::string &startDirectory) {
    try {
        for(const auto &entry : std::filesystem::directory_iterator(startDirectory)) {
            if(!this->isNotFound) {
                break;
            }
            if(entry.is_directory() && entry.path().filename() != this->wantedDir && entry.path() != "/proc") {
                std::unique_lock<std::mutex> lock(queueMutex);

                this->directoriesQueue.push(entry.path().string());
                lock.unlock();

                this->queueCV.notify_one();
            } else if(entry.is_directory() && entry.path().filename() == this->wantedDir) {
                this->resultPath = "Full path to the directory -> " + entry.path().string();
                this->isNotFound = false;
            }
        } 
    }   catch(const std::filesystem::filesystem_error &e) {
        if(this->isNotFound) {
            //std::cout << e.what() << std::endl;
        }
    }
}

// Synchronizes threads for parallel execution of the processDirectory method
void PathFounder::searchDirectory() {
    while (this->isNotFound) {
        std::string currentDirectory;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (this->directoriesQueue.empty()) {
                if (this->activeThreads == 0) {
                    this->isNotFound = false;
                    queueCV.notify_all();
                    return;
                } else {
                    queueCV.wait(lock);
                }
            } else {
                currentDirectory = directoriesQueue.front();
                this->directoriesQueue.pop();
            }
        }

        if (!currentDirectory.empty()) {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                this->activeThreads++;
            }
            processDirectory(currentDirectory);
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                this->activeThreads--;
                if (this->activeThreads == 0 && this->directoriesQueue.empty()) {
                    this->isNotFound = false;
                    queueCV.notify_all();
                    return;
                } else {
                    queueCV.notify_one();
                }
            }
        }
    }
}

void PathFounder::processing() {
    while(this->isNotFound) {
        std::cout << "Processing..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

std::string PathFounder::getResultPath() {
    return this->resultPath;
}

//If the directory is found, the queue may not be empty, so we reset it
void PathFounder::reset() {
    while(!directoriesQueue.empty()) {
        directoriesQueue.pop();
    }
    directoriesQueue.push(this->startPath);
}


PathFounder::PathFounder(const std::string &startPath, const std::string &wantedDir) {
    this->startPath = startPath;
    this->directoriesQueue.push(this->startPath);
    this->wantedDir = wantedDir;
    this->isNotFound = true;
    this->activeThreads = 0;   
}