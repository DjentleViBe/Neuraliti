#include "../dependencies/include/extras.h"
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <fstream>
#include "../dependencies/include/menu.h"

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
std::string properties;

void printverbose(std::string verb, bool check){
    if(check == true){
        // std::cout << verb;
    }
    else{
        
    }
}

void addlogs(std::string logtoadd){
    if(verbose==0){
        logs += logtoadd;
    }
}

void addproperties(std::string proptoadd){
    if(verbose==0){
        properties += proptoadd;
    }
}

void clearproperties(){
    properties = "";
}

std::vector<std::string> listfiles(std::string path, std::string type){
    std::vector<std::string> fileList; // Vector to store file paths
    std::vector<std::string> fileLoc; // filepath
    //std::cout << path;
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

int writefile(std::string file_path, std::string line_to_write){
    // Create an output file stream (ofstream) object
    std::ofstream outfile(file_path, std::ios::app);  // Open the file in append mode

    // Check if the file is open
    if (outfile.is_open()) {
        // Write the line to the file
        outfile << line_to_write << std::endl;

        // Close the file stream
        outfile.close();
        addlogs("Line written to file successfully");
        // std::cout << "Line written to file successfully." << std::endl;
    } else {
        addlogs("Filed to open the file");
        // std::cerr << "Failed to open the file." << std::endl;
    }
    return 0;
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

    // std::cout << "Line " << lineNumber << " overwritten successfully." << std::endl;
}

void printMatrix(const std::vector<std::vector<int>>& matrix, std::string verbose) {
    // Get the number of rows and columns in the matrix
    int rows = matrix.size();
    int cols = matrix[0].size(); // Assuming all rows have the same number of elements

    // Print the matrix
    // std::cout << verbose << "\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void printvector(const std::vector<int> vec, std::string verbose){
    std::cout << verbose << "\n";
    for (int i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}

std::string floatToString(float value, int precision) {
    std::ostringstream oss;
    oss.precision(precision);   // Set the precision
    oss << std::fixed << value; // Use fixed-point notation
    return oss.str();           // Return the string representation
}

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

bool isFloat(const std::string& str)
{
    char* ptr;
    strtof(str.c_str(), &ptr);
    return (*ptr) == '\0';
}