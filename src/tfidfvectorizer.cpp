#include <crc32/Crc32.h>

#include <tfidfvectorizer.h>

#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

std::unordered_map<int32_t, int32_t> TfIdfVectorizer::dict;

std::string TfIdfVectorizer::readWordsFromFile(const std::string& file_name) {
  std::ifstream fin;
  fin.open(file_name);
  if (!fin) {
    throw std::runtime_error(
        "TfIdfVectorizer::readWordsFromFile: the file can't be opened");
  }

  std::string contents((std::istreambuf_iterator<char>(fin)), 
                       std::istreambuf_iterator<char>());
  fin.close();

  return contents;
}

void TfIdfVectorizer::setDict(const std::vector<int32_t>& dict_vec) {
  dict.clear();
  for (size_t i = 0; i < dict_vec.size(); ++i) {
    dict.insert(std::make_pair(dict_vec[i], 0));
  }
}

void TfIdfVectorizer::setDict(const std::string& file_name) {
  std::ifstream fin;
  fin.open(file_name);
  if (!fin) {
    throw std::runtime_error(
        "TfIdfVectorizer::setDict: the file can't be opened");
  }

  dict.clear();
  for (uint32_t ucrc32; fin >> ucrc32;) {
    int32_t scrc32 = *reinterpret_cast<int32_t*>(&ucrc32);
    dict.insert(std::make_pair(scrc32, 0));
  }
  fin.close();
}

int32_t TfIdfVectorizer::crc32(const std::string& str) {
  uint32_t crc_uint;
  crc_uint = crc32_fast(str.data(), str.size() * sizeof(std::string::value_type), 0);

  return *reinterpret_cast<int32_t*>(&crc_uint);
}

std::unordered_map<int32_t, int32_t> TfIdfVectorizer::countVector(const char* words) {
  if (nullptr == words) {
    throw std::runtime_error("TfIdfVectorizer::countVector: 'words' is nullptr");
  }
  std::unordered_map<int32_t, int32_t> count_vector = dict;
  
  std::istringstream ibuf;
  // split only by ASCII delimiters, not all UTF-8
  ibuf.str(std::string(words));
  for (std::string word; ibuf >> word;) {
    int32_t word_crc32 = crc32(word);
    if (count_vector.end() != count_vector.find(word_crc32)) {
      ++count_vector[word_crc32];
    }
  }
  return count_vector;
}
