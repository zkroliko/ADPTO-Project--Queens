#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <tuple>
#include<map>
#include <algorithm>

//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
#define DEBUG(x) std::cerr << x << std::endl;
#else
#define DEBUG(x)
#endif

class Loader;
class Board;
class Queen;

typedef std::tuple<Queen*,Queen*> Move;

typedef std::tuple<unsigned short,unsigned short> Pos;

/* ------ Board */

enum Direction
{
    top_left = 0,
    top = 1,
    top_right = 2,
    left = 3,
    right = 4,
    bottom_left = 5,
    bottom = 6,
    bottom_right = 7
};

const unsigned short MAX_BOARD_SIZE = 128;

typedef std::map<Pos, Queen*> PlacementMap;

class Board {
private:
    unsigned short size;
    PlacementMap queens;
public:
    Board() {}
    Board(unsigned short size);
    void addQueen(Queen& queen, const unsigned short x, const unsigned short y);
    bool changeQueen(unsigned short x, unsigned short y, unsigned short power);
    bool removeQueen(unsigned short x, unsigned short y);
    bool occupied(unsigned short x, unsigned short y) { return queens.count(Pos(x,y)) > 0; }
    Queen* get(unsigned short x, unsigned short y) { return queens.at(Pos(x,y));}
    short getSize() const { return size;}
    std::string toString();
    PlacementMap* getQueens() { return &queens; }
};

/* --------- Loader */

class Loader {
public:
    static Board* load(const unsigned short size);
};

/* --------- Queen */

const unsigned short MAX_QUEEN_POWER = 60;
const unsigned short MIN_QUEEN_POWER = 0;

typedef std::map<Direction,Queen*> ConnectionMap;

class Queen {
private:
    unsigned short power;
    Pos position;
    bool exists;
    ConnectionMap connections;
public:

    Queen() { exists = true ; }

    Queen(unsigned short power, const Pos &position) : power(power), position(position) { exists = true ;}

    inline unsigned short getPower() const {return power;}

    inline void addConnection(Direction direction, Queen* queen)  {connections[direction] = queen;}

    inline Queen* getConnection (Direction direction) const {return connections.at(direction);};

    inline ConnectionMap* getConnections() { return &connections; }

    void setPower(unsigned short power);

    static unsigned short powerFromExternal(const unsigned long long);

    static unsigned long long powerToExternal(const unsigned short);

    bool doesExist() const { return exists; }

    void setExists(bool exists) { Queen::exists = exists; }

    unsigned short connectionCount() const;

    unsigned short viableConnectionCount() const;

    bool isConnected(const Queen& other) const;

    bool canJoin(const Queen& other) const;

    const Pos &getPosition() const { return position; }
};

/* ---------- Solver */

typedef std::vector<Queen*> QueenVector;
typedef std::vector<Move*> MoveVector;

class Solver {
    Board board;
    QueenVector leftQueens;
    MoveVector moves;
    unsigned int queenCount;
    unsigned int target;
public:
    Solver(Board& board) : board(board) {  }

    void setBoard(Board &board) {
        Solver::board = board;
    }

    bool possible(unsigned int target);

    const std::vector<Move*> *getSolution() const {
        return &moves;
    }
private:
    unsigned int kernelize(unsigned int target);
    bool moveValid(const Queen& source, const Queen& target) const;
    void move(Queen *source, Queen *target);
    void undo();
    unsigned int countQueens();
    void outlineQueens();

    bool check();
};


/* ----------------------------------------
 * ----------------------------------------
 *
 *  IMPLEMENTATIONS
 *
 * ----------------------------------------
 * ----------------------------------------
 */

/* ------------ An util */

template <typename M, typename V>
void mapToVec( const  M & m, V & v ) {
    for( typename M::const_iterator it = m.begin(); it != m.end(); ++it ) {
        v.push_back( it->second );
    }
}

/* ----------- Board */

Board::Board(unsigned short size) : size(std::min(MAX_BOARD_SIZE,size)) {}

void Board::addQueen(Queen& queen, const unsigned short x, const unsigned short y) {
    Board::queens[Pos(x,y)] = &queen;
}

std::string Board::toString() {
    std::string result;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            if (queens.count(Pos(i,j)) > 0 && queens[Pos(i, j)]->doesExist()){
                result += std::to_string(Queen::powerToExternal(queens[Pos(i,j)]->getPower()));
            } else {
                result += "0";
            }
            result += "    ";
        }
        result += "\n";
    }
    return result;
}

bool Board::removeQueen(unsigned short x, unsigned short y) {
    if (queens.count(Pos(x,y)) > 0 ) {
        queens[Pos(x,y)]->setExists(false);
        return true;
    } else {
        return false;
    }
}

bool Board::changeQueen(unsigned short x, unsigned short y, unsigned short power) {
    if (queens.count(Pos(x,y)) >0 ) {
        queens[Pos(x,y)]->setPower(power);
        return false;
    } else {
        return false;
    }
}


/* --------------------- Loader implementation */

using namespace std;

Board* Loader::load(const unsigned short size) {
    if (size > MAX_BOARD_SIZE) {
        __throw_invalid_argument("Invalid board size");
    }
    Board *board = new Board(size);

    // Loading and making the horizontal connection_count at the same time

    std::string element;
    unsigned short power;
    Queen *last = nullptr;

    int connection_count = 0;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            cin >> element;
            unsigned long long parsedLong = std::stoull(element);
            if (parsedLong > 0) {
                power = static_cast<unsigned short>(Queen::powerFromExternal(parsedLong));
                Queen* created = new Queen(power, Pos(i,j));
                board->addQueen(*created, i, j);
                if (last) {
                    last->addConnection(Direction::right, created);
                    created->addConnection(Direction::left, last);
                    connection_count++;
                }
                last = created;
            }
        }
        last = nullptr;
    }
    DEBUG("Horizontal connections added: " << connection_count );
    // Making the vertical connections
    connection_count = 0;
    for(unsigned short i = 0; i < size; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            if (board->occupied(j,i)) {
                Queen* actual = board->get(j,i);
                if (last) {
                    last->addConnection(Direction::bottom, actual);
                    actual->addConnection(Direction::top, last);
                    connection_count++;
                }
                last = actual;
            }
        }
        last = nullptr;
    }
    DEBUG("Vertical connections added: " << connection_count );

    // Making the diagonal left to right connections
    connection_count = 0;
    for(int i = -size+2; i < size-1; ++i) {
        for(unsigned short j = 0; j < size; ++j) {
            unsigned short x = static_cast<unsigned short>(j+i);
            unsigned short y = j;
            if (board->occupied(y, x)) {
                Queen* actual = board->get(y, x);
                if (last) {
                    last->addConnection(Direction::bottom_right, actual);
                    actual->addConnection(Direction::top_left, last);
                    connection_count++;
                }
                last = actual;
            }
        }
        last = nullptr;
    }
    DEBUG("Diagonal left to right connections added: " << connection_count );

    // Making the diagonal right to left connections
    connection_count = 0;
    for(short i = 0; i > -size; --i) {
        for(unsigned short j = 0; j < size; ++j) {
            unsigned short x = size-j+i;
            unsigned short y = j;
            if (board->occupied(y, x)) {
                Queen* actual = board->get(y, x);
                if (last) {
                    last->addConnection(Direction::bottom_left, actual);
                    actual->addConnection(Direction::top_right, last);
                    connection_count++;
                }
                last = actual;
            }
        }
        last = nullptr;
    }
    DEBUG("Diagonal right to left connections added: " << connection_count );

    return board;
}

using namespace std;

/* ------------- MAIN */

int main() {
    unsigned int solution_size;
    unsigned short board_size;
    /* Getting the board specification */

    cin >> board_size;
    DEBUG("Board size: " << board_size);
    cin >> solution_size;
    DEBUG("Will be solving solution of size: " << solution_size);
    // Getting the board from the loader
    DEBUG("Loading board");
    Board* board = Loader::load(board_size);
    DEBUG("Loading board finished");
    DEBUG("The board:");
    DEBUG(board->toString());
    DEBUG("Searching for a solution of size: " << solution_size);
    Solver solver(*board);
    if (solver.possible(solution_size)) {
        DEBUG("Solution found");
        for (Move* move : *solver.getSolution()) {
            cout << std::get<0>(std::get<0>(*move)->getPosition()) << " " << std::get<1>(std::get<0>(*move)->getPosition());
            cout << " ";
            cout << std::get<0>(std::get<1>(*move)->getPosition()) << " " << std::get<1>(std::get<1>(*move)->getPosition());
            cout << endl;
            // Deallocating moves as we go
            delete move;
        }
    } else {
        DEBUG("Solution not found");
    }

    DEBUG("Cleaning up");
    for (auto record: *board->getQueens()) {
        delete record.second;
    }
    delete board;

    return 0;
}

/* ------------- Queen implementation */

void Queen::setPower(const unsigned short power) {
    Queen::power = std::max(MIN_QUEEN_POWER,std::min(power,MAX_QUEEN_POWER));
}

unsigned short Queen::powerFromExternal(const unsigned long long power) {
    return static_cast<unsigned short>(log2(power));
}

unsigned long long Queen::powerToExternal(const unsigned short power) {
    return static_cast<unsigned long long int> (pow(2, power));
}

bool Queen::isConnected(const Queen &other) const {
    for (auto connection : connections) {
        if (connection.second == &other) {
            return true;
        }
    }
    return false;
}


bool Queen::canJoin(const Queen &other) const {
    return exists && other.exists && power == other.power;
}

unsigned short Queen::connectionCount() const {
    return static_cast<unsigned short>(connections.size());
}

unsigned short Queen::viableConnectionCount() const {
    unsigned short count = 0;
    for (auto connection : connections) {
        if (canJoin(*connection.second)){
            count++;
        }
    }
    return count;
}

/* -------- Solver implementation */

bool Solver::possible(unsigned int solutionRequirement) {
    target = kernelize(solutionRequirement);
    queenCount = countQueens();
    DEBUG("Solver: We have: " << queenCount << " queens with a target of " << target << std::endl );
    outlineQueens();
    DEBUG("After kernelization:");
    DEBUG(board.toString());
    DEBUG("Now running recursive function.");
    return check();
}

bool Solver::check() {
    if (queenCount <= target) {
        return true;
    }
    for (Queen * queen: leftQueens) {
        if (queen->doesExist()) {
            for (auto connection : *queen->getConnections()) {
                if (queen->canJoin(*connection.second)) {
                    move(queen,connection.second);
                    if (check()) {
                        return true;
                    } else {
                        undo();
                    }
                }
            }
        }
    }
    return false;
}

unsigned int Solver::kernelize(unsigned int target) {
    unsigned int removed = 0;

    for (auto entry : *board.getQueens()) {
        if (entry.second->getConnections()->empty()) {
            entry.second->setExists(false);
            removed++;
        }
    }
    return target-removed;
}

bool Solver::moveValid(const Queen &source, const Queen &target) const{
    return source.canJoin(target);
}

void Solver::move(Queen *source, Queen *target) {
    source->setExists(false);
    target->setPower(target->getPower()+ static_cast<unsigned short>(1));
    moves.push_back(new Move(source,target));
    queenCount--;
}

void Solver::undo() {
    Queen* source = std::get<0>(*moves.back());
    Queen* target = std::get<1>(*moves.back());
    delete moves.back();
    moves.pop_back();
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
    queenCount++;
}

unsigned int Solver::countQueens() {
    unsigned int count = 0;

    for (auto entry : *board.getQueens()) {
        if (entry.second->doesExist()) {
            count++;
        }
    }
    return count;
}

void Solver::outlineQueens() {
    QueenVector retrieved;
    mapToVec<PlacementMap,QueenVector>(*board.getQueens(),retrieved);
    for (QueenVector::iterator it = retrieved.begin(); it != retrieved.end(); ++it) {
        if (it.operator*()->doesExist()) {
            leftQueens.push_back(it.operator*());
        }
    }
    std::sort(leftQueens.begin(),leftQueens.end(), [ ]( Queen* lhs, Queen* rhs )
    {
        return lhs->getPower() < rhs->getPower();
    });
}























