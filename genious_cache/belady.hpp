#include <iostream>
#include <array>
#include <vector>

/* This is not cache memory actually. It`s just 
vector with hashes, which implements belady cache 
replacement algorithm. It's made to compare num 
of hits with arc cache.*/

class belady {
    std::vector<std::size_t> hash_vec;
    std::size_t capacity; 

    void replace_hash (std::size_t new_hash, std::size_t pos_of_old_hash);
    void place_hash (std::size_t new_hash);
    bool contain (std::size_t hash);
    std::size_t will_use_later (const std::vector<std::size_t>& i_hash_vec, std::size_t pos);

public:
    std::size_t get_capacity () {return capacity;}
    std::size_t get_size() {return hash_vec.size();}

    belady (std::size_t i_capacity) : capacity (i_capacity) {}

    std::size_t cache (const std::vector<std::size_t>& i_hash_vec);
    
    void dump ();
};

void belady::replace_hash (std::size_t new_hash, std::size_t pos_of_old_hash) { 
    hash_vec[pos_of_old_hash] = new_hash; 
} 

void belady::place_hash (std::size_t new_hash) { 
    hash_vec.push_back(new_hash); 
}

bool belady::contain (std::size_t x_hash) {
    for (std::size_t hash : hash_vec) {
        if (hash == x_hash)
            return true;
    }
    return false;
}

std::size_t belady::will_use_later (const std::vector<std::size_t>& i_hash_vec, std::size_t pos) {
    std::size_t most_useless = 0;
    std::size_t future_pos;
    std::size_t max_future_pos = pos + 1;

    for(std::size_t i = 0; i < capacity; i++) {
        future_pos = pos + 1;

        while (future_pos < i_hash_vec.size() && i_hash_vec[future_pos] != hash_vec[i]) {
            future_pos++;
        }

        if (max_future_pos < future_pos) {
            max_future_pos = future_pos;
            most_useless = i;
        }
    }

    return most_useless;
}

std::size_t belady::cache (const std::vector<std::size_t>& i_hash_vec) {
    std::size_t num_of_hits = 0;

    for (std::size_t pos = 0; pos < i_hash_vec.size(); pos++) {
        if (contain (i_hash_vec[pos]))
            num_of_hits++;

        else if (hash_vec.size() < capacity)
            place_hash (i_hash_vec[pos]);

        else
        {
            std::size_t replace_pos = will_use_later (i_hash_vec, pos);
            replace_hash (i_hash_vec[pos], replace_pos);
        }
    }

    return num_of_hits;
}

void belady::dump () {
    std::cout << "Belady's data: ";
    for (std::size_t hash : hash_vec) {std::cout << "[ " << hash << " ] ";}
    std::cout << std::endl;
}