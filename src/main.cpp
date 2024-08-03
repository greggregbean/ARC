#include "arc.hpp"

int main () 
{   
    std::size_t capacity;
    std::size_t num_of_elems;
    std::size_t num_of_hits = 0;

    std::cin >> capacity;
    std::cin >> num_of_elems;

    arc<int> cache_mem (capacity);

    int x;
    for(std::size_t i = 0; i < num_of_elems; ++i) {
      std::cin >> x;
      if (cache_mem.cache (x))
        num_of_hits++;
    } 

    std::cout << num_of_hits << std::endl;
}