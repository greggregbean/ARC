#include "arc.hpp"
#include "gtest/gtest.h"

TEST (arc_cache, strings_testing) {
    // Arange
    arc<std::string> test_cache (8);

    // Act
    test_cache.cache ("1");
    test_cache.cache ("1");
    

    // Assert
    
}
