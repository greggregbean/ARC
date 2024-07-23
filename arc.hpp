#include <algorithm>
#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>

#define DEBUG 1

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

template <typename T>
class arc final
{
    std::unordered_map<std::size_t, T> memory;
    cache_directory cache_dir;
    std::size_t capacity = 0;
    std::size_t pivot = 0;
    std::hash<T> hash;

    void replace_pivot () {
        #if DEBUG
            std::cout << "replace pivot" << std::endl;
        #endif
    }

    void process_T_case (std::list<std::size_t>::iterator pos) {
        #if DEBUG
            std::cout << "T case" << std::endl;
        #endif
    }

    void process_B1_case (std::list<std::size_t>::iterator pos) {
        #if DEBUG
            std::cout << "B1 case" << std::endl;
        #endif
    }

    void process_B2_case (std::list<std::size_t>::iterator pos) {
        #if DEBUG
            std::cout << "B2 case" << std::endl;
        #endif
    }

    void process_nowhere_case (const T& x, std::size_t x_hash) {
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
                replace_pivot();
            }

            else
            {
                cache_dir.T1.pop_back();
                memory.erase(x_hash);
            }
        }

        else if (cache_dir.L1_size() + cache_dir.L2_size() >= capacity)
        {
            #if DEBUG
                std::cout << "case ii" << std::endl;
            #endif

            if (cache_dir.L1_size() + cache_dir.L2_size() == 2*capacity)
                cache_dir.B2.pop_back();
            
            replace_pivot();
        }

        cache_dir.T1.push_front (x_hash);
        memory [x_hash] = x;

        #if DEBUG
            assert (cache_dir.L1_size() <= capacity);
            assert (cache_dir.L1_size() + cache_dir.L2_size() <= capacity);
        #endif
    }

    

public:
    arc (std::size_t i_capacity) : capacity (i_capacity) {}

    void cache (T x)
    {
        std::size_t x_hash = hash (x);
        std::list<std::size_t>::iterator pos;

        if ((pos = std::find (cache_dir.T1.begin(), cache_dir.T1.end(), x_hash)) != cache_dir.T1.end())
            process_T_case (pos);
        
        else if ((pos = std::find (cache_dir.T2.begin(), cache_dir.T2.end(), x_hash)) != cache_dir.T2.end())
            process_T_case (pos);
        
        else if ((pos = std::find (cache_dir.B1.begin(), cache_dir.B1.end(), x_hash)) != cache_dir.B1.end())
            process_B1_case (pos);
        
        else if ((pos = std::find (cache_dir.B2.begin(), cache_dir.B2.end(), x_hash)) != cache_dir.B2.end())
            process_B2_case (pos);
        
        else    
            process_nowhere_case (x, x_hash);   
    }

};
