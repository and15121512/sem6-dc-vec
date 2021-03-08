#pragma once
#ifndef TFIDFVECTORIZER_H
#define TFIDFVECTORIZER_H

#include <crc32_1/CRC.h>

#include <unordered_map>
#include <string>

struct TfIdfVectorizer {

    static int32_t crc32(const std::wstring& wstr, const std::string& version = "CRC++");
    static std::vector<std::wstring> wordsFromFile(const std::string& file_name);
    static std::unordered_map<int32_t, int32_t> countVector(const std::vector<std::wstring>& words, 
                                                            const std::string& version = "CRC++");
    
    static CRC::Table<std::uint32_t, 32> table;
};

#endif // #ifndef TFIDFVECTORIZER_H
