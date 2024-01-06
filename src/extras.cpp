#include "extras.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "menu.h"

// Platform-specific headers
#if defined(_WIN32)
#include <direct.h> // for _getcwd()
#define GetCurrentDir _getcwd
#else
#include <unistd.h> // for getcwd()
#define GetCurrentDir getcwd
#endif


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

std::vector<std::string> listfiles(std::string path, std::string type){
    std::vector<std::string> fileList; // Vector to store file paths
    std::vector<std::string> fileLoc; // filepath
    std::cout << path;
    fileList.push_back(appsettings["defaultfont"]);
    // Check if the path exists and is a directory
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        // Iterate over each file in the directory
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            fileLoc = splitString(entry.path().string(), '/');
            if (fileLoc[fileLoc.size() - 1].find(type)!= std::string::npos){
                fileList.push_back(fileLoc[fileLoc.size() - 1]);
            }
            
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

std::vector<std::string> readfile(char const *filename){
    std::string line;
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            //std::cout << line << "\n";
            lines.push_back(line);
        }
        file.close();
    }
    return lines;
}

std::string readfileconcat(char const *filename){
    std::string line;
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            //std::cout << line << "\n";
            lines.push_back(line);
        }
        file.close();
    }
    std::string concatenatedString;
    for (const std::string& str : lines) {
        concatenatedString += str;
    }
    return concatenatedString;
}

std::string GetCurrentWorkingDirectory() {
    char buffer[FILENAME_MAX];
    if (GetCurrentDir(buffer, sizeof(buffer)) != nullptr) {
        return std::string(buffer);
    } else {
        return std::string("");
    }
}

void overwriteLine(const std::string& filename, int lineNumber, const std::string& newContent) {
    // Read the file into memory
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();

    // Check if the line number is valid
    if (lineNumber < 1 || lineNumber > static_cast<int>(lines.size())) {
        std::cerr << "Error: Line number out of range." << std::endl;
        return;
    }

    // Modify the desired line
    lines[lineNumber - 1] = newContent;

    // Write the modified lines back to the file
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }

    for (const std::string& modifiedLine : lines) {
        outfile << modifiedLine << std::endl;
    }

    std::cout << "Line " << lineNumber << " overwritten successfully." << std::endl;
}
