#ifndef ADPTO_QUEEN_H
#define ADPTO_QUEEN_H

const unsigned short MAX_QUEEN_POWER = 60;

class Queen {
private:
    unsigned short power;
public:

    Queen() { }

    Queen(unsigned short power) : power(power) { }

    inline unsigned short getPower() const;

    inline void setPower(unsigned short power);
};

#endif //ADPTO_QUEEN_H
