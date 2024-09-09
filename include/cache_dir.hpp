#pragma once

#include <iostream>
#include <list>  
#include <algorithm>

/* Illustration of ARC cache directory model:

        [6 5 4 3 2 1 0[6 5 4 3 2 1 0|0 1 2 3 4 5 6]0 1 2 3 4 5 6]
        [     B1    <-[     T1    <-|->      T2   ]->    B2     ] 
        [ . . . . . . [ . . . . . . | . .^. . . . ] . . . . . . ]
                                        (p)
                      [<-------(capacity)-------->]               */

struct cache_directory {
    /* L1 part: */
    std::list<std::size_t> T1;
    std::list<std::size_t> B1;

    /* L2 part: */
    std::list<std::size_t> T2;
    std::list<std::size_t> B2;

    std::size_t p = 0;
    std::size_t capacity = 0;

    cache_directory (std::size_t i_capacity) : capacity (i_capacity) {}
    void store_new_hash (std::size_t hash) { T1.push_front (hash); }

    std::size_t replace_p (bool B2_case = false);

    bool process_T_case (std::size_t hash);
    bool process_B1_case (std::size_t hash, std::size_t& to_remove);
    bool process_B2_case (std::size_t hash, std::size_t& to_remove);
    void process_nowhere_case (std::size_t& to_remove);

    std::size_t L1_size () const {return T1.size() + B1.size();}
    std::size_t L2_size () const {return T2.size() + B2.size();}

    void dump ();
};


std::size_t cache_directory::replace_p (bool B2_case) {
    if ((T1.size() >= 1) && ((B2_case && (T1.size() == p)) || T1.size() > p)) {
        std::size_t T1_lru = T1.back();
        T1.pop_back();
        B1.push_front (T1_lru);

        return T1_lru;
    }

    else {
        std::size_t T2_lru = T2.back();
        T2.pop_back();
        B2.push_front (T2_lru);

        return T2_lru;
    }
}

bool cache_directory::process_T_case (std::size_t hash) {  
    std::list<std::size_t>::iterator it;

    if ((it = std::find (T1.begin(), T1.end(), hash)) != T1.end()) {
        T1.erase (it);
        T2.push_front (hash);

        return true;
    }

    if ((it = std::find (T2.begin(), T2.end(), hash)) != T2.end()) {
        T2.erase (it);
        T2.push_front (hash);

        return true;
    }

    return false;
}

bool cache_directory::process_B1_case (std::size_t hash, std::size_t& to_remove) {
    std::list<std::size_t>::iterator it;
    if ((it = std::find (B1.begin(), B1.end(), hash)) != B1.end()) {
        p = std::min (capacity, p + std::max (B2.size()/B1.size(), static_cast<std::size_t> (1)));
        to_remove = replace_p ();

        B1.erase (it);
        T2.push_front (hash);

        return true;
    }

    return false;
}

bool cache_directory::process_B2_case (std::size_t hash, std::size_t& to_remove) {
    std::list<std::size_t>::iterator it;
    if ((it = std::find (B2.begin(), B2.end(), hash)) != B2.end()) {
        p = std::max (static_cast<std::size_t> (0), p - std::max (B1.size()/B2.size(), static_cast<std::size_t> (1)));
        to_remove = replace_p (true);

        B2.erase (it);
        T2.push_front (hash);

        return true;
    }

    return false;
}

void cache_directory::process_nowhere_case (std::size_t& to_remove) {
    if (L1_size () == capacity) {
        if (T1.size() < capacity) {
            B1.pop_back();
            to_remove = replace_p();
        }
        else {   
            to_remove = T1.back();
            T1.pop_back();
        }
    }

    else if (L1_size() + L2_size() >= capacity) {
        if (L1_size() + L2_size() == 2*capacity)
            B2.pop_back();
        
        to_remove = replace_p();
    }
}

void cache_directory::dump () {
    std::cout << "--------B1--------" << std::endl;
    std::for_each(B1.rbegin(), B1.rend(), 
        [](std::size_t hash) { std::cout << hash << std::endl; });
    std::cout << "--------T1--------" << std::endl;
    std::for_each(T1.rbegin(), T1.rend(), 
        [](std::size_t hash) { std::cout << hash << std::endl; });
    std::cout << "==================" << std::endl;
    std::for_each(T2.begin(), T2.end(), 
        [](std::size_t hash) { std::cout << hash << std::endl; });
    std::cout << "--------T2--------" << std::endl;
    std::for_each(B2.begin(), B2.end(), 
        [](std::size_t hash) { std::cout << hash << std::endl; });
    std::cout << "--------B2--------" << std::endl;
} 