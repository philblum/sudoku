#include "sudoku.hpp"
#include <iostream>

using namespace std;
namespace sudoku {

    map<string, string> Sudoku::init_values()
    {
      map<string, string> v;
      for(const auto s: squares)
      {
        string d = digits;
        random_shuffle(d.begin(), d.end());
        v[s] = d;
      }
      return v;
    }

    vector<string> Sudoku::cross(const string& row, const string& col)
    {
      vector<string> s;
      for(const auto r : row)
        for(const auto c : col)
        {
	  string t = {r, c};
	  s.push_back(t);
        }	  
      return s;
    }

    vector<string> Sudoku::cross(const string& row, const char c)
    {
      vector<string> s;
      for(const auto r : row)
      {
	string t = {r, c};
	s.push_back(t);
      }	  
      return s;
    }

    vector<string> Sudoku::cross(const char r, const string& col)
    {
      vector<string> s;
      for(const auto c : col)
      {
	string t = {r, c};
	s.push_back(t);
      }	  
      return s;
    }

    vector<vector<string>> Sudoku::make_unit_list(const string& row, const string& col)
    {
      vector<vector<string>> units;
      for(const auto c: cols)
      {
         const vector<string> u = cross(rows, c);
         units.push_back(u);
      }

      for(const auto r: rows)
      {
         const vector<string> u = cross(r, cols);
         units.push_back(u);
      }

      const vector<string> rows = {"ABC", "DEF", "GHI"};
      for(const auto r: rows)
      {
        const vector<string> cols = {"123", "456", "789"};
        for(const auto c: cols)
        {
          const vector<string> u = cross(r, c);
          units.push_back(u);
        }
      }
      return units;
      
    }

    map<string, vector<vector<string>>>
    Sudoku::init_units(const vector<vector<string>>& unit_list)
    {
      map<string, vector<vector<string>>> units;
      for(const auto s: squares)
      {
        vector<vector<string>> u_list;
        for(const auto unit: unit_list) // unit is a vector<string>
        {
          for(const auto u: unit)       // u is a string
            if (u.find(s) != std::string::npos)
              u_list.push_back(unit);
	}
        units[s] = u_list;
      }
      return units;
    }

    map<string, unordered_set<string>>
    Sudoku::init_peers(map<string, vector<vector<string>>>& units)
    {
      map<string, unordered_set<string>> peers;
      for(const auto s: squares)
      {
        vector<string> list;
        for(const auto &unit: units[s])
          for(const auto u: unit)
	    list.push_back(u);

        unordered_set<string> uos(begin(list), end(list));
        uos.erase(s);
        peers[s] = uos;
      }
      return peers;
    }

    void Sudoku::unit_test()
    {
        assert(squares.size() == SIZE * SIZE);
        assert(unit_list.size() == BLOCK_SIZE * SIZE);

        for (const auto &s : squares)
        {
	  assert(units[s].size() == 3);
          assert(peers[s].size() == 20);
        }

        const vector<vector<string>> units_c2 =
        {
                {"A2", "B2", "C2", "D2", "E2", "F2", "G2", "H2", "I2"},
                {"C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9"},
                {"A1", "A2", "A3", "B1", "B2", "B3", "C1", "C2", "C3"}
        };
        assert(units["C2"] == units_c2);

        const unordered_set<string> peers_c2 = {
	        "A2", "B2", "D2", "E2", "F2", "G2", "H2", "I2",
                "C1", "C3", "C4", "C5", "C6", "C7", "C8", "C9",
                "A1", "A3", "B1", "B3" };
        assert(peers["C2"] == peers_c2);

        std::cout << "All tests pass" << std::endl;
    }

    ValuesMap Sudoku::init_grid(const string& grid)
    {
      assert(squares.size() == grid.size());

      ValuesMap grid_values;
      for (unsigned int i = 0; i < grid.size(); i++)
      {
        char c = grid[i];
	if ((digits.find(c) != std::string::npos) ||
           c == '0' || c == '.')
        {
          string s = {c};
	  grid_values[squares[i]] = s;
        }
      }
      assert(grid_values.size() == SIZE * SIZE);
      return grid_values;
    }

    bool Sudoku::parse_grid(ValuesMap& values, const string& grid)
    { 
      for(const auto s: squares)
        values[s] = digits;

      const auto grid_values = init_grid(grid);
      for(const auto item: grid_values)
      {
	const auto s = item.first;
        const auto d = item.second;
	if ((digits.find(d) != std::string::npos) and
	    not assign(values, s, d))
	  return false; // fail is we can't assign d to square s
      }
      return true;
    }

    bool Sudoku::assign(ValuesMap& values, const string& s, const string& d)
    {
      auto other_values = values[s];
      replace(other_values, d, "");

      for(const auto d2: other_values)
      {
	string dd = {d2};
	if(eliminate(values, s, dd) == false)
	  return false;
      }
      return true;
    }

    bool Sudoku::eliminate(ValuesMap& values, const string& s, const string& d)
    {
      steps++;
      if(values[s].find(d) == string::npos)
	return true; // d already eliminated

      replace(values[s], d, "");
      // (1) If a square is reduced to one value d2,
      // then eliminate d2 from the peers
      if(values[s].size() == 0)
	return false;  // contradiction; eliminated last possibility
      else if(values[s].size() == 1)
      {
	const auto dd = values[s];
	for(const auto ss: peers[s])
	  if(eliminate(values, ss, dd) == false)
	    return false;
      }
      // (2) If a unit is reduced to only one place for a value d,
      // then put it there
      for(const auto& unit : units[s])
      {
        vector<string> dplaces;
	for(const auto s: unit)
	  if(values[s].find(d) != string::npos)
	    dplaces.push_back(s);
	if(dplaces.size() == 0)
	  return false; // contradiction; no place for this value
        else if (dplaces.size() == 1)
          // d can only be one place in unit; assign it there
          if(assign(values, dplaces[0], d) == false)
	    return false;
      }
      return true;
    }

    bool Sudoku::search(ValuesMap& values)
    {
      size_t max_size = 1;
      size_t min_size = SIZE;
      string min_s = {};
      
      for(const auto s: squares)
	max_size = max(max_size, values[s].size());

      if(max_size == 1) //solved!
        return true;

      // Find the square with the fewest possibilities
      for(const auto s: squares)
        if(values[s].size() > 1)
          if (values[s].size() < min_size)
          {
            min_size = values[s].size();
            min_s = s;
          }

      bool status = false;
      for(const auto &d: values[min_s])
      {
        string dd = {d};
        auto values_copy(values);
	if(assign(values_copy, min_s, dd) == false)
        {
          status = false;
	  continue;
        }
        if(search(values_copy) == true)
        {
	  status = true;
	  values = values_copy;
          break;
        }
      }
      return status;
    }

    bool Sudoku::solve(const string& grid)
    {
      if(parse_grid(values, grid) == true)
        return search(values);
      else
	return false;
    }

    /* A unit is solved when its values are permutation of the
     * digits 1 to 9. */
    bool Sudoku::is_unit_solved(const vector<string>& unit)
    {
      static const char *str = digits.c_str();
      static const unordered_set<char> dd(str,str+strlen(str));

      unordered_set<char> uu;
      for(const auto &s: unit)
	for(const auto &d: values[s])
          uu.emplace(d);

      if(uu != dd)
        return false;

      return true;
    }

    /* A puzzle is solved when all of its units are solved. */
    bool Sudoku::is_solved()
    {
      for(const auto s: squares)
	if(values[s].size() != 1)
	  return false;
      
      // check all 27 units are solved (9 rows, 9 cols, 9 squares)
      for(const auto& unit: unit_list)
      {
	if(is_unit_solved(unit) == false)
	  return false;
      }
      return true;
    }

    void replace(string& str, const string& from, const string& to)
    {
      if(from.empty())
        return;
      size_t pos = 0;
      while((pos = str.find(from, pos)) != std::string::npos)
      {
        str.replace(pos, from.length(), to);
        pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
      }
    }

    string join(const vector<string>& v, char c)
    {
      string s;
      for (vector<string>::const_iterator p = v.cbegin(); p != v.cend(); ++p)
      {
        s += *p;
        if (p != v.end() - 1)
          s += c;
      }
      return s;
    }

    string center(const string& text, const size_t width)
    {
      size_t len = text.length();
      if(width < len)
	return text;

      size_t diff = width - len;
      size_t left = diff / 2;
      size_t right = diff - left;
      return string(left, ' ') + text + string(right, ' ');
    }

    void Sudoku::display()
    {
      size_t currentMax = 0;
      for(const auto s: squares)
	currentMax = std::max(currentMax, values[s].size());
      size_t width = 2 + currentMax;
      string dash (width*3,'-');
      vector<string> dashes = {dash, dash, dash};
      string dline = join(dashes, '+');

      for(const auto &r: rows)
      {
	string line;
        for(const auto &c: cols)
        {
	  string s = {r,c};
	  line += center(values[s], width);
	  if (c == '3' || c == '6')
	    line += '|';
        }
	std::cout << line << std::endl;
        if (r == 'C' || r == 'F')
          std::cout << dline << std::endl;
      }
      std::cout << std::endl;
    }

    string Sudoku::random_puzzle(unsigned n)
    {
      /* Make a random puzzle with n or more assignments. Restart on contradictions.
       * Note the resulting puzzle is not guaranteed to be solvable, but empirically
       * about 99.8% of them are solvable. Some have multiple solutions. */
      ValuesMap values;
      for(const auto s: squares)
        values[s] = digits;

      std::random_device rd;
      std::mt19937 g(rd());

      vector<string> shuffled_squares(squares);
      shuffle(shuffled_squares.begin(), shuffled_squares.end(), g);

      // Setup uniform random selection of digits
      std::uniform_int_distribution<int> dist(0, digits.size() - 1);

      string grid;
      for(const auto s: shuffled_squares)
      {
        string d = {digits[dist(g)]};
        if(not assign(values, s, d))
          break; // fail; we can't assign d to square s

        int num_sq = 0;
        std::unordered_set<char> ds;
        for(const auto s: squares)
          if(values[s].size() == 1)
          {
            num_sq++;
            ds.insert(values[s].at(0));
          }

        if(num_sq >= n and ds.size() >= 8)
        {
          for(const auto s: squares)
            if(values[s].size() == 1)
              grid += values[s];
            else
              grid += '.';
          return grid;
         }
      }
      return random_puzzle(n); // fail; start over; make a new puzzle
    }
} // namespace sudoku

