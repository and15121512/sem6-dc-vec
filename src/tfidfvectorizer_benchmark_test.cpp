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

    std::cout << "------ Vectorization Speed Test ------" << std::endl << std::endl;

    TfIdfVectorizer::setDict("benchmark_crc32.txt");
    
    std::ifstream fin;
    fin.open("benchmark_str.txt");
    if (!fin) {
      throw std::runtime_error("Vectorization Speed Test: the file can't be opened");
    }
    
     std::string str = std::string((std::istreambuf_iterator<char>(fin)),
                                    std::istreambuf_iterator<char>());
    
    fin.close();

    timer.reset();
    std::unordered_map<int32_t, int32_t> cv = TfIdfVectorizer::countVector(str.data());
    double duration = timer.elapsed();

    // benchmark: 56.694 Mb
    std::cout << 56.694 / duration << " Mb/s" << std::endl;

    return 0;
}