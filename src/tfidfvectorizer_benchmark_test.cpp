#include <tfidfvectorizer.h>
#include <crc32/Crc32.h>

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <chrono>



class Timer
{
public:
    Timer() : m_beg(clock_t::now())
    {
    }

    void reset()
    {
        m_beg = clock_t::now();
    }

    double elapsed() const
    {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }

private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;

    std::chrono::time_point<clock_t> m_beg;
};

int main(int argc, char** argv) {
    Timer timer;

    // ------ CRC32 LIBS COMPARING ------
    std::vector<std::string> crc32_versions = { "CRC++", 
                                                "Fast_CRC32 crc32_fast", 
                                                "Fast_CRC32 crc32_16bytes_prefetch", 
                                                "Fast_CRC32 crc32_16bytes", 
                                                "Fast_CRC32 crc32_8bytes", 
                                                "Fast_CRC32 crc32_4bytes", 
                                                "Fast_CRC32 crc32_1byte", 
                                                "Fast_CRC32 crc32_halfbyte" };
    
    std::vector<std::string> benchmarks = { "benchmark0.txt", "benchmark1.txt" };
    
    for (const auto& benchmark : benchmarks) {
        std::cout << "------ BENCHMARK " << benchmark << " ------" << std::endl << std::endl;
        std::vector<std::wstring> words = TfIdfVectorizer::wordsFromFile(benchmark);

        std::cout << "Word count: " << words.size() << std::endl << std::endl;

        for (size_t i = 0; i < crc32_versions.size(); ++i) {
            std::cout << "------ " << crc32_versions[i] << " ------" << std::endl;

            timer.reset();
            std::unordered_map<int32_t, int32_t> count_vector = TfIdfVectorizer::countVector(words, crc32_versions[i]);
            double duration = timer.elapsed();

            std::cout << "Execution time duration (sec): " << duration << std::endl;
            std::cout << "-------------------------" << std::endl;
        }

        std::cout << "-------------------------" << std::endl << std::endl;
    }

    // ------ SIZE INCREASING ------
    std::vector<std::wstring> words = TfIdfVectorizer::wordsFromFile("benchmark1.txt");
    std::vector<double> is = { 0.001, 0.005, 0.01, 0.02, 0.03, 0.05, 0.1, 0.5, 1 };
    for (double i : is) {
        std::cout << "-------------------------" << std::endl;
        
        std::vector<std::wstring> words_part = words;
        words_part.resize(words.size() * i);
        
        timer.reset();
        std::unordered_map<int32_t, int32_t> count_vector = TfIdfVectorizer::countVector(words_part);
        double duration = timer.elapsed();
        
        std::cout << "Execution time duration (sec): " << duration << std::endl;
        std::cout << "Word count (input): " << words_part.size() << std::endl;
        std::cout << "Dimension (output): " << count_vector.size() << std::endl;

        std::cout << "-------------------------" << std::endl << std::endl;
    }

    return 0;
}