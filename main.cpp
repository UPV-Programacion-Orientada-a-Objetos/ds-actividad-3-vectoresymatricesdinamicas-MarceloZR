#include <iostream>
#include <iomanip> 
#include <limits> 

// Constantes globales
const int num_productos = 5;      
const int num_mp = 4;             
const int semanas_iniciales = 4;  
// Costos de Adquisición de Materia Prima: MP-01, MP-02, MP-03, MP-04
const float costo_adquisicion_mp[num_mp] = {5.0f, 3.5f, 7.0f, 4.0f};

class Planificador {
private:
    // Punteros y Estructuras Dinámicas
    float **matriz_produccion; 
    float *vector_costos;      
    float *vector_mp_inicial;  
    int num_semanas;           

    // Estructura Estática: Requerimientos de Materia Prima por Producto (5x4)
    // Producto x MP
    int requerimientos_mp[num_productos][num_mp] = {
        {2, 1, 0, 3}, // P0: 2 MP-01, 1 MP-02, 0 MP-03, 3 MP-04
        {1, 2, 1, 0}, // P1: 1 MP-01, 2 MP-02, 1 MP-03, 0 MP-04
        {0, 0, 3, 2}, // P2: ...
        {3, 1, 1, 1}, 
        {1, 3, 0, 1}  
    };

    void redimensionarMatriz();

public:
    Planificador();
    ~Planificador(); 

    void mostrarMenu();
    void inicializarDatosPrueba();
    void verPlanProduccion();
    void agregarNuevaSemana();
    void modificarProduccion();
    void calcularCOGS();
};


int main() {
    Planificador sistema;
    sistema.inicializarDatosPrueba(); 
    
    int opcion = 0;
    while (opcion != 5) {
        sistema.mostrarMenu();
        std::cout << "\nOpción seleccionada: ";
        if (!(std::cin >> opcion)) { 
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            opcion = 0;
        }

        switch (opcion) {
            case 1: sistema.verPlanProduccion(); break;
            case 2: sistema.agregarNuevaSemana(); break;
            case 3: sistema.modificarProduccion(); break;
            case 4: sistema.calcularCOGS(); break;
            case 5: break;
            default: std::cout << "Opción no valida." << std::endl;
        }
    }
    
    return 0;
}