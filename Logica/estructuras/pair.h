//
// Created by j1p2p3a4 on 5/18/2026.
//

#ifndef TANKATTACK_PAIR_H
#define TANKATTACK_PAIR_H
template <typename T1, typename T2>
struct pair {
    T1 first; T2 second;

    pair(T1 a, T2 b) {
        first = a;
        second = b;
    }

    pair() : first() , second() {}

    pair& operator=(const pair& otro) {
        first = otro.first;
        second = otro.second;
        return *this;
    }

    bool operator==(const pair& otro) {
        return (first == otro.first && second == otro.second);
    }
    bool operator != (const pair&otro) {
        return (first != otro.first || second != otro.second);
    }

};
#endif //TANKATTACK_PAIR_H
