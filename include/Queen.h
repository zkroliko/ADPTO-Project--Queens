#ifndef ADPTO_QUEEN_H
#define ADPTO_QUEEN_H

#include <map>

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

class Queen {
private:
    unsigned short power;
    bool exists;
    std::map<Direction,Queen> connections;
public:

    Queen() { }

    Queen(unsigned short power) : power(power) { }

    inline unsigned short getPower() const {return power;}

    inline void addConnection(Direction direction, const Queen &queen)  {connections[direction] = queen;}

    inline const Queen& getConnection (Direction direction) const {return connections.at(direction);};

    inline std::map<Direction, Queen>* getConnections() { return &connections; }

    void setPower(unsigned short power);

    static unsigned short powerFromExternal(const unsigned long long);

    static unsigned long long powerToExternal(const unsigned short);

    bool isExists() const { return exists; }

    void setExists(bool exists) { Queen::exists = exists; }


};

#endif //ADPTO_QUEEN_H
