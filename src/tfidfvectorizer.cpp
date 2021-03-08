#include <crc32/Crc32.h>
#include <crc32_1/CRC.h>

#include <tfidfvectorizer.h>

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <math.h>

CRC::Table<std::uint32_t, 32> TfIdfVectorizer::table(CRC::CRC_32());

std::vector<std::wstring> TfIdfVectorizer::wordsFromFile(const std::string& file_name) {
    // Args: 
    //    file_name --- name of file with words to count, by word per line
    // Ret: 
    //    vector of these words
    std::wifstream wfin;
    wfin.open(file_name);
    if (!wfin) {
        throw std::runtime_error("TfIdfVectorizer::wordsFromFile: the file can't be opened");
    }

    std::vector<std::wstring> words;
    for (std::wstring word; wfin >> word; words.push_back(word)) { }
    wfin.close();
    return words;
}

int32_t TfIdfVectorizer::crc32(const std::wstring& wstr, const std::string& version) {
    uint32_t crc_uint;
    if (version == "CRC++") {
        crc_uint = CRC::Calculate(wstr.data(), wstr.size() * sizeof(wchar_t), table);
    }
    else if (version == "Fast_CRC32 crc32_fast") {
        crc_uint = crc32_fast(wstr.data(), wstr.size() * sizeof(wchar_t), 0);
    }
    else if (version == "Fast_CRC32 crc32_16bytes_prefetch") {
        crc_uint = crc32_16bytes_prefetch(wstr.data(), wstr.size() * sizeof(wchar_t), 0, 256);
    }
    else if (version == "Fast_CRC32 crc32_16bytes") {
        crc_uint = crc32_16bytes(wstr.data(), wstr.size() * sizeof(wchar_t), 0);
    }
    else if (version == "Fast_CRC32 crc32_8bytes") {
        crc_uint = crc32_8bytes(wstr.data(), wstr.size() * sizeof(wchar_t), 0);
    }
    else if (version == "Fast_CRC32 crc32_4bytes") {
        crc_uint = crc32_4bytes(wstr.data(), wstr.size() * sizeof(wchar_t), 0);
    }
    else if (version == "Fast_CRC32 crc32_1byte") {
        crc_uint = crc32_1byte(wstr.data(), wstr.size() * sizeof(wchar_t), 0);
    }
    else if (version == "Fast_CRC32 crc32_halfbyte") {
        crc_uint = crc32_halfbyte(wstr.data(), wstr.size() * sizeof(wchar_t), 0);
    }

    return *reinterpret_cast<int32_t*>(&crc_uint);
}

std::unordered_map<int32_t, int32_t> TfIdfVectorizer::countVector(const std::vector<std::wstring>& words,
                                                                  const std::string& version) {
    std::unordered_map<int32_t, int32_t> count_vector;

    for (size_t i = 0; i < words.size(); ++i) {
        int32_t word_crc32 = crc32(words[i], version);
        if (count_vector.end() == count_vector.find(word_crc32)) {
            count_vector.insert(std::make_pair(word_crc32, 1));
        }
        else {
            ++count_vector[word_crc32];
        }
    }
    return count_vector;
}
