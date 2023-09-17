#include "PathFounder.h"

void PathFounder::run(const std::string &startDirectory) {
    const unsigned int maxThreads = std::thread::hardware_concurrency() - 1;
    
    std::vector<std::thread> threads;
    for(unsigned int i = 0; i < maxThreads; ++i) {
        threads.emplace_back(&PathFounder::searchDirectory, this);
    }


    std::thread processingThread(&PathFounder::processing, this);


    for (auto &thread : threads) {
        thread.join();
    }
    processingThread.join();
}


void PathFounder::processDirectory(const std::string &startDirectory) {
    try {
        for(const auto &entry : std::filesystem::directory_iterator(startDirectory)) {
            if(!this->isNotFound) {
                break;
            }
            if(entry.is_directory() && entry.path().filename() != this->wantedDir) {
                std::unique_lock<std::mutex> lock(queueMutex);

                this->directoriesQueue.push(entry.path().string());
                lock.unlock();

                this->queueCV.notify_one();
            } else if(entry.is_directory() && entry.path().filename() == this->wantedDir) {
                this->resultPath = "Full path to the directory -> " + entry.path().string();
                std::cout << "Thread " << std::this_thread::get_id() << entry.path() << std::endl;
                this->isNotFound = false;
            }
        } 
    }   catch(const std::filesystem::filesystem_error &e) {
        if(this->isNotFound) {
            std::cout << e.what() << std::endl;
        }
    }
}


void PathFounder::searchDirectory() {
    while(this->isNotFound) {
        std::string currentDirectory;
        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if(this->directoriesQueue.empty()) {
                if(this->activeThreads == 0) {
                    break;
                }
                queueCV.wait(lock);
            } else {
                currentDirectory = directoriesQueue.front();
                directoriesQueue.pop();
                activeThreads++;
            }
        }

        if(!currentDirectory.empty()) {
            processDirectory(currentDirectory);
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


PathFounder::PathFounder(const std::string &startPath, const std::string &wantedDir) {
    this->directoriesQueue.push(startPath);
    this->wantedDir = wantedDir;
    this->isNotFound = true;
}