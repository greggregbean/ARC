#include <algorithm>
#include <iostream>
#include <string>
#include <list>
#include <unordered_map>

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

    /* L2 part: */
    std::list<std::size_t> T2;
    std::list<std::size_t> B2;
};

template <typename T>
class arc final
{
    std::unordered_map<std::size_t, T> memory;
    cache_directory cache_dir;
    std::size_t capacity;
    std::size_t pivot;
    std::hash<T> hash;

    void process_T_case (std::list<std::size_t>::iterator pos) {
        std::cout << "T" << std::endl;
    }

    void process_B1_case (std::list<std::size_t>::iterator pos) {
        std::cout << "B1" << std::endl;
    }

    void process_B2_case (std::list<std::size_t>::iterator pos) {
        std::cout << "B2" << std::endl;
    }

    void process_nowhere_case () {
        std::cout << "nowhere" << std::endl;
    }

public:
    arc (std::size_t i_capacity) : 
        capacity (i_capacity), pivot (i_capacity/2) 
    {
        memory.reserve (capacity);
    }

    void cache (T el)
    {
        std::size_t el_hash = hash (el);
        std::list<std::size_t>::iterator pos;

        if ((pos = std::find (cache_dir.T1.begin(), cache_dir.T1.end(), el_hash)) != cache_dir.T1.end())
            process_T_case (pos);
        
        else if ((pos = std::find (cache_dir.T2.begin(), cache_dir.T2.end(), el_hash)) != cache_dir.T2.end())
            process_T_case (pos);
        
        else if ((pos = std::find (cache_dir.B1.begin(), cache_dir.B1.end(), el_hash)) != cache_dir.B1.end())
            process_B1_case (pos);
        
        else if ((pos = std::find (cache_dir.B2.begin(), cache_dir.B2.end(), el_hash)) != cache_dir.B2.end())
            process_B2_case (pos);
        
        else    
            process_nowhere_case ();   
    }

};
