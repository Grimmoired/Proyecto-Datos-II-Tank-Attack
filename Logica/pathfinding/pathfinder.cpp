//
// Created by j1p2p3a4 on 5/18/2026.
//

#include "pathfinder.h"



using path = ArregloDinamico<pair<int,int>>;

pathfinder::pathfinder(Mapa *m) {
    mapa = m;
}

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};
bool pathfinder::check_conectividad(pair<int,int> beg, pair<int,int> end) {
    int filas = mapa ->getFilas();
    int columnas = mapa -> getColumnas();
    int ib = beg.first, jb = end.first, ie = end.first, je = end.second;
    Cola<pair<int,int>> q;
    bool vis[filas][columnas];


    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            vis[i][j] = false;
        }
    }



    q.encolar(beg);
    vis[ib][jb] = true;

    while (!q.vacia()) {
        pair<int,int> current = q.frente(); q.desencolar();
        if (current.first == ib && current.second == je) {
            vis[ib][je] = true;
            break;
        }

        for (int t = 0; t < 4; t++) {
            int i = current.first + dx[t];
            int j = current.second + dy[t];

            if (i < 0 || i >= filas || j < 0 || j >= columnas) continue;
            if (vis[i][j]) continue;
            if (mapa -> getCasilla(i,j).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa -> getCasilla(i,j).getTanque() != nullptr) continue;

            vis[i][j] = true;
            if (i == ib && j == je) {
                vis[ib][je] = true;
                break;
            }
            pair next(i,j);
            q.encolar(next);

        }

        if (vis[ib][je]) break;
    }


    return vis[ib][je];

}
path pathfinder::bfs(pair<int,int> beg, pair<int,int> end) {
    path p;

    if (!check_conectividad(beg, end)) {
        return p;
    }


    int filas = mapa -> getFilas();
    int columnas = mapa -> getColumnas();

    pair<int,int> parent[filas][columnas];
    bool vis[filas][columnas];

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            vis[i][j] = false;
        }
    }

    Cola<pair<int,int>> q;
    q.encolar(beg);
    vis[beg.first][beg.second] = true;

    while (!q.vacia()) {
        bool found = false;
        pair<int,int>  current = q.frente(); q.desencolar();
        for (int t = 0; t < 4; t++) {

            int i = current.first + dx[t], j = current.second + dy[t];

            if (i < 0 || i >= filas || j < 0 || j >= columnas) continue;
            if (vis[i][j]) continue;
            if ((mapa -> getCasilla(i,j)).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa -> getCasilla(i,j).getTanque() != nullptr) continue;

            parent[i][j] = current;
            vis[i][j] = true;
            pair next(i, j);

            if (i == end.first && j == end.second) {
                q.limpiar();
                found = true;
                break;
            }
            q.encolar(next);

        }

        if (found) {
            break;
        }

    }

    pair current(end.first, end.second);
    while (current.first != beg.first || current.second != beg.second) {
        p.agregar(current);
        current = parent[current.first][current.second];
    }
    p.agregar(beg);
    p.reverse();

    return p;

}
path pathfinder::djikstra(pair<int,int> beg, pair<int,int> end) {
    path p;
    bool found = false;
    if (!check_conectividad(beg, end)) {
        return p;
    }

    int filas = mapa -> getFilas();
    int columnas = mapa -> getColumnas();

    pair<int,int> parent[filas][columnas];
    bool vis[filas][columnas];

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            vis[i][j] = false;
        }
    }

    priority_queue pq;
    pair wto(0, beg);
    pq.push(wto);
    vis[beg.first][beg.second] = true;


    while (!pq.empty()) {
        pair<int, pair<int,int>> current = pq.top(); pq.pop();
        for (int t = 0; t < 4; t++) {
            int i = (current.second).first + dx[t];
            int j = (current.second).second + dy[t];
            if (i < 0 || i >= filas || j < 0 || j >= columnas) continue;
            if (vis[i][j]) continue;

            if (mapa -> getCasilla(i,j).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa -> getCasilla(i,j).getTanque() != nullptr) continue;

            vis[i][j] = true;
            parent[i][j] = current.second;

            pair next(current.first + 1, pair(i,j));

            if (i == end.first && j == end.second) {
                found = true;
                break;
            }

            pq.push(next);

        }

        if (found) {
            break;
        }
    }

    pair<int,int> current = end;
    while (current != beg) {
        p.agregar(current);
        current = parent[current.first][current.second];
    }
    p.agregar(beg);
    p.reverse();
    return p;

}