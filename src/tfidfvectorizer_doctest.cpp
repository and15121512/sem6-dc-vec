#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "tfidfvectorizer.h"

#include <doctest/doctest.h>
#include <crc32/Crc32.h>

#include <limits>
#include <utility>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>



TEST_CASE("countVector and crc32, just works") {
  std::string words;
  // test values (crc_vec) were calculated with https://github.com/d-bahr/CRCpp/tree/master/inc
  std::vector<int32_t> dict = {-267947219, 1080413965, 800826605, -70143732};
  std::unordered_map<int32_t, int32_t> ans;

  const size_t kStrTypeCnt = 10;
  for (size_t i = 0; i < kStrTypeCnt; ++i) {
    words += std::string(u8"aaa ");
    words += std::string(u8"bbb ");
    words += std::string(u8"ccc ");
    words += std::string(u8"ddd ");
  }

  for (size_t i = 0; i < 4; ++i) {
    ans.insert(std::make_pair(dict[i], kStrTypeCnt) );
  }

  TfIdfVectorizer::setDict(dict);
  std::unordered_map<int32_t, int32_t> res = TfIdfVectorizer::countVector(words.c_str());
  CHECK(res == ans);
}

TEST_CASE("crc32 of non-ASCII utf8, sample 2, Icelandic") {
  std::ifstream fin_crc32;
  std::ifstream fin_str;
  fin_crc32.open("dict2_crc32.txt");
  fin_str.open("dict2_str.txt");
  if (!(fin_crc32 && fin_str)) {
    throw std::runtime_error("Test 'crc32 of non-ASCII utf8': can't open test file");
  }

  std::string str;
  uint32_t crc;
  for (; fin_crc32 >> crc && fin_str >> str;) {
    CHECK(*reinterpret_cast<int32_t*>(&crc) == TfIdfVectorizer::crc32(str));
  }

  fin_crc32.close();
  fin_str.close();
}

TEST_CASE("countVector of non-ASCII utf8, sample 2, Icelandic") {
  TfIdfVectorizer::setDict("dict2_crc32.txt");
  
  std::string text = TfIdfVectorizer::readWordsFromFile("dict2_str.txt");
  std::unordered_map<int32_t, int32_t> cv = TfIdfVectorizer::countVector(text.data());

  std::ifstream fin;
  fin.open("counts2.txt");
  if (!fin) {
    throw std::runtime_error("Test 'countVector of non-ASCII utf8': can't open test file");
  }
  
  uint32_t crc;
  size_t cnt;
  for (; fin >> crc >> cnt; ) {
    int32_t scrc = *reinterpret_cast<int32_t*>(&crc);
    CHECK(cv[scrc] == cnt);
  }

  fin.close();
}

TEST_CASE("crc32 of non-ASCII utf8, sample 3, Anna Karenina") {
  std::ifstream fin_crc32;
  std::ifstream fin_str;
  fin_crc32.open("dict3_crc32.txt");
  fin_str.open("dict3_str.txt");
  if (!(fin_crc32 && fin_str)) {
    throw std::runtime_error(
        "Test 'crc32 of non-ASCII utf8': can't open test file");
  }

  std::string str;
  uint32_t crc;
  for (; fin_crc32 >> crc && fin_str >> str;) {
    CHECK(*reinterpret_cast<int32_t*>(&crc) == TfIdfVectorizer::crc32(str));
  }

  fin_crc32.close();
  fin_str.close();
}

TEST_CASE("countVector of non-ASCII utf8, sample 3, Anna Karenina") {
  TfIdfVectorizer::setDict("dict3_crc32.txt");

  std::string text = TfIdfVectorizer::readWordsFromFile("dict3_str.txt");
  std::unordered_map<int32_t, int32_t> cv =
      TfIdfVectorizer::countVector(text.data());

  std::ifstream fin;
  fin.open("counts3.txt");
  if (!fin) {
    throw std::runtime_error(
        "Test 'countVector of non-ASCII utf8': can't open test file");
  }

  uint32_t crc;
  size_t cnt;
  for (; fin >> crc >> cnt;) {
    int32_t scrc = *reinterpret_cast<int32_t*>(&crc);
    CHECK(cv[scrc] == cnt);
  }

  fin.close();
}
