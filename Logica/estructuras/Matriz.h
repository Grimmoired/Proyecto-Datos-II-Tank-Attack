#pragma once

template<typename T>
class Matriz {
public:
    Matriz() : filas(0), columnas(0), datos(nullptr) {}
    Matriz(int filas, int columnas, T valorInicial = T())
        : filas(filas), columnas(columnas), datos(nullptr) {
        alojar();
        rellenar(valorInicial);
    }
    ~Matriz() { liberar(); }

    Matriz(const Matriz& otra)
        : filas(otra.filas), columnas(otra.columnas), datos(nullptr) {
        alojar();
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                datos[i][j] = otra.datos[i][j];
    }

    Matriz& operator=(const Matriz& otra) {
        if (this == &otra) return *this;
        liberar();
        filas    = otra.filas;
        columnas = otra.columnas;
        alojar();
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                datos[i][j] = otra.datos[i][j];
        return *this;
    }

    T* operator[](int fila) { return datos[fila]; }
    const T* operator[](int fila) const { return datos[fila]; }
    void rellenar(T valor) {
        for (int i = 0; i < filas; i++)
            for (int j = 0; j < columnas; j++)
                datos[i][j] = valor;
    }

    int getFilas() const { return filas; }
    int getColumnas() const { return columnas; }
private:
    int  filas;
    int  columnas;
    T**  datos;

    void alojar() {
        datos = new T*[filas];
        for (int i = 0; i < filas; i++)
            datos[i] = new T[columnas];
    }

    void liberar() {
        if (datos) {
            for (int i = 0; i < filas; i++)
                delete[] datos[i];
            delete[] datos;
            datos = nullptr;
        }
    }
    bool esPar(int n) {
        if (n % 2 == 1) return true;
        if (n % 2 != 1) return false;
    }
};

