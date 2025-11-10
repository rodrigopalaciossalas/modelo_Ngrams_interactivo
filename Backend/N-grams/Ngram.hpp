#include <iostream>
#include <vector>
#include <string>

class Ngram {
    protected:
        std::vector<std::string> ngrams;
        size_t n;
    public:
        Ngram(size_t n) : n(n) {}
        virtual void generate(const std::string& text) = 0;
        const std::vector<std::string>& getNgrams() const { return ngrams; }
};