#include "belady.hpp"

int main() {
    std::size_t capacity;
    std::size_t num_of_elems;

    std::cin >> capacity;
    std::cin >> num_of_elems;

    std::vector<std::size_t> i_hash_vec;
    std::size_t x_hash;

    for (auto i = 0; i < num_of_elems; ++i) {
        std::cin >> x_hash;
        i_hash_vec.push_back (x_hash);
    }

    belady genious_cache (capacity);

    std::cout << genious_cache.cache (i_hash_vec) << std::endl;
}