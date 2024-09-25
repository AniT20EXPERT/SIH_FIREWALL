#include <iostream>
#include <fstream>
#include <string>
#include <curl/curl.h>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp> 

bool isJsonFileEmpty(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::ofstream createFile(filePath);
        if (createFile.is_open()) {
            createFile.close(); // Close the file after creation
            std::cout << "Created empty file: " << filePath << std::endl;
        }
        else {
            std::cerr << "Could not create the file: " << filePath << std::endl;
            return false;
        }
        return true; // The file is considered empty since it was just created
    }
    return file.peek() == std::ifstream::traits_type::eof();
}

// Callback function for cURL
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* str = static_cast<std::string*>(userp);
    size_t totalSize = size * nmemb;
    str->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

// Function to perform a GET request and save the JSON response
bool saveJsonFromGetRequest(const std::string& url, const std::string& outputFilePath) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        else {
            std::ofstream outputFile(outputFilePath);
            if (outputFile.is_open()) {
                outputFile << readBuffer;
                outputFile.close();
                std::cout << "JSON content saved to " << outputFilePath << std::endl;
            }
            else {
                std::cerr << "Could not open file for writing: " << outputFilePath << std::endl;
                curl_easy_cleanup(curl);
                return false;
            }
        }
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return true;
}

// Function to perform a GET request and return the JSON response
std::string getJsonResponse(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            return "";
        }
    }
    return readBuffer;
}

// SHA-256 hash function
std::string sha256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)data.c_str(), data.size(), hash);

    std::ostringstream oss;
    for (const auto& byte : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    }
    return oss.str();
}

// Normalize JSON function to ensure keys are sorted
std::string normalizeJson(const std::string& jsonString) {
    nlohmann::json jsonData = nlohmann::json::parse(jsonString);
    return jsonData.dump(); // Dump without indentation for consistent formatting
}

// Update your hash function to use this normalization
std::string hashJsonFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    std::string jsonContent = normalizeJson(buffer.str()); // Normalize before hashing
    return sha256(jsonContent);
}

std::string getRuleHash(const std::string& jsonString) {
    nlohmann::json jsonData = nlohmann::json::parse(jsonString);
    if (jsonData.contains("rule_Hash")) {
        return jsonData["rule_Hash"].get<std::string>();
    }
    else {
        throw std::runtime_error("Key 'rule_Hash' not found in JSON.");
    }
}

void printJsonFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Could not open the file: " << filePath << std::endl;
        return;
    }

    nlohmann::json jsonData;
    file >> jsonData;
    file.close();

    std::cout << jsonData.dump(4) << std::endl; // Pretty print with indentation
}

int main() {
    std::string baseurl = "http://localhost:3000";
    std::string local_rulefilePath = "C:/Users/aniru/Downloads/SIH_RULES_UPDATOR/cpp_filechecker/instruction_file/rule_file.json";
    std::string local_rulefile_hash = "";

    // Initialize or load the rule file
    if (isJsonFileEmpty(local_rulefilePath)) {
        std::cout << "The JSON file was empty." << std::endl;
        saveJsonFromGetRequest(baseurl + "/rules_body", local_rulefilePath);
        local_rulefile_hash = hashJsonFile(local_rulefilePath);
    }
    else {
        std::cout << "The JSON file was not empty." << std::endl;
        local_rulefile_hash = hashJsonFile(local_rulefilePath);
    }

    while (true) {
        std::string server_responsestring = getJsonResponse(baseurl + "/rules_hash");
        std::string server_rule_file_hash = getRuleHash(server_responsestring);

       // std::cout << "Current Local Hash: " << local_rulefile_hash << std::endl;
       // std::cout << "Current Server Hash: " << server_rule_file_hash << std::endl;

        if (local_rulefile_hash != server_rule_file_hash) {
            std::cout << "Hashes differ. Updating local JSON file..." << std::endl;
            saveJsonFromGetRequest(baseurl + "/rules_body", local_rulefilePath);
            local_rulefile_hash = hashJsonFile(local_rulefilePath); // Update the local hash
            local_rulefile_hash = server_rule_file_hash;
        }
        else {
            std::cout << "Hashes are the same. No update needed." << std::endl;
        }

        //printJsonFromFile(local_rulefilePath);
       // std::cout << "Server Rule File: " << getJsonResponse(baseurl + "/rules_body") << '\n';

        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
    return 0;
}
