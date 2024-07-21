#include "arc.hpp"

int main () 
{
    arc<std::string> my_cache (3);
    my_cache.cache ("cringe");
    my_cache.cache ("zalupa");
}