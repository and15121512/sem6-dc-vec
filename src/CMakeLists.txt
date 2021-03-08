add_library(tfidfvectorizer
  tfidfvectorizer.h
  tfidfvectorizer.cpp
)
#set_property(TARGET tfidfvectorizer PROPERTY CXX_STANDARD 17)

add_executable(tfidfvectorizer_doctest
  tfidfvectorizer_doctest.cpp
)

add_executable(tfidfvectorizer_benchmark_test
  tfidfvectorizer_benchmark_test.cpp
)

target_link_libraries(tfidfvectorizer_doctest
  tfidfvectorizer
  Crc32
)

target_link_libraries(tfidfvectorizer_benchmark_test
  tfidfvectorizer
  Crc32
)

file(COPY ../data/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
