#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "tfidfvectorizer.h"

#include <doctest/doctest.h>
#include <crc32/Crc32.h>

#include <limits>
#include <utility>
#include <vector>
#include <string>



TEST_CASE("countVector and crc32, just works") {
    std::vector<std::wstring> words;
    std::vector<std::wstring> dict = { L"aaa", L"bbb", L"ccc", L"ddd" };
    // test values (crc_vec) were calculated with https://github.com/d-bahr/CRCpp/tree/master/inc
    std::vector<int32_t> crc_vec = { -1927890980, 840579423, 1491188958, -1754127848 };
    std::unordered_map<int32_t, int32_t> ans;

    const size_t kStrTypeCnt = 10;
    for (size_t i = 0; i < kStrTypeCnt; ++i) {
        words.push_back(L"aaa");
        words.push_back(L"bbb");
        words.push_back(L"ccc");
        words.push_back(L"ddd");
    }

    for (size_t i = 0; i < 4; ++i) {
        int32_t crc32_val = TfIdfVectorizer::crc32(dict[i]);
        CHECK(crc_vec[i] == crc32_val);
        ans.insert( std::make_pair(crc32_val, kStrTypeCnt) );
    }

    std::unordered_map<int32_t, int32_t> res = TfIdfVectorizer::countVector(words);
    CHECK(res == ans);
}

TEST_CASE("wordsFromFile") {
    std::vector<std::wstring> words_test = { L"alice", L"\'s", L"adventure", L"in", L"wonderland", L"alice", L"\'s" };
    std::vector<std::wstring> ans = TfIdfVectorizer::wordsFromFile("benchmark0_test.txt");
    CHECK(ans == words_test);

    // doesn't exist
    ans = TfIdfVectorizer::wordsFromFile("aaa.txt");
    CHECK(ans.size() == 0);
}

