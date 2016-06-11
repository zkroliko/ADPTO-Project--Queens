#ifndef ADPTO_QUEEN_H
#define ADPTO_QUEEN_H

const short MAX_QUEEN_POWER = 60;

class Queen {
private:
    short power;
public:

    Queen() { }

    Queen(short power) : power(power) { }

    inline short getPower() const;

    inline void setPower(short power);
};

#endif //ADPTO_QUEEN_H
