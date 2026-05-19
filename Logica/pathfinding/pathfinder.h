//
// Created by j1p2p3a4 on 5/18/2026.
//

#ifndef TANKATTACK_PATHFINDER_H
#define TANKATTACK_PATHFINDER_H


#include "juego/Tanque.h"
#include "mapa/Mapa.h"
#include "estructuras/Cola.h"
#include "estructuras/pair.h"
#include "estructuras/priority_queue.h"
using path = ArregloDinamico<pair<int,int>>;
class pathfinder {
public:

    bool check_conectividad(pair<int,int> beg, pair<int,int> end);
    pathfinder(Mapa* m);
    path bfs(pair<int,int> begin, pair<int,int> end);
    path djikstra(pair<int,int> begin, pair<int,int> end);
    path Astar(pair<int,int> begin, pair<int,int> end);
private:
    Mapa* mapa = nullptr;

};



#endif //TANKATTACK_PATHFINDER_H
