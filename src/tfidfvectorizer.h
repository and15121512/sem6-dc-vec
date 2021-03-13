#pragma once
#ifndef TFIDFVECTORIZER_H
#define TFIDFVECTORIZER_H

#include <unordered_map>
#include <string>

struct TfIdfVectorizer {

    static std::string readWordsFromFile(const std::string& file_name);
    static int32_t crc32(const std::string& str);
    static void setDict(const std::vector<int32_t>& dict_vec);
    
    static void setDict(const std::string& file_name);
    static std::unordered_map<int32_t, int32_t> countVector(const char* words);

    static std::unordered_map<int32_t, int32_t> dict;
};

#endif // #ifndef TFIDFVECTORIZER_H
