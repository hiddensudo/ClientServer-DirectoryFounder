#include "PathFounder.h"

void PathFounder::processDirectory (const std::string &startDir, const std::string &wantedDir) {
    try {
        for(const auto &entry : std::filesystem::recursive_directory_iterator(startDir)) {
            if(entry.is_directory() && entry.path().filename() == wantedDir) {
                std::cout << "The result is: " << entry.path() << std::endl;
                break;
            }
        }
    } catch(const std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
    }
    
}

PathFounder::PathFounder(const std::string &startPath, const std::string &wantedDir) {
    this->startPath = startPath;
    this->wantedDir = wantedDir;
}