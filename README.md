
# C++ Sudoku

Please refer to Peter Norvig's article http://norvig.com/sudoku.html.
I created this software as an excercise to learn C++ where I didn't 
have to think about software design and could focus on translating
Peter's beautiful python code into C++ in the best way possible.

Enjoy!

### Compile

The code is compiled with g++ version 5.4.0 following the C++11 standard.

```sh
$ g++ -std=c++11 -O3 sudoku.cpp main.cpp -o sudoku
```

### Run

My machine is an Intel® Core™ i7-4800MQ CPU @ 2.7 GHz × 8 with 32 GB RAM.
The times below are for a single CPU running at or near 100% utilization.

```sh
$ ./sudoku
Sudoku version 1.7
All tests pass
Solved 50 of 50 easy50 puzzles in 0.099 seconds [avg: 0.002 sec (504.127 Hz), max: 0.006 sec]
Solved 95 of 95 top95 puzzles in 0.588 seconds [avg: 0.006 sec (161.613 Hz), max: 0.030 sec]
Solved 11 of 11 hardest puzzles in 0.027 seconds [avg: 0.002 sec (401.581 Hz), max: 0.003 sec]
Solved 49151 of 49151 sudoku17 puzzles in 106.110 seconds [avg: 0.002 sec (463.207 Hz), max: 0.171 sec]
Solved 100 of 100 random puzzles in 0.226 seconds [avg: 0.002 sec (443.183 Hz), max: 0.003 sec]
Solved 1 of 1 hard1 puzzles in 31.952 seconds [avg: 31.952 sec (0.031 Hz), max: 31.952 sec]
Solved 0 of 1 impossible1 puzzles in 273.546 seconds [avg: 273.546 sec (0.004 Hz), max: 273.546 sec]
```

### Profile/Improve

The implementation has not been profiled so there is likely room for performance improvements.
