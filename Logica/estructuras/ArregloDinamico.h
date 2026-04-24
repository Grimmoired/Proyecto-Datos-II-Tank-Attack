#pragma once

template<typename T>
class ArregloDinamico {
public:
    ArregloDinamico()
        : datos(nullptr), cantidad(0), capacidad(0) {}

    explicit ArregloDinamico(int capacidadInicial)
        : cantidad(0), capacidad(capacidadInicial) {
        datos = new T[capacidad];
    }

    // Destructor
    ~ArregloDinamico() {
        delete[] datos;
    }

    ArregloDinamico(const ArregloDinamico&) = delete;
    ArregloDinamico& operator=(const ArregloDinamico&) = delete;

    ArregloDinamico(ArregloDinamico&& otro) noexcept
        : datos(otro.datos),
          cantidad(otro.cantidad),
          capacidad(otro.capacidad) {

        otro.datos = nullptr;
        otro.cantidad = 0;
        otro.capacidad = 0;
    }

    ArregloDinamico& operator=(ArregloDinamico&& otro) noexcept {
        if (this != &otro) {
            delete[] datos;

            datos = otro.datos;
            cantidad = otro.cantidad;
            capacidad = otro.capacidad;

            otro.datos = nullptr;
            otro.cantidad = 0;
            otro.capacidad = 0;
        }
        return *this;
    }

    void agregar(const T& valor) {
        if (cantidad == capacidad)
            expandir();
        datos[cantidad++] = valor;
    }

    T& operator[](int i) {
        return datos[i];
    }

    const T& operator[](int i) const {
        return datos[i];
    }

    int size() const {
        return cantidad;
    }

    bool vacio() const {
        return cantidad == 0;
    }

    void limpiar() {
        cantidad = 0;
    }

private:
    T* datos;
    int cantidad;
    int capacidad;

    void expandir() {
        capacidad = (capacidad == 0) ? 8 : capacidad * 2;

        T* nuevo = new T[capacidad];

        for (int i = 0; i < cantidad; i++)
            nuevo[i] = datos[i];

        delete[] datos;
        datos = nuevo;
    }
};