#include "PathFounder.h"


void PathFounder::processDirectory(const std::string &startDir, const std::string &wantedDir) {
    std::vector<std::thread> threads;
    threads.emplace_back(&PathFounder::processing, this);
    threads.emplace_back(&PathFounder::searchDirectory, this, std::ref(startDir), std::ref(wantedDir));

    for (auto &thread : threads) {
        thread.join();
    }
}


void PathFounder::searchDirectory(const std::string &startDir, const std::string &wantedDir) {
    try {
        for(const auto &entry : std::filesystem::recursive_directory_iterator(startDir)) {
            if(entry.is_directory() && entry.path().filename() == wantedDir) {
                std::cout << std::this_thread::get_id << ": The result is: " << entry.path() << std::endl;
                this->isNotFound = false;
                break;
            }
        } 
    }   catch(const std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
    }
}

void PathFounder::processing() {
    while(isNotFound) {
        std::cout << "Thread with id" << std::this_thread::get_id << " in process..." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


PathFounder::PathFounder(const std::string &startPath, const std::string &wantedDir) {
    this->startPath = startPath;
    this->wantedDir = wantedDir;
    this->isNotFound = true;
}