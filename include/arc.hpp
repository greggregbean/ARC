#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>

#define DEBUG 0

/* 
    Illustration of ARC cache memory model:

        [6 5 4 3 2 1 0[6 5 4 3 2 1 0|0 1 2 3 4 5 6]0 1 2 3 4 5 6]
        [     B1    <-[     T1    <-|->      T2   ]->    B2     ] 
        [ . . . . . . [ . . . . . . | . .^. . . . ] . . . . . . ]
                                        (p)
                      [<-------(capacity)-------->]
*/


struct cache_directory
{
    /* L1 part: */
    std::list<std::size_t> T1;
    std::list<std::size_t> B1;

    std::size_t L1_size () {
        return T1.size() + B1.size();
    }

    /* L2 part: */
    std::list<std::size_t> T2;
    std::list<std::size_t> B2;

    std::size_t L2_size () {
        return T2.size() + B2.size();
    }
};

/* CACHE_DIR stores only hashes of pages.
   Pages themselves are stored in MEMORY. */
template <typename T>
class arc final
{
    std::unordered_map<std::size_t, T> memory;
    cache_directory cache_dir;
    std::size_t capacity = 0;
    std::size_t p = 0;
    std::hash<T> hash;

    void place_in_memory (const T& x, std::size_t x_hash);
    void remove_from_memory (std::size_t x_hash);

    void replace_p (bool B2_case = false);

    void process_T1_case (std::list<std::size_t>::iterator it);
    void process_T2_case (std::list<std::size_t>::iterator it);
    void process_B1_case (std::list<std::size_t>::iterator it, const T& x);
    void process_B2_case (std::list<std::size_t>::iterator it, const T& x);
    void process_nowhere_case (const T& x, std::size_t x_hash);

public:
    arc (std::size_t i_capacity = 10) : capacity (i_capacity) {
        memory.reserve(i_capacity);
    }
    bool cache (const T& x);
};

template <typename T>
inline void arc<T>::place_in_memory(const T &x, std::size_t x_hash) {
    memory [x_hash] = x;
}

template <typename T>
inline void arc<T>::remove_from_memory (std::size_t hash) {
    memory.extract (hash);
}

template <typename T>
void arc<T>::replace_p (bool B2_case) {
    #if DEBUG
        std::cout << "replace p" << std::endl;
    #endif

    if ((cache_dir.T1.size() >= 1) &&
        ((B2_case && (cache_dir.T1.size() == p)) || cache_dir.T1.size() > p))
    {
        std::size_t T1_lru = cache_dir.T1.back();
        cache_dir.T1.pop_back();
        cache_dir.B1.push_front (T1_lru);

        remove_from_memory (T1_lru);
    }

    else
    {
        std::size_t T2_lru = cache_dir.T2.back();
        cache_dir.T2.pop_back();
        cache_dir.B2.push_front (T2_lru);

        remove_from_memory (T2_lru);
    }
}

template <typename T>
void arc<T>::process_T1_case (std::list<std::size_t>::iterator it) {
    #if DEBUG
        std::cout << "T1 case" << std::endl;
    #endif

    std::size_t hash = *it;
    cache_dir.T1.erase (it);

    cache_dir.T2.push_front (hash);
}

template <typename T>
void arc<T>::process_T2_case (std::list<std::size_t>::iterator it) {
    #if DEBUG
        std::cout << "T2 case" << std::endl;
    #endif

    std::size_t x_hash = *it; 
    cache_dir.T2.erase (it);

    cache_dir.T2.push_front (x_hash);
}

template <typename T>
void arc<T>::process_B1_case (std::list<std::size_t>::iterator it, const T& x) {
    #if DEBUG
        std::cout << "B1 case" << std::endl;
    #endif

    p = std::min (capacity, 
        p + std::max (cache_dir.B2.size()/cache_dir.B1.size(), static_cast<std::size_t> (1)));
    replace_p ();

    std::size_t x_hash = *it;
    cache_dir.B1.erase (it);

    cache_dir.T2.push_front (x_hash);

    place_in_memory (x, x_hash);
}

template <typename T>
void arc<T>::process_B2_case (std::list<std::size_t>::iterator it, const T& x) {
    #if DEBUG
        std::cout << "B2 case" << std::endl;
    #endif

    p = std::max (static_cast<std::size_t> (0), 
        p - std::max (cache_dir.B1.size()/cache_dir.B2.size(), static_cast<std::size_t> (1)));
    replace_p (true);

    std::size_t x_hash = *it;
    cache_dir.B2.erase (it);

    cache_dir.T2.push_front (x_hash);

    place_in_memory (x, x_hash);
}

template <typename T>
void arc<T>::process_nowhere_case (const T &x, std::size_t x_hash)
{
    #if DEBUG
        std::cout << "nowhere case" << std::endl;
    #endif

    if (cache_dir.L1_size() == capacity)
    {
        #if DEBUG
            std::cout << "case i" << std::endl;
        #endif

        if (cache_dir.T1.size() < capacity)
        {
            cache_dir.B1.pop_back();
            replace_p();
        }

        else
        {
            cache_dir.T1.pop_back();
            remove_from_memory(x_hash);
        }
    }

    else if ((cache_dir.L1_size() < capacity) &&
        (cache_dir.L1_size() + cache_dir.L2_size() >= capacity))
    {
        #if DEBUG
            std::cout << "case ii" << std::endl;
        #endif

        if (cache_dir.L1_size() + cache_dir.L2_size() == 2*capacity)
            cache_dir.B2.pop_back();
        
        replace_p();
    }

    cache_dir.T1.push_front (x_hash);
    place_in_memory (x, x_hash);
}

template <typename T>
bool arc<T>::cache (const T& x)
{
    bool already_in_memory = false;
    std::size_t x_hash = hash (x);
    std::list<std::size_t>::iterator it;

    if ((it = std::find (cache_dir.T1.begin(), cache_dir.T1.end(), x_hash)) != cache_dir.T1.end())
    {
        already_in_memory = true;
        process_T1_case (it);
    }
        
    
    else if ((it = std::find (cache_dir.T2.begin(), cache_dir.T2.end(), x_hash)) != cache_dir.T2.end())
    {
        already_in_memory = true;
        process_T2_case (it); 
    }
    
    else if ((it = std::find (cache_dir.B1.begin(), cache_dir.B1.end(), x_hash)) != cache_dir.B1.end())
        process_B1_case (it, x);
    
    else if ((it = std::find (cache_dir.B2.begin(), cache_dir.B2.end(), x_hash)) != cache_dir.B2.end())
        process_B2_case (it, x);
    
    else    
        process_nowhere_case (x, x_hash);   
    
    #if DEBUG
        assert (cache_dir.L1_size() + cache_dir.L2_size() <= 2*capacity);
    #endif

    return already_in_memory;
}
