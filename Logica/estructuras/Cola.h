#pragma once

template<typename T>
class Cola {
public:
    Cola() : cabeza(nullptr), fin(nullptr), cantidad(0) {}
    ~Cola() { limpiar(); }
    Cola(const Cola&) = delete;

    Cola& operator=(const Cola& otra) {
        if (this == &otra) return *this;
        limpiar();
        Nodo* actual = otra.cabeza;
        while (actual != nullptr) {
            encolar(actual->dato);
            actual = actual->siguiente;
        }
        return *this;
    }
    Cola& operator=(Cola&& otra) noexcept {
        if (this == &otra) return *this;
        limpiar();
        cabeza        = otra.cabeza;
        fin           = otra.fin;
        cantidad      = otra.cantidad;
        otra.cabeza   = nullptr;
        otra.fin      = nullptr;
        otra.cantidad = 0;
        return *this;
    }

    void encolar(const T& valor) {
        Nodo* nuevo = new Nodo(valor);
        if (fin == nullptr) {
            cabeza = fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
        cantidad++;
    }

    T desencolar() {
        Nodo* tmp = cabeza;
        T valor   = tmp->dato;
        cabeza    = cabeza->siguiente;
        if (cabeza == nullptr) fin = nullptr;
        delete tmp;
        cantidad--;
        return valor;
    }
    
    const T& frente() const { return cabeza->dato; }
    bool vacia()   const { return cantidad == 0; }
    int  size()    const { return cantidad; }
    void limpiar() {
        while (cabeza != nullptr) {
            Nodo* tmp = cabeza;
            cabeza    = cabeza->siguiente;
            delete tmp;
        }
        fin      = nullptr;
        cantidad = 0;
    }

private:
    struct Nodo {
        T     dato;
        Nodo* siguiente;
        explicit Nodo(const T& d) : dato(d), siguiente(nullptr) {}
    };
    Nodo* cabeza;
    Nodo* fin;
    int   cantidad;
};