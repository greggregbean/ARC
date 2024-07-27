# arc

## Clonnig the project
Use `git clone --recursive` to download the project and its gtest git submodule. Otherwise from the project root repository you need to download the submodule with `git submodule update --init`.

## Building the project
````
cmake -B build
cmake --build build
````

## Runing executable
````
./build/run
````
## Running tests
````
./build/run_unit_tests
````