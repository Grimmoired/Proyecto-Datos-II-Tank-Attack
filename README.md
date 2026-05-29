# Proyecto-Datos-II-Tank-Attack

## Requisitos del Sistema

### Sistema Operativo
- Windows 10 / Windows 11 (64-bit)

### Herramientas de Desarrollo
| Herramienta | Versión recomendada |
|---|---|
| CLion | 2026.1 o superior |
| CMake | 3.20 o superior |
| MinGW-w64 (GCC) | 13.1.0 (incluido en CLion 2026.1) |

### Dependencias Externas
| Librería | Versión | Descarga |
|---|---|---|
| SFML | 2.6.2 | [sfml-dev.org](https://www.sfml-dev.org/download/sfml/2.6.2/) |

> **Importante:** Hay que descargar especificamente la versión **GCC 13.1.0 MinGW (SEH) - 64-bit** de SFML para garantizar compatibilidad con el compilador.

## Instalación y Configuración

### 1. Clonar el repositorio

```bash
git clone https://github.com/tu-usuario/Proyecto-Datos-II-Tank-Attack.git
cd Proyecto-Datos-II-Tank-Attack
```

### 2. Instalar SFML

1. Descargue SFML 2.6.2 desde [sfml-dev.org](https://www.sfml-dev.org/download/sfml/2.6.2/)
2. Seleccione **GCC 13.1.0 MinGW (SEH) - 64-bit**
3. Extraiga el contenido en una ruta sin espacios, por ejemplo: C:/Libs/SFML-2.6.2/

### 3. Configurar CMakeLists.txt

Abra `CMakeLists.txt` y verifica que la ruta de SFML apunte a donde fue instalado:

```cmake
set(SFML_DIR "C:/Libs/SFML-2.6.2/lib/cmake/SFML")
```
### 4. Abrir el proyecto en CLion (Puede varir para otros IDE)

1. Abra CLion
2. Seleccione **File → Open** y navegue a la carpeta del proyecto
3. CLion detectará automáticamente el `CMakeLists.txt`
4. En la barra superior seleccione la configuración **TankAttack** y presione el boton de run para compilar y ejecutar

---

## Compilar el ejecutable (.exe)

Para generar un `.exe` que no dependa de la estructura de archivos ni dependencias de este repositorio se siguen estos pasos

### Paso 1 — Compilar en modo Release

En CLion, cambie la configuración de compilación de **Debug** a **Release**:

1. Vaya a **Edit Configurations** (menú desplegable junto al botón de run)
2. En **CMake options** seleccione o agrega el perfil **Release**
3. En la sección de CMake profiles, asegurese de tener: -DCMAKE_BUILD_TYPE=Release
4. Haga clic en **Build → Build Project** (Ctrl+F9), el ejecutable se generará en: cmake-build-release/TankAttack.exe

### Paso 2 — Copiar las DLLs de SFML

El `.exe` necesita las DLLs de SFML junto a él para ejecutarse, copielas desde su instalación de SFML:
C:/Libs/SFML-2.6.2/bin/sfml-graphics-2.dll
C:/Libs/SFML-2.6.2/bin/sfml-window-2.dll
C:/Libs/SFML-2.6.2/bin/sfml-audio-2.dll
C:/Libs/SFML-2.6.2/bin/sfml-system-2.dll

Peguelas en la misma carpeta donde está el `TankAttack.exe`.

### Paso 3 — Copiar las DLLs de MinGW

También se necesitan las DLLs del runtime de GCC, se encuentran en la carpeta `bin` de MinGW incluido en CLion (la ruta exacta puede variar):
C:/Program Files/JetBrains/CLion 2026.1/bin/mingw/bin/libgcc_s_seh-1.dll
C:/Program Files/JetBrains/CLion 2026.1/bin/mingw/bin/libstdc++-6.dll
C:/Program Files/JetBrains/CLion 2026.1/bin/mingw/bin/libwinpthread-1.dll

Peguelas también junto al `.exe`.

### Paso 4 — Copiar los assets

Copie la carpeta `assets/` completa al mismo nivel que el `.exe`; el juego deberia abrirse sin mayor problema

---

## Controles

| Acción | Teclado | Mouse |
|---|---|---|
| Modo mover | `A` | Botón **MOVER** en HUD |
| Modo atacar | `S` | Botón **ATACAR** en HUD |
| Usar power-up | `LShift` | Botón **USAR POWER-UP** en HUD |
| Finalizar turno | `Enter` | Botón **FIN TURN** en HUD |
| Volver al menú | `Escape` | — |
| Seleccionar casilla / objetivo | — | Click izquierdo / derecho |

---

##  Autores

- **Bryan David Abarca Quirós**
- **Jorge Pablo Porras Alvarado**

Curso CE 2103 — Algoritmos y Estructuras de Datos II  
Instituto Tecnológico de Costa Rica — I Semestre 2026
