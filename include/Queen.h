#ifndef ADPTO_QUEEN_H
#define ADPTO_QUEEN_H

#include <map>
#include <vector>
#include "Postition.h"
#include "Board.h"

class Queen;
class Board;

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
    Board* board;
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
    unsigned short connectionCount() const {return static_cast<unsigned short>(connections.size());};
    unsigned short viableConnectionCount() const;
    bool canJoin(const Queen &other) const {return exists && other.exists && power == other.power;};
    const Pos &getPosition() const { return position; }
    const Direction directionTo(const Queen *other) const;
    short rating() const;
    bool useless();


    void setBoard(Board *board) {
        Queen::board = board;
    }

    static Queen* findActiveQueen(Queen *source, Queen *target);
    static Queen* findFutureJoinableQueen(Queen *source, Queen *target);
};

#endif //ADPTO_QUEEN_H
