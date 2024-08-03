#pragma once

#include <unordered_map>

#include "cache_dir.hpp"

/* CACHE_DIR stores only hashes of data. MEMORY stores 
data in unorderd_map, where X's ara data, X_HASHes are
corresponding data hashes. To see how CACHE_DIR and it's 
sections T1, T2, B1, B2 look like, check cache_dir.hpp */

template <typename T>
class arc final {
    std::unordered_map<std::size_t, T> memory;
    cache_directory cache_dir;
    std::size_t capacity;
    std::hash<T> hash_function;

    inline void place_in_memory (const T& x, std::size_t x_hash) { memory [x_hash] = x; }
    inline void remove_from_memory (std::size_t x_hash) { memory.extract (x_hash); }

    bool T_case (std::size_t x_hash);
    bool B1_case (const T& x, std::size_t x_hash);
    bool B2_case (const T& x, std::size_t x_hash);
    void nowhere_case (const T& x, std::size_t x_hash);

public:
    arc (std::size_t i_capacity) : 
        cache_dir (i_capacity), capacity (i_capacity) {
            memory.reserve (i_capacity);
    }
    bool cache (const T& x);
    void dump () { cache_dir.dump(); }
};

template <typename T>
bool arc<T>::T_case (std::size_t x_hash) { 
    return cache_dir.process_T_case (x_hash); 
}

template <typename T>
bool arc<T>::B1_case (const T& x, std::size_t x_hash) {
    std::size_t to_remove;

    if (cache_dir.process_B1_case (x_hash, to_remove)) {   
        remove_from_memory (to_remove);
        place_in_memory (x, x_hash);

        return true;
    }

    return false;
}

template <typename T>
bool arc<T>::B2_case (const T& x, std::size_t x_hash) {
    std::size_t to_remove;

    if (cache_dir.process_B2_case (x_hash, to_remove)) { 
        remove_from_memory (to_remove);
        place_in_memory (x, x_hash);

        return true;
    }

    return false;
}

template <typename T>
void arc<T>::nowhere_case (const T &x, std::size_t x_hash) {
    std::size_t to_remove;
    cache_dir.process_nowhere_case (to_remove);
    remove_from_memory (to_remove);

    cache_dir.store_new_hash (x_hash);
    place_in_memory (x, x_hash);
}

template <typename T>
bool arc<T>::cache (const T& x) {
    std::size_t x_hash = hash_function (x);
    std::list<std::size_t>::iterator it;

    if (T_case (x_hash))
        return true;
    
    else if (B1_case (x, x_hash))
        return true;
    
    else if (B2_case (x, x_hash))
        return true;

    else    
        nowhere_case (x, x_hash); 
        
    return false;     
}

