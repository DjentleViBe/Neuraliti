#include "Extras.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <filesystem>

int verbose;
std::string logs;

void printverbose(std::string verb, bool check){
    if(check == true){
        std::cout << verb;
    }
    else{
        
    }
}

void addlogs(std::string logtoadd){
    if(verbose==0){
        logs += logtoadd;
    }
}

std::vector<std::string> listfiles(std::string path){
    std::vector<std::string> fileList; // Vector to store file paths
    std::vector<std::string> fileLoc; // filepath
    std::cout << path;
    // Check if the path exists and is a directory
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        // Iterate over each file in the directory
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            fileLoc = splitString(entry.path().string(), '/');
            fileList.push_back(fileLoc[fileLoc.size() - 1]);
        }
    } else {
        std::cerr << "Error: Specified path is not a directory or does not exist." << std::endl;
    }
    return fileList;
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> substrings;
    size_t start = 0;
    size_t end = input.find(delimiter);

    while (end != std::string::npos) {
        // Extract substring and add to vector
        substrings.push_back(input.substr(start, end - start));
        // Move start index to next character after delimiter
        start = end + 1;
        // Find next occurrence of delimiter
        end = input.find(delimiter, start);
    }

    // Add the last substring (after the last delimiter)
    substrings.push_back(input.substr(start));

    return substrings;
}
