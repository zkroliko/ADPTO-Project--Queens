#ifndef ADPTO_QUEEN_H
#define ADPTO_QUEEN_H

const unsigned short MAX_QUEEN_POWER = 60;
const unsigned short MIN_QUEEN_POWER = 1;

class Queen {
private:
    unsigned short power;
public:

    Queen() { }

    Queen(unsigned short power) : power(power) { }

    inline unsigned short getPower() const {return power;}

    void setPower(unsigned short power);

    static unsigned short powerFromExternal(const unsigned long long);

    static unsigned long long powerToExternal(const unsigned short);

};

#endif //ADPTO_QUEEN_H
