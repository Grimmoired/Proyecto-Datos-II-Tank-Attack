#include "pathfinder.h"

int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, 1, -1};

pathfinder::pathfinder(Mapa* m)
    : mapa(m), rng(std::random_device{}()) {}

bool pathfinder::check_conectividad(int ib, int jb, int ie, int je) {
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    bool* vis = new bool[filas * columnas];
    for (int i = 0; i < filas * columnas; i++) vis[i] = false;
    Cola<pair<int,int>> q;
    q.encolar(pair<int,int>(ib, jb));
    vis[ib * columnas + jb] = true;

    bool encontrado = false;
    while (!q.vacia() && !encontrado) {
        pair<int,int> current = q.frente();
        q.desencolar();
        for (int t = 0; t < 4; t++) {
            int ni = current.first  + dx[t];
            int nj = current.second + dy[t];
            if (ni < 0 || ni >= filas || nj < 0 || nj >= columnas) continue;
            if (vis[ni * columnas + nj]) continue;
            if (mapa->getCasilla(ni, nj).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa->getCasilla(ni, nj).getTanque() != nullptr) continue;
            vis[ni * columnas + nj] = true;
            if (ni == ie && nj == je) { encontrado = true; break; }
            q.encolar(pair<int,int>(ni, nj));
        }
    }

    delete[] vis;
    return encontrado;
}

path pathfinder::bfs(pair<int,int> beg, pair<int,int> end) {
    path p;
    if (!check_conectividad(beg.first, beg.second, end.first, end.second)) return p;
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    int n = filas * columnas;
    pair<int,int>* parent = new pair<int,int>[n];
    bool* vis = new bool[n];
    for (int i = 0; i < n; i++) {
        vis[i]    = false;
        parent[i] = pair<int,int>(-1, -1);
    }

    Cola<pair<int,int>> q;
    q.encolar(beg);
    vis[beg.first * columnas + beg.second] = true;
    bool encontrado = false;
    while (!q.vacia() && !encontrado) {
        pair<int,int> current = q.frente();
        q.desencolar();
        for (int t = 0; t < 4; t++) {
            int ni = current.first  + dx[t];
            int nj = current.second + dy[t];
            if (ni < 0 || ni >= filas || nj < 0 || nj >= columnas) continue;
            int idx = ni * columnas + nj;
            if (vis[idx]) continue;
            if (mapa->getCasilla(ni, nj).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa->getCasilla(ni, nj).getTanque() != nullptr) continue;
            vis[idx]    = true;
            parent[idx] = current;
            if (ni == end.first && nj == end.second) {
                encontrado = true; break;
            }
            q.encolar(pair<int,int>(ni, nj));
        }
    }

    if (encontrado) {
        pair<int,int> cur = end;
        while (cur.first != beg.first || cur.second != beg.second) {
            p.agregar(cur);
            cur = parent[cur.first * columnas + cur.second];
        }
        p.agregar(beg);
        p.reverse();
    }

    delete[] parent;
    delete[] vis;
    return p;
}

ArregloDinamico<pair<int,int>> pathfinder::casillasEnRango(pair<int,int> origen, int rango) {
    ArregloDinamico<pair<int,int>> resultado;
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    int n = filas * columnas;
    int*  dist = new int[n];
    bool* vis = new bool[n];
    for (int i = 0; i < n; i++) { dist[i] = 999999; vis[i] = false; }
    Cola<pair<int,int>> q;
    int origenIdx = origen.first * columnas + origen.second;
    dist[origenIdx] = 0;
    vis[origenIdx] = true;
    q.encolar(origen);

    while (!q.vacia()) {
        pair<int,int> cur = q.frente();
        q.desencolar();
        int curIdx = cur.first * columnas + cur.second;
        int curDist = dist[curIdx];
        if (curDist >= rango) continue;
        for (int t = 0; t < 4; t++) {
            int ni = cur.first  + dx[t];
            int nj = cur.second + dy[t];
            if (ni < 0 || ni >= filas || nj < 0 || nj >= columnas) continue;
            int nIdx = ni * columnas + nj;
            if (vis[nIdx]) continue;
            if (mapa->getCasilla(ni, nj).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa->getCasilla(ni, nj).getTanque() != nullptr) continue;
            vis[nIdx] = true;
            dist[nIdx] = curDist + 1;
            resultado.agregar(pair<int,int>(ni, nj));
            q.encolar(pair<int,int>(ni, nj));
        }
    }

    delete[] dist;
    delete[] vis;
    return resultado;
}

path pathfinder::dijkstra(pair<int,int> beg, pair<int,int> end) {
    path p;
    if (!check_conectividad(beg.first, beg.second, end.first, end.second)) return p;
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    int n = filas * columnas;
    int* dist   = new int[n];
    pair<int,int>* parent = new pair<int,int>[n];
    bool* vis    = new bool[n];
    for (int i = 0; i < n; i++) {
        dist[i]   = 999999;
        parent[i] = pair<int,int>(-1, -1);
        vis[i]    = false;
    }

    priority_queue pq;
    int inicioIdx = beg.first * columnas + beg.second;
    dist[inicioIdx] = 0;
    pq.push(wto(0, pair<int,int>(beg.first, beg.second)));

    while (!pq.empty()) {
        wto top = pq.top(); pq.pop();
        int f = top.second.first;
        int c = top.second.second;
        int d = top.first;
        int idx = f * columnas + c;
        if (vis[idx]) continue;
        vis[idx] = true;
        if (f == end.first && c == end.second) break;

        for (int t = 0; t < 4; t++) {
            int ni   = f + dx[t];
            int nj   = c + dy[t];
            if (ni < 0 || ni >= filas || nj < 0 || nj >= columnas) continue;
            if (mapa->getCasilla(ni, nj).getTipo() != TipoCasilla::Suelo) continue;
            if (mapa->getCasilla(ni, nj).getTanque() != nullptr) continue;
            int nIdx  = ni * columnas + nj;
            int nDist = dist[idx] + 1;
            if (nDist < dist[nIdx]) {
                dist[nIdx]   = nDist;
                parent[nIdx] = pair<int,int>(f, c);
                pq.push(wto(nDist, pair<int,int>(ni, nj)));
            }
        }
    }

    int endIdx = end.first * columnas + end.second;
    if (parent[endIdx].first != -1) {
        pair<int,int> cur = end;
        while (cur.first != beg.first || cur.second != beg.second) {
            p.agregar(cur);
            cur = parent[cur.first * columnas + cur.second];
        }
        p.agregar(beg);
        p.reverse();
    }

    delete[] dist;
    delete[] parent;
    delete[] vis;
    return p;
}

int pathfinder::heuristica(int f1, int c1, int f2, int c2) { // En este caso se usa manhattan
    int df = f1 - f2; if (df < 0) df = -df;
    int dc = c1 - c2; if (dc < 0) dc = -dc;
    return df + dc;
}

path pathfinder::aStar(pair<int,int> beg, pair<int,int> end) {
    path p;
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    int n = filas * columnas;
    int* gCost   = new int[n];
    pair<int,int>* parent  = new pair<int,int>[n];
    bool* cerrado = new bool[n];
    for (int i = 0; i < n; i++) {
        gCost[i] = 999999;
        parent[i] = pair<int,int>(-1, -1);
        cerrado[i] = false;
    }

    priority_queue pq;
    int inicioIdx = beg.first * columnas + beg.second;
    gCost[inicioIdx] = 0;
    int h0 = heuristica(beg.first, beg.second, end.first, end.second);
    pq.push(wto(h0, pair<int,int>(beg.first, beg.second)));
    bool encontrado = false;
    while (!pq.empty() && !encontrado) {
        wto  top = pq.top(); pq.pop();
        int  f = top.second.first;
        int  c = top.second.second;
        int  idx = f * columnas + c;
        if (cerrado[idx]) continue;
        cerrado[idx] = true;
        if (f == end.first && c == end.second) {
            encontrado = true; break;
        }

        for (int t = 0; t < 4; t++) {
            int ni = f + dx[t];
            int nj = c + dy[t];
            if (ni < 0 || ni >= filas || nj < 0 || nj >= columnas) continue;
            int nIdx = ni * columnas + nj;
            if (cerrado[nIdx]) continue;
            TipoCasilla tipo = mapa->getCasilla(ni, nj).getTipo();
            if (tipo != TipoCasilla::Suelo) continue;
            if (mapa->getCasilla(ni, nj).getTanque() != nullptr &&
                !(ni == end.first && nj == end.second)) continue;

            int nG = gCost[idx] + 1;
            if (nG < gCost[nIdx]) {
                gCost[nIdx]  = nG;
                parent[nIdx] = pair<int,int>(f, c);
                int h = heuristica(ni, nj, end.first, end.second);
                pq.push(wto(nG + h, pair<int,int>(ni, nj)));
            }
        }
    }

    if (encontrado) {
        pair<int,int> cur = end;
        while (cur.first != beg.first || cur.second != beg.second) {
            p.agregar(cur);
            cur = parent[cur.first * columnas + cur.second];
        }
        p.agregar(beg);
        p.reverse();
    }

    delete[] gCost;
    delete[] parent;
    delete[] cerrado;
    return p;
}

bool pathfinder::lineaVista(int f1, int c1, int f2, int c2) {
    int df = f2 - f1; if (df < 0) df = -df;
    int dc = c2 - c1; if (dc < 0) dc = -dc;
    int sf = (f1 < f2) ? 1 : -1;
    int sc = (c1 < c2) ? 1 : -1;
    int err = df - dc;
    int f = f1, c = c1;
    while (!(f == f2 && c == c2)) {
        if (f != f1 || c != c1) {
            TipoCasilla tipo = mapa->getCasilla(f, c).getTipo();
            if (tipo != TipoCasilla::Suelo) return false;
        }
        int e2 = 2 * err;
        if (e2 > -dc) { err -= dc; f += sf; }
        if (e2 <  df) { err += df; c += sc; }
    }
    return true;
}

pair<int,int> pathfinder::avanzarHastaObstaculo(int fOrigen, int cOrigen, int fDir, int cDir) {
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    int fActual = fOrigen;
    int cActual = cOrigen;

    while (true) {
        int fSig = fActual + fDir;
        int cSig = cActual + cDir;
        if (fSig < 0 || fSig >= filas || cSig < 0 || cSig >= columnas) break;
        if (mapa->getCasilla(fSig, cSig).getTipo() != TipoCasilla::Suelo) break;
        if (mapa->getCasilla(fSig, cSig).getTanque() != nullptr) break;
        fActual = fSig;
        cActual = cSig;
    }
    return pair<int,int>(fActual, cActual);
}

pair<int,int> pathfinder::movimientoAleatorio(int fila, int columna, int radio) {
    int filas = mapa->getFilas();
    int columnas = mapa->getColumnas();
    std::uniform_int_distribution<int> randDF(-radio, radio);
    std::uniform_int_distribution<int> randDC(-radio, radio);
    int fDest = fila + randDF(rng);
    int cDest = columna + randDC(rng);
    fDest = fDest < 0 ? 0 : (fDest >= filas    ? filas    - 1 : fDest);
    cDest = cDest < 0 ? 0 : (cDest >= columnas ? columnas - 1 : cDest);

    // Primero intenta linea vista directa hacia el destino
    if (lineaVista(fila, columna, fDest, cDest) &&
        mapa->getCasilla(fDest, cDest).getTipo() == TipoCasilla::Suelo &&
        mapa->getCasilla(fDest, cDest).getTanque() == nullptr)
        return pair<int,int>(fDest, cDest);

    // Si hay un obstaculo elige una posicion intermedia aleatoria en el radio y se mueve hacia ella si es valida, despues vuelve a intentar linea vista
    int fInter = fila + randDF(rng);
    int cInter = columna + randDC(rng);
    fInter = fInter < 0 ? 0 : (fInter >= filas    ? filas    - 1 : fInter);
    cInter = cInter < 0 ? 0 : (cInter >= columnas ? columnas - 1 : cInter);
    if (mapa->getCasilla(fInter, cInter).getTipo() == TipoCasilla::Suelo &&
        mapa->getCasilla(fInter, cInter).getTanque() == nullptr) {
        if (lineaVista(fInter, cInter, fDest, cDest) &&
            mapa->getCasilla(fDest, cDest).getTipo() == TipoCasilla::Suelo &&
            mapa->getCasilla(fDest, cDest).getTanque() == nullptr)
            return pair<int,int>(fDest, cDest);
        int fDir = (fDest > fInter) ? 1 : ((fDest < fInter) ? -1 : 0);
        int cDir = (cDest > cInter) ? 1 : ((cDest < cInter) ? -1 : 0);
        return avanzarHastaObstaculo(fInter, cInter, fDir, cDir);
    }

    // Si no puede solamente avanza hasta donde sea posible
    int fDir = (fDest > fila) ? 1 : ((fDest < fila) ? -1 : 0);
    int cDir = (cDest > columna) ? 1 : ((cDest < columna) ? -1 : 0);
    return avanzarHastaObstaculo(fila, columna, fDir, cDir);
}

path pathfinder::decidirMovimiento(Tanque* tanque, pair<int,int> destino, bool powerupPrecision) {
    pair<int,int> origen(tanque->getFila(), tanque->getColumna());
    float probAlgoritmo;
    bool  usaBFS;
    ColorTanque color = tanque->getColor();
    if (color == ColorTanque::Azul || color == ColorTanque::Verde) {
        probAlgoritmo = powerupPrecision ? 0.90f : 0.90f;
        usaBFS        = true;
    } else {
        probAlgoritmo = powerupPrecision ? 0.90f : 0.90f;
        usaBFS        = false;
    }

    std::uniform_real_distribution<float> prob(0.0f, 1.0f);
    bool usarAlgoritmo = prob(rng) < probAlgoritmo;
    if (usarAlgoritmo) {
        path rutaCompleta = usaBFS ? bfs(origen, destino) : dijkstra(origen, destino);
        int rango = tanque->getRango();
        if (rutaCompleta.size() > (rango + 1)) {
            path rutaRecortada;
            for (int i = 0; i <= rango; i++)
                rutaRecortada.agregar(rutaCompleta[i]);
            return rutaRecortada;
        }
        return rutaCompleta;

    } else {
        pair<int,int> posAleatoria = movimientoAleatorio(tanque->getFila(), tanque->getColumna(), tanque->getRango());
        path ruta = bfs(origen, posAleatoria);
        int rango = tanque->getRango();
        if (ruta.size() > (rango + 1)) {
            path rutaRecortada;
            for (int i = 0; i <= rango; i++)
                rutaRecortada.agregar(ruta[i]);
            return rutaRecortada;
        }
        return ruta;
    }
}