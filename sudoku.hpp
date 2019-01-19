
#pragma once

// uncomment to disable assert()
//#define NDEBUG
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <random>
#include <unordered_set>
#include <vector>

using namespace std;
namespace sudoku {

using ValuesMap = std::map<std::string, std::string>;

class Sudoku {
public:
    constexpr static int SIZE = 9;
    constexpr static int BLOCK_SIZE = 3; // sqrt(SIZE)

    Sudoku() = default;
    Sudoku(const string& grid) { values = init_grid(grid); }
    void unit_test();
    unsigned long get_steps() { return steps; }
    void set_steps(unsigned long val) { steps = val; }
    void display();
    bool solve(const string& grid);
    bool is_solved();
    string random_puzzle(unsigned n=17);

private:
    const string digits = "123456789";
    const string rows   = "ABCDEFGHI";
    const string cols   = digits;
    const vector<string> squares = cross(rows, cols);
    const vector<vector<string>> unit_list = make_unit_list(rows, cols);
    map<string, vector<vector<string>>> units = init_units(unit_list);
    map<string, unordered_set<string>> peers = init_peers(units);

    unsigned long steps = 0;
    ValuesMap values = init_values();

    vector<string> cross(const string& row, const string& col);
    vector<string> cross(const string& row, const char c);
    vector<string> cross(const char r, const string& col);
    vector<vector<string>> make_unit_list(const string& row, const string& col);
    map<string, vector<vector<string>>> init_units(const vector<vector<string>>& unit_list);
    map<string, unordered_set<string>> init_peers(map<string, vector<vector<string>>>& units);
    ValuesMap init_values();
    ValuesMap init_grid(const string& grid);
    bool parse_grid(ValuesMap& values, const string& grid);
    bool assign(ValuesMap& values, const string& s, const string& d);
    bool eliminate(ValuesMap& values, const string& s, const string& d);
    bool is_unit_solved(const vector<string>& unit);
    bool search(ValuesMap& values);
};

void replace(string& str, const string& from, const string& to);
string join(const vector<string>& v, char c);
string center(const string& text, const size_t width);

} // namespace 

