#include "sudoku.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace sudoku;

void unit_test()
{
    Sudoku puzzle;
    puzzle.unit_test();
}

void display(const string grid)
{
    auto S = Sudoku(grid);
    S.display();
}

vector<string> random_puzzles(unsigned n)
{
    vector<string> grids;
    auto S = Sudoku();
    for(int k=0; k < n; k++)
      grids.push_back(S.random_puzzle());
    return grids;
}

vector<string> from_file(string filename)
{
    vector<string> grids;
    string grid;
    ifstream file(filename);
    if (file.is_open())
    {
      while (getline(file, grid))
      {
        grid.resize(Sudoku::SIZE * Sudoku::SIZE);
        grids.push_back(grid);
      }
      file.close();
    }
    else cout << "Unable to open file";
    return grids;
}

void solve_all(const vector<string> grids,
        const string filename, const bool print_all=false,
        const double display_if=1.0)
{
    Sudoku puzzle;
    unsigned solved_count = 0;
    unsigned total_steps = 0;
    std::chrono::duration<double> total_time(0.0);
    std::chrono::duration<double> max_time(0.0);

    for(unsigned i = 0; i < grids.size(); i++)
    {
      const auto grid = grids[i];
      auto start_steps = puzzle.get_steps();
      auto tic = std::chrono::steady_clock::now();
      auto ans = puzzle.solve(grid);
      auto toc = std::chrono::steady_clock::now();
      std::chrono::duration<double> dt = toc - tic;
      total_time += dt;
      max_time = std::max(max_time, dt);
      auto steps = puzzle.get_steps() - start_steps;
      // Print puzzle run times, etc.
      if(print_all)
      {
        string result = " incorrectly solved in ";
        if(ans && puzzle.is_solved())
          result = " correctly solved in ";
        std::cout << "Puzzle " << setw(2) << i+1 << result << setw(9)
                  << steps << " steps, " << fixed << setprecision(3) << setw(6)
                  << (float)dt.count() << " sec" << std::endl;
      }
      //Show puzzles that take long enough
      if(dt.count() > display_if)
      {
        std::cout << filename << ": puzzle " << i+1 << std::endl;
        display(grid);
        puzzle.display();
      }
      if(ans && puzzle.is_solved())
	solved_count++;
    }
    total_steps = puzzle.get_steps();
    auto n = grids.size();
    double avg_duration = (double)total_time.count() / (double)n;
    std::cout << "Solved " << solved_count << " of " << n << " " << filename
              << " puzzles in " << fixed << setprecision(3) << (double)total_time.count() 
              << " seconds [avg: " << setprecision(3) << avg_duration
              << " sec (" << fixed << setprecision(3) << 1.0/avg_duration
              << " Hz), max: " << fixed << setprecision(3) << (double)max_time.count()
              << " sec]" << std::endl;
}

int main()
{
    std::cout << "Sudoku version 1.7\n";
    unit_test();

    vector<string> hard1 = {".....6....59.....82....8....45........3........6..3.54...325..6.................."};
    vector<string> impossible1 = {".....5.8....6.1.43..........1.5........1.6...3.......553.....61........4........."};

    solve_all(from_file("easy50.txt"), "easy50", false, 1.0);
    solve_all(from_file("top95.txt"), "top95", false, 1.0);
    solve_all(from_file("hardest.txt"), "hardest", false, 1.0);
    solve_all(from_file("sudoku17.txt"), "sudoku17", false, 1.0);
    solve_all(random_puzzles(100), "random", false, 1.0);
    solve_all(hard1, "hard1", false, 1.0);
    solve_all(impossible1, "impossible1", false, 1.0);
    return 0;
}
