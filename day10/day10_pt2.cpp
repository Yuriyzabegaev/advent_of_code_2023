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
#include <queue>
using namespace std;

enum Direction
{
    Up,
    Down,
    Left,
    Right,
};

const vector<Direction> ALL_DIRECTIONS = {Left, Right, Up, Down};

class Point : public pair<int, int>
{
public:
    Point(){};
    Point(int first, int second) : pair{first, second} {};
    Point(const Point &other)
    {
        first = other.first;
        second = other.second;
    };

    Point moved(const Direction &dir) const
    {
        switch (dir)
        {
        case Down:
            return Point(first + 1, second);
        case Up:
            return Point(first - 1, second);
        case Left:
            return Point(first, second - 1);
        case Right:
            return Point(first, second + 1);
        }
        assert(false);
    }

    class Hash
    {
    public:
        auto operator()(const Point &p) const -> size_t
        {
            return ((size_t)p.first) << 32 | ((size_t)p.second);
        }
    };
};

struct Maze
{
    unordered_set<Point, Point::Hash> cellsInMaze;
    vector<string> lines;

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

    void visualize(const Point &loc) const
    {
        for (int i = 0; i < lines.size(); ++i)
        {
            string line = lines[i];
            if (i == loc.first)
            {
                line = string(line);
                line[loc.second] = '@';
            }
            cout << line << '\n';
        }
        cout << endl;
    }
};

class MazeRunner
{
    Point position;

public:
    const Point startPosition;
    Maze maze;

    MazeRunner(const Point startPosition, const Maze &maze) : startPosition(startPosition), maze(maze)
    {
        position = startPosition;
        this->maze.cellsInMaze.insert(position);
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
        unordered_set<Direction> initialMoves;
        for (Direction tmp : ALL_DIRECTIONS)
        {
            if (maze.canMove(startPosition, tmp))
            {
                initialMoves.insert(tmp);
            }
        }
        assert(initialMoves.size() == 2);

        int stepsCount = 0;
        char cell = ' ';
        Direction direction = *initialMoves.begin();
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
        if ((initialMoves.find(Up) != initialMoves.end()) && (initialMoves.find(Down) != initialMoves.end()))
        {
            maze.lines[startPosition.first][startPosition.second] = '|';
        }
        else if ((initialMoves.find(Up) != initialMoves.end()) && (initialMoves.find(Left) != initialMoves.end()))
        {
            maze.lines[startPosition.first][startPosition.second] = 'J';
        }
        else if ((initialMoves.find(Up) != initialMoves.end()) && (initialMoves.find(Right) != initialMoves.end()))
        {
            maze.lines[startPosition.first][startPosition.second] = 'L';
        }
        else if ((initialMoves.find(Down) != initialMoves.end()) && (initialMoves.find(Left) != initialMoves.end()))
        {
            maze.lines[startPosition.first][startPosition.second] = '7';
        }
        else if ((initialMoves.find(Down) != initialMoves.end()) && (initialMoves.find(Right) != initialMoves.end()))
        {
            maze.lines[startPosition.first][startPosition.second] = 'F';
        }
        else if ((initialMoves.find(Left) != initialMoves.end()) && (initialMoves.find(Right) != initialMoves.end()))
        {
            maze.lines[startPosition.first][startPosition.second] = '-';
        }
        else
        {
            assert(false);
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

struct OffMazeCluster
{
    unordered_set<Point, Point::Hash> visitedCells;
    const Point startLocation;
    const Maze maze;

    OffMazeCluster(const Point &startLocation, const Maze &maze) : startLocation(startLocation), maze(maze), visitedCells() {}

    bool isInside() const
    {
        assert(clusterTypeFound);
        return clusterInside;
    }

    int explore()
    {
        assert(visitedCells.empty());
        Point point(startLocation);
        int result = recursiveExplore(point);
        if (!clusterTypeFound)
        {
            clusterTypeFound = true;
        }
        return result;
    }

private:
    bool clusterTypeFound = false;
    bool clusterInside = true;
    int recursiveExplore(Point point)
    {
        visitedCells.insert(point);
        if (!clusterTypeFound)
        {
            if ((point.first == 0 || point.second == 0) && !maze.checkCellInMaze(point))
            {
                clusterTypeFound = true;
                clusterInside = false;
            }
            else if (point.first == (maze.lines.size() - 1) || point.second == (maze.lines[point.first].size() - 1))
            {
                clusterTypeFound = true;
                clusterInside = false;
            }
        }
        int coveredPoints = 0;
        if (!maze.checkCellInMaze(point))
        {
            coveredPoints += 1;
        }

        // maze.visualize(point);

        for (auto dir : ALL_DIRECTIONS)
        {
            if (canMove(point, dir) && visitedCells.find(point.moved(dir)) == visitedCells.end())
            {
                coveredPoints += recursiveExplore(point.moved(dir));
            }
        }
        return coveredPoints;
    }

    bool checkHorisontalMove(const Point &loc) const
    {
        char symbol = maze.lines.at(loc.first).at(loc.second);
        if (!maze.checkCellInMaze(loc))
        {
            return true;
        }
        return symbol != 'F' && symbol != '7' && symbol != '|';
    }

    bool checkVerticalMove(const Point &loc) const
    {
        char symbol = maze.lines.at(loc.first).at(loc.second);
        if (!maze.checkCellInMaze(loc))
        {
            return true;
        }
        return symbol != 'F' && symbol != 'L' && symbol != '-';
    }

    bool canMove(const Point &loc, const Direction direction) const
    {
        switch (direction)
        {
        case Down:
            if (loc.first >= (maze.lines.size() - 1))
            {
                return false;
            }
            return checkVerticalMove(loc.moved(direction));
        case Up:
            if (loc.first < 1)
            {
                return false;
            }
            return checkVerticalMove(loc);
        case Left:
            if (loc.second < 1)
            {
                return false;
            }
            return checkHorisontalMove(loc);
        case Right:
            if (loc.second >= (maze.lines[loc.first].size() - 1))
            {
                return false;
            }
            return checkHorisontalMove(loc.moved(direction));
        }
        assert(false);
    }
};

int main()
{
    string filename = "input.txt";
    MazeRunner runner = initializeMaze(filename);
    int stepsCount = runner.traverseMaze();
    assert((stepsCount % 2) == 0);
    Maze maze = runner.maze;

    vector<OffMazeCluster> clusters;

    int totalCoveredPoints = 0;
    int totalPointsInside = 0;

    cout << runner.maze.lines.size() * runner.maze.lines[0].size() << endl;

    unordered_set<Point, Point::Hash> pointsNotInCluster;
    for (int i = 0; i < maze.lines.size(); ++i)
    {
        for (int j = 0; j < maze.lines[i].size(); ++j)
        {
            pointsNotInCluster.insert(Point(i, j));
        }
    }

    while (!pointsNotInCluster.empty())
    {
        cout << "Points not in cluster: " << pointsNotInCluster.size() << endl;
        Point point = *pointsNotInCluster.begin();
        for (auto cluster : clusters)
        {
            assert(cluster.visitedCells.find(point) == cluster.visitedCells.end());
        }

        OffMazeCluster cluster(point, maze);
        int numPoints = cluster.explore();
        if (cluster.isInside())
        {
            totalPointsInside += numPoints;
        }
        totalCoveredPoints += numPoints;
        clusters.push_back(cluster);
        cout << "Found cluster with points: " << numPoints << endl;

        for (auto point : cluster.visitedCells) {
            pointsNotInCluster.erase(point);
        }
    }

    cout << stepsCount + totalCoveredPoints << " / "
         << runner.maze.lines.size() * runner.maze.lines[0].size() << endl;

    cout << "Points inside: " << totalPointsInside << endl;
    return 0;
}
