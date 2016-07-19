#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <tuple>
#include <map>
#include <algorithm>
#include <sstream>

//#define DEBUG_ENABLED

#ifdef DEBUG_ENABLED
#define DEBUG(x) std::cerr << x << std::endl;
#else
#define DEBUG(x)
#endif

class Loader;
class Board;
class Queen;
class Move;
class Ignore;


typedef std::tuple<unsigned short,unsigned short> Pos;
typedef std::map<unsigned short, unsigned short> QueenCount;

enum Axes
{
    horizontal = 0, vertical = 1, diagonal_left = 2, diagonal_right = 3
};

/* ------ Board */

const unsigned short MAX_BOARD_SIZE = 128;

typedef std::map<Pos, Queen*> PlacementMap;

class Board {
private:
    unsigned short size;
    PlacementMap queens;
    QueenCount rowQueenCount;
    QueenCount columnQueenCount;
    QueenCount leftDiagonalQueenCount;
    QueenCount rightDiagonalQueenCount;
public:
    Board() {size = 0;}
    Board(unsigned short size);
    void addQueen(Queen& queen, const unsigned short x, const unsigned short y);
    bool changeQueen(unsigned short x, unsigned short y, unsigned short power);
    bool removeQueen(unsigned short x, unsigned short y);
    bool occupied(unsigned short x, unsigned short y) { return queens.count(Pos(x,y)) > 0; }
    Queen* get(unsigned short x, unsigned short y) { return queens.at(Pos(x,y));}
    short getSize() const { return size;}
    std::string toString();
    PlacementMap* getQueens() { return &queens; }
    unsigned short limitOfMovesToPosition(const Pos& pos);
    inline unsigned short posToAxis(const Pos& pos, const Axes);
    inline unsigned short posToRow(const Pos& pos);
    inline unsigned short posToColumn(const Pos& pos);
    inline unsigned short posToLeftDiagonal(const Pos& pos);
    inline unsigned short posToRightDiagonal(const Pos& pos);

private:
    void incrementQueenCounts(const Pos& pos);
    void decrementQueenCounts(const Pos& pos);
};

/* --------- Loader */

class Loader {
public:
    static Board* load(const unsigned short size);
};

/* --------- Queen */

const unsigned short MAX_QUEEN_POWER = 60;
const unsigned short MIN_QUEEN_POWER = 0;

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

typedef std::map<Direction,Queen*> ConnectionMap;
typedef std::vector<Queen*> PossibleMoves;

class Queen {
private:
    unsigned short power;
    Pos position;
    bool exists;
    ConnectionMap connections;
public:

    Queen() { exists = true; power = 0; }
    Queen(unsigned short power, const Pos &position) : power(power), position(position) { exists = true; }
    inline unsigned short getPower() const { return power; }
    inline void addConnection(Direction direction, Queen *queen) { connections[direction] = queen; }
    inline Queen *getConnection(Direction direction) const { return connections.at(direction); };
    inline ConnectionMap *getConnections() { return &connections; }
    inline void setPower(unsigned short power) { Queen::power = std::max(MIN_QUEEN_POWER,std::min(power,MAX_QUEEN_POWER));}
    static unsigned short powerFromExternal(const unsigned long long);
    static unsigned long long powerToExternal(const unsigned short);
    bool doesExist() const { return exists; }
    void setExists(bool exists) { Queen::exists = exists; }
    bool isConnected(const Queen &other) const;
    bool isConnected(const Direction &direction) const {return connections.count(direction) >0;};
    bool useless();
    unsigned short connectionCount() const {return static_cast<unsigned short>(connections.size());};
    unsigned short viableConnectionCount() const;
    bool canJoin(const Queen &other) const {return exists && other.exists && power == other.power;};
    const Pos &getPosition() const { return position; }
    const Direction directionTo(const Queen *other) const;

    static Queen* findActiveQueen(Queen *source, Queen *target);
    static Queen* findFutureJoinableQueen(Queen *source, Queen *target);
};


/* ---------- Actions */

using namespace std;

typedef vector<Ignore*> ReductionVector;

class Action {
public:
    virtual short apply() = 0;
    virtual short undo() = 0;
    virtual string toString() = 0;
    virtual bool isIgnore() = 0;
};

class Move: public Action {
    Queen* source;
    Queen* target;
    ReductionVector reductions;
public:
    Move() { }
    Move(Queen *source, Queen *target) : source(source), target(target) { }
    virtual short apply();
    virtual short undo();
    virtual string toString();
    virtual bool isIgnore() { return false; }
    Queen *getSource() const { return source;}
    Queen *getTarget() const { return target;}

};

class Ignore: public Action {
    Queen* queen;
public:
    Ignore(Queen *queen) : queen(queen) { }
    virtual short apply();
    virtual short undo();
    virtual bool isIgnore() { return true; }
    Queen *getQueen() const { return queen;}
};


/* ---------- Solver */

typedef std::vector<Queen*> QueenVector;
typedef std::vector<Move> MoveVector;

const unsigned short POWER_WEIGHT = 1;
const unsigned short CONNECTION_COUNT_WEIGHT = 2;

class Solver {
    Board board;
    QueenVector leftQueens;
    MoveVector moves;
    int queenCount;
    int target;
public:
    Solver(Board& board) : board(board) {  }

    void setBoard(Board &board) {
        Solver::board = board;
    }

    bool possible(unsigned int target);

    const MoveVector *getSolution() const {
        return &moves;
    }
private:
    void move(Queen *source, Queen *target);
    void undoMove();
    unsigned int countQueens();
    void outlineQueens();
    void sortQueens();
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

inline unsigned short Board::posToRow(const Pos &pos) {
    return std::get<0>(pos);
}

inline unsigned short Board::posToColumn(const Pos &pos) {
    return std::get<1>(pos);
}

inline unsigned short Board::posToLeftDiagonal(const Pos &pos) {
    return static_cast<unsigned short>(std::get<0>(pos) - std::get<1>(pos) + size - 1);
}

inline unsigned short Board::posToRightDiagonal(const Pos &pos) {
    return static_cast<unsigned short>(2*size-2-std::get<0>(pos) - std::get<1>(pos));
}

unsigned short Board::posToAxis(const Pos &pos, const Axes axis) {
    switch( axis )
    {
        case horizontal:
            return posToRow(pos);
        case vertical:
            return posToColumn(pos);
        case diagonal_left:
            return posToLeftDiagonal(pos);
        case diagonal_right:
            return posToRightDiagonal(pos);

        default:
            std::__throw_logic_error("No such axis");
    }
}

void Board::incrementQueenCounts(const Pos &pos) {
    QueenCount* axes[] = {&rowQueenCount,&columnQueenCount,&leftDiagonalQueenCount,&rightDiagonalQueenCount};
    for (unsigned int i = 0; i < 4; ++i) {
        if (axes[i]->count(posToAxis(pos,Axes(i))) > 0) {
            (*axes[i])[posToAxis(pos,Axes(i))]++;
        } else {
            (*axes[i])[posToAxis(pos,Axes(i))] = 1;
        }
    }
}

void Board::decrementQueenCounts(const Pos &pos) {
    QueenCount* axes[] = {&rowQueenCount,&columnQueenCount,&leftDiagonalQueenCount,&rightDiagonalQueenCount};
    for (unsigned int i = 0; i < 4; ++i) {
        if (axes[i]->count(posToAxis(pos,Axes(i))) > 0) {
            (*axes[i])[posToAxis(pos,Axes(i))]--;
        } else {
            std::__throw_logic_error("Decrementing should only be done after incrementing - probably there is no such queen");
        }
    }
}

unsigned short Board::limitOfMovesToPosition(const Pos& pos) {
    unsigned short total = 0;
    QueenCount* axes[] = {&rowQueenCount,&columnQueenCount,&leftDiagonalQueenCount,&rightDiagonalQueenCount};
    for (unsigned int i = 0; i < 4; ++i) {
        if (axes[i]->count(posToAxis(pos,Axes(i))) > 0) {
            total += (*axes[i])[posToAxis(pos,Axes(i))] - 1;
        }
    }
    return total;
}



/* --------------------- Loader implementation */

using namespace std;

Board *Loader::load(const unsigned short size) {
    if (size > MAX_BOARD_SIZE) {
        __throw_invalid_argument("Invalid board size");
    }
    Board *board = new Board(size);

    // Loading and making the horizontal connection_count at the same time

    std::string element;
    unsigned short power;
    Queen *last = nullptr;

    int connection_count = 0;
    for (unsigned short i = 0; i < size; ++i) {
        for (unsigned short j = 0; j < size; ++j) {
            cin >> element;
            unsigned long long parsedLong = std::stoull(element);
            if (parsedLong > 0) {
                power = Queen::powerFromExternal(parsedLong);
                Queen *created = new Queen(power, Pos(i, j));
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
    DEBUG("Horizontal connections added: " << connection_count);
    // Making the vertical connections
    connection_count = 0;
    for (unsigned short i = 0; i < size; ++i) {
        for (unsigned short j = 0; j < size; ++j) {
            if (board->occupied(j, i)) {
                Queen *actual = board->get(j, i);
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
    DEBUG("Vertical connections added: " << connection_count);

    // Making the diagonal left to right connections
    connection_count = 0;
    for (int i = -size + 2; i < size - 1; ++i) {
        for (unsigned short j = 0; j < size; ++j) {
            unsigned short x = static_cast<unsigned short>(j + i);
            unsigned short y = j;
            if (board->occupied(y, x)) {
                Queen *actual = board->get(y, x);
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
    DEBUG("Diagonal left to right connections added: " << connection_count);

    // Making the diagonal right to left connections
    connection_count = 0;
    for (short i = size; i > -size; --i) {
        for (unsigned short j = 0; j < size; ++j) {
            unsigned short x = size - j + i;
            unsigned short y = j;
            if (board->occupied(y, x)) {
                Queen *actual = board->get(y, x);
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
    DEBUG("Diagonal right to left connections added: " << connection_count);

    return board;
}

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
    DEBUG("Searching for a solution of size: " << solution_size);
    Solver solver(*board);
    if (solver.possible(solution_size)) {
        DEBUG("Solution found");
        for (auto move : *solver.getSolution()) {
            if (!move.isIgnore()) {
                cout << move.toString() << endl;
            }
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

unsigned short Queen::viableConnectionCount() const {
    unsigned short count = 0;
    for (auto connection : connections) {
        if (canJoin(*connection.second)){
            count++;
        }
    }
    return count;
}

const Direction Queen::directionTo(const Queen *other) const{
    for (auto entry : connections ) {
        if (entry.second == other) {
            return entry.first;
        }
    }
    std::__throw_logic_error("Connection improperly setup. One-way.");
}

/*
 *  If at current time there is no neighbour with lower power - all have higher power,
 *  then we can assume that the current queen won't be useful for the solution
 *  because no chain of reduction can pass through it,
 *  therefore it is useless
 */
bool Queen::useless() {
    for (auto connection: connections) {
        Queen* other = Queen::findFutureJoinableQueen(this, connection.second);
        if (other) {
            return false;
        }
    }
    return true;
}

/* Find a queen in line with which there is any possiblity of joining */
Queen *Queen::findActiveQueen(Queen *source, Queen *target) {
    if ((target->doesExist() && target->getPower() <= source->getPower())) {
        return target;
    }
    Direction direction = source->directionTo(target);
    Queen* current = target;
    while(!current->doesExist() && current->isConnected(direction)) {
        current = current->getConnection(direction);
    }
    if (current->doesExist() && current->getPower() <= source->getPower()) {
        return current;
    } else {
        return nullptr;
    }
}

/* Find an active(existing) queen */
Queen *Queen::findFutureJoinableQueen(Queen *source, Queen *target) {
    if (target->doesExist()) {
        return target;
    }
    bool found = false;
    Direction direction = source->directionTo(target);
    Queen* current = target;
    while(!current->doesExist() && current->isConnected(direction)) {
        current = current->getConnection(direction);
        found = true;
    }
    if (found) {
        return current;
    } else {
        return nullptr;
    }
}

/* -------- Actions implementation */


/* Returns the number of reductions in this move */
short Move::apply() {
    source->setExists(false);
    target->setPower(target->getPower()+ static_cast<unsigned short>(1));
    return 0;
}

/* Returns the number of reverted reductions in this move */
short Move::undo() {
    for (auto reduction: reductions) {
        reduction->undo();
    }
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
    return static_cast<unsigned short>(reductions.size());
}

string Move::toString() {
    std::stringstream ss;
    ss << std::get<0>(source->getPosition()) << " " << std::get<1>(source->getPosition());
    ss << " ";
    ss << std::get<0>(target->getPosition()) << " " << std::get<1>(target->getPosition());
    return ss.str();
}

short Ignore::apply() {
    if (queen->doesExist()) {
        queen->setExists(false);
    } else {
        std::__throw_logic_error("Cannot ignore already inactive queen");
    }
    return -1;
}

short Ignore::undo() {
    queen->setExists(true);
    return 1;
}

/* -------- Solver implementation */

bool Solver::possible(unsigned int solutionRequirement) {
    target = solutionRequirement;
    queenCount = countQueens();
    outlineQueens();
    DEBUG("Solver: We have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    DEBUG("Now running recursive function.");
    return check();
}

bool Solver::check() {
    if (queenCount <= target) {
        return true;
    }
    sortQueens();
    for (Queen *current: leftQueens) {
        if (current->doesExist()) {
            for (auto connection : *current->getConnections()) {
                Queen *possibility = Queen::findActiveQueen(current, connection.second);
                if (possibility && current->canJoin(*possibility)) {
                    move(current, possibility);
                    if (check()) {
                        return true;
                    }
                }
            }
        }
    }
    undoMove();
    return false;
}

void Solver::move(Queen *source, Queen *target) {
    Move move(source,target);
    target -= move.apply();
    moves.push_back(move);
    queenCount--;
}

void Solver::undoMove() {
    target += moves.back().undo();
    moves.pop_back();
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
}

void Solver::sortQueens() {
    std::sort(leftQueens.begin(),leftQueens.end(), []( Queen* lhs, Queen* rhs )
    {
        unsigned short leftConnectionCount = lhs->connectionCount();
        unsigned short rightConnectionCount = rhs->connectionCount();
        unsigned short left = lhs->getPower()*POWER_WEIGHT+leftConnectionCount*CONNECTION_COUNT_WEIGHT;
        unsigned short right = rhs->getPower()*POWER_WEIGHT+rightConnectionCount*CONNECTION_COUNT_WEIGHT;
        return left < right;
    });
}












































