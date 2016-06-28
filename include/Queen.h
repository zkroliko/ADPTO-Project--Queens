#ifndef ADPTO_QUEEN_H
#define ADPTO_QUEEN_H

#include <map>
#include "Postition.h"

class Queen;

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
//
//
//struct reversedDirection{
//    static std::map<Direction ,Direction > create_map()
//    {
//        std::map<Direction ,Direction > m;
//        m[top_left] = bottom_right;
//        m[top] = bottom;
//        m[top_right] = bottom_left;
//        m[left] = right;
//        m[right] = left;
//        m[bottom_left] = top_right;
//        m[bottom] = top;
//        m[bottom_right] = top_left;
//        return m;
//    }
//    static const std::map<Direction ,Direction > reversedDir;
//};
//
//const std::map<Direction ,Direction > reversedDirection::reversedDir = reversedDirection::create_map();

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

#endif //ADPTO_QUEEN_H
