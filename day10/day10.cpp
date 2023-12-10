#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <vector>
#include <regex>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
using namespace std;

enum Direction
{
    Up,
    Down,
    Left,
    Right,
};

class Point : public pair<int, int> {
   public:
   Point() {};
   Point(int first, int second) : pair{first, second}{};
   class Hash {
      public:
      auto operator()(const Point &p) const -> size_t {
         return ((size_t)p.first) << 32 | ((size_t)p.second);
      }
   };
 };

struct Maze
{
    unordered_set<Point, Point::Hash> cellsInMaze;
    const vector<string> lines;

    Maze(const vector<string> &lines) : lines(lines), cellsInMaze() {}

    bool checkCellInMaze(const Point &loc) const
    {
        return cellsInMaze.find(loc) != cellsInMaze.end();
    }

    bool canMove(const Point &loc, const Direction &dir) const
    {
        char nextCell;
        switch (dir)
        {
        case Down:
            if (loc.first >= (lines.size() - 1))
            {
                return false;
            }
            nextCell = lines.at(loc.first + 1).at(loc.second);
            return nextCell == '|' || nextCell == 'J' || nextCell == 'L';
        case Up:
            if (loc.first < 1)
            {
                return false;
            }
            nextCell = lines.at(loc.first - 1).at(loc.second);
            return nextCell == '|' || nextCell == 'F' || nextCell == '7';
        case Left:
            if (loc.second < 1)
            {
                return false;
            }
            nextCell = lines.at(loc.first).at(loc.second - 1);
            return nextCell == '-' || nextCell == 'F' || nextCell == 'L';
        case Right:
            if (loc.second >= (lines.size() - 1))
            {
                return false;
            }
            nextCell = lines.at(loc.first).at(loc.second + 1);
            return nextCell == '-' || nextCell == 'J' || nextCell == '7';
        }
        assert(false);
    }
};

class MazeRunner
{
    Point position;

public:
    const Point startPosition;
    Maze maze;

    MazeRunner(const Point &startPosition, const Maze &maze) : startPosition(startPosition), maze(maze)
    {
        position = startPosition;
    }

    char move(const Direction &dir)
    {
        switch (dir)
        {
        case Up:
            position.first -= 1;
            break;
        case Down:
            position.first += 1;
            break;
        case Left:
            position.second -= 1;
            break;
        case Right:
            position.second += 1;
            break;
        }
        char result = maze.lines.at(position.first).at(position.second);
        maze.cellsInMaze.insert(position);
        return result;
    }

    int traverseMaze()
    {
        const vector<Direction> initialDirections = {Up, Down, Left, Right};

        int numInitialMoves = 0;
        Direction direction;
        for (Direction tmp : initialDirections)
        {
            if (maze.canMove(startPosition, tmp))
            {
                direction = tmp;
                numInitialMoves += 1;
            }
        }
        assert(numInitialMoves == 2);

        int stepsCount = 0;
        char cell = ' ';
        while (cell != 'S')
        {
            cell = move(direction);
            stepsCount++;
            switch (cell)
            {
            case 'S':
                break;
            case '-':
                break;
            case '|':
                break;
            case 'F':
                if (direction == Up)
                {
                    direction = Right;
                }
                else if (direction == Left)
                {
                    direction = Down;
                }
                else
                {
                    assert(false);
                }
                break;
            case '7':
                if (direction == Up)
                {
                    direction = Left;
                }
                else if (direction == Right)
                {
                    direction = Down;
                }
                else
                {
                    assert(false);
                }
                break;
            case 'L':
                if (direction == Down)
                {
                    direction = Right;
                }
                else if (direction == Left)
                {
                    direction = Up;
                }
                else
                {
                    assert(false);
                }
                break;
            case 'J':
                if (direction == Down)
                {
                    direction = Left;
                }
                else if (direction == Right)
                {
                    direction = Up;
                }
                else
                {
                    assert(false);
                }
                break;
            default:
                assert(false);
            }
        }
        return stepsCount;
    }
};

regex pattern("S");

MazeRunner initializeMaze(const string &filename)
{
    ifstream inputFile(filename);
    assert(inputFile.is_open());
    string line;
    vector<string> mazeLines;
    smatch match;
    int col = 0;
    Point startLocation(0, 0);
    while (getline(inputFile, line))
    {
        mazeLines.push_back(line);
        if (regex_search(line, match, pattern))
        {
            startLocation.first = col;
            startLocation.second = match.position();
        }
        col++;
    }
    Maze maze(mazeLines);
    return MazeRunner(startLocation, maze);
}

int main()
{
    string filename = "input.txt";
    MazeRunner runner = initializeMaze(filename);
    int stepsCount = runner.traverseMaze();
    assert((stepsCount % 2) == 0);
    cout << stepsCount / 2 << endl;
    return 0;
}
