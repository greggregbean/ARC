#include "arc.hpp"

int main () {   
    std::size_t capacity;
    std::size_t num_of_elems;
    std::size_t num_of_hits = 0;

    std::cin >> capacity;
    std::cin >> num_of_elems;

    arc<int> cache_mem (capacity);

    int x;
    std::size_t x_hash;

    for(std::size_t i = 0; i < num_of_elems; ++i) {
      std::cin >> x;
      x_hash = cache_mem.count_hash(x);

      if (cache_mem.in_cache_p (x_hash)) {
        cache_mem.get_from_cache (x_hash);
        num_of_hits++;
      }

      else {
        cache_mem.place_in_cache (x, x_hash);
      }
    } 

    std::cout << num_of_hits << std::endl;
}