# nba

C++ non-blocking algorithms collection

#### Project directory structure:
````
  .
  |-- build
  |-- test
  |-- src
      |-- algorithms
          |-- include
          └-- src
      └-- memory
          |-- include
          └-- src  
````

#### Dev Dependencies
lcov  


#### Test
```
# Build make files
cmake .
# Build executable targets and libs
make
# Run all registred tests in super verbose mode
ctest -VV
```
