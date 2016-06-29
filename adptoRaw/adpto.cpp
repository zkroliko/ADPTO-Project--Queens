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

    bool isConnected(const Queen& other) const;

    bool isConnected(const Direction& direction) const;

    unsigned short connectionCount() const;

    unsigned short viableConnectionCount() const;

    bool canJoin(const Queen& other) const;

    const Pos &getPosition() const { return position; }

    const Direction directionTo(const Queen *other) const;
};

/* ---------- Solver */

typedef std::vector<Queen*> QueenVector;
typedef std::vector<Move> MoveVector;

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
    void kernelize();
    void move(Queen *source, Queen *target);
    void ignore(Queen* queen);
    void ignoreUselessNeighbours(Queen *queen);
    bool uselessToNeighbours(Queen* queen);
    void undoStep();
    void undoIgnores();
    void undoMove();
    void undoIgnore();
    unsigned int countQueens();
    void outlineQueens();
    void sortQueens();
    bool check();
    Queen* findActiveQueen(Queen *source, Queen *target);
    Queen* findFutureJoinableQueen(Queen *source, Queen *target);
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
        for (Move move : *solver.getSolution()) {
            if (std::get<0>(move) != std::get<1>(move)) { // Not the same
                cout << std::get<0>(std::get<0>(move)->getPosition()) << " " << std::get<1>(std::get<0>(move)->getPosition());
                cout << " ";
                cout << std::get<0>(std::get<1>(move)->getPosition()) << " " << std::get<1>(std::get<1>(move)->getPosition());
                cout << endl;
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

bool Queen::isConnected(const Direction& direction) const {
    return connections.count(direction) >0;
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

const Direction Queen::directionTo(const Queen *other) const {
    for (auto entry : connections ) {
        if (entry.second == other) {
            return entry.first;
        }
    }
    std::__throw_logic_error("Connection improperly setup. One-way.");
}



/* -------- Solver implementation */

bool Solver::possible(unsigned int solutionRequirement) {
    target = solutionRequirement;
    queenCount = countQueens();
    outlineQueens();
    DEBUG("Solver: We have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    kernelize();
    DEBUG("Solver: AFTER KERNELIZATION we have: " << leftQueens.size() << " queens with a target of " << target << std::endl );
    DEBUG("Solver: Board after kernelization:");
    DEBUG(board.toString());
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
                Queen *possibility = findActiveQueen(current, connection.second);
                if (possibility && current->canJoin(*possibility)) {
                    // There is a viable connection to this direction
                    move(current, possibility);
                    if (check()) {
                        return true;
                    } else {
                        undoStep();
                    }
                }
            }
            // No viable connection
        }
    }
    return false;
}

void Solver::kernelize() {
    for (auto queen : leftQueens) {
        if (queen->getConnections()->empty()) {
            ignore(queen);
        }
    }
}

/*
 *  If at current time there is no neighbour with lower power - all have higher power,
 *  then we can assume that the current queen won't be useful for the solution
 *  because no chain of reduction can pass through it,
 *  therefore it is useless
 */
bool Solver::uselessToNeighbours(Queen *queen) {
    for (auto connection: *queen->getConnections()) {
        Queen* other = findFutureJoinableQueen(queen, connection.second);
        if (other) {
            return false;
        }
    }
    return true;
}

/* Uses above function to reduce some problems */
void Solver::ignoreUselessNeighbours(Queen *queen) {
    for (auto connection: *queen->getConnections()) {
        Queen* other = findActiveQueen(queen, connection.second);
        if (other) {
            if (queen->doesExist()) {
                if (queen->getPower() > other->getPower() && uselessToNeighbours(other)) {
                    ignore(other);
                }
            } else {
                if (uselessToNeighbours(other)) {
                    ignore(other);
                }
            }
        }
    }
}

void Solver::move(Queen *source, Queen *target) {
    source->setExists(false);
    target->setPower(target->getPower()+ static_cast<unsigned short>(1));
    moves.push_back(Move(source,target));
    ignoreUselessNeighbours(source);
    ignoreUselessNeighbours(target);
    queenCount--;
}

/* Cannot be used in the solution, we are now solving a problem for k:=k-1 */
inline void Solver::ignore(Queen *queen) {
    if (queen->doesExist()) {
        move(queen,queen);
        target--;
    } else {
        std::__throw_logic_error("Cannot ignore already inactive queen");
    }
}

void Solver::undoStep() {
    undoIgnores(); // ignored queens
    undoMove(); // the move itself
}

/* Ignoring a queens still counts as move but with equal source and target
 * ignoring is done after each move so we must first dispose of these reductions
 */
void Solver::undoIgnores() {
    while (!moves.empty() && std::get<0>(moves.back()) == std::get<1>(moves.back())) {
        undoIgnore();
    }
}

/* Takes a single move from stack and restores the state before it */
void Solver::undoMove() {
    Queen* source = std::get<0>(moves.back());
    Queen* target = std::get<1>(moves.back());
    moves.pop_back();
    target->setPower(target->getPower()- static_cast<unsigned short>(1));
    source->setExists(true);
    queenCount++;
}

/* Takes a single move from stack restores the state before it - treats it as an ignore-move   */
void Solver::undoIgnore() {
    undoMove();
    target++;
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
    std::sort(leftQueens.begin(),leftQueens.end(), [ ]( Queen* lhs, Queen* rhs )
    {
        return lhs->getPower() < rhs->getPower();
    });
}

/* Find a queen in line with which there is any possiblity of joining */
Queen *Solver::findFutureJoinableQueen(Queen *source, Queen *target) {
    if ((target->doesExist() && target->getPower() <= source->getPower())) {
        return target;
    }
    bool found = false;
    Direction direction = source->directionTo(target);
    Queen* current = target;
    while(!(current->doesExist() && current->getPower() <= source->getPower()) && current->isConnected(direction)) {
        current = current->getConnection(direction);
        found = true;
    }
    if (found) {
        return current;
    } else {
        return nullptr;
    }
}

/* Find an active(existing) queen */
Queen *Solver::findActiveQueen(Queen *source, Queen *target) {
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






























