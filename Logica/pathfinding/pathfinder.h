//
// Created by j1p2p3a4 on 5/18/2026.
//

#ifndef TANKATTACK_PATHFINDER_H
#define TANKATTACK_PATHFINDER_H

<<<<<<< HEAD
=======

>>>>>>> origin/main
#include "juego/Tanque.h"
#include "mapa/Mapa.h"
#include "estructuras/Cola.h"
#include "estructuras/pair.h"
#include "estructuras/priority_queue.h"
<<<<<<< HEAD
#include <random>
#include <cmath>

#ifndef PATH_TYPE_DEFINED
#define PATH_TYPE_DEFINED
using path = ArregloDinamico<pair<int,int>>;
#endif

class pathfinder {
public:
    pathfinder(Mapa* m);
    bool check_conectividad(int ib, int jb, int ie, int je);
    path bfs(pair<int,int> begin, pair<int,int> end);
    path dijkstra(pair<int,int> begin, pair<int,int> end);
    path aStar(pair<int,int> begin, pair<int,int> end);
    ArregloDinamico<pair<int,int>> casillasEnRango(pair<int,int> origen, int rango);
    pair<int,int> movimientoAleatorio(int fila, int columna, int radio);
    path decidirMovimiento(Tanque* tanque, pair<int,int> destino, bool powerupPrecision);

private:
    Mapa* mapa;
    std::mt19937 rng;
    bool lineaVista(int f1, int c1, int f2, int c2);
    int heuristica(int f1, int c1, int f2, int c2);
    pair<int,int> avanzarHastaObstaculo(int fOrigen, int cOrigen, int fDir, int cDir);
};

#endif
=======
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
>>>>>>> origin/main
