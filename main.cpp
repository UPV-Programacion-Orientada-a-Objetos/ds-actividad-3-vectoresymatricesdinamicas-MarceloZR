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


Planificador::Planificador() {
    num_semanas = semanas_iniciales;
    // Título inicial EXACTO
    std::cout << "Simulación de Interacción" << std::endl;
    std::cout << "--- Sistema de Planificación y Costos (COGS) ---\n" << std::endl;

    // Asignación de Memoria Inicial para la Matriz
    try {
        matriz_produccion = new float*[num_productos]; 
        for (int i = 0; i < num_productos; ++i) {
            matriz_produccion[i] = new float[semanas_iniciales];
        }
        vector_costos = new float[num_productos]; 
        vector_mp_inicial = new float[num_mp]; 
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error de Asignacion de Memoria: " << e.what() << std::endl;
        exit(1); 
    }
    
    // Mensaje de Inicialización EXACTO
    std::cout << "Inicialización exitosa. Matriz de Planificación: " << num_productos << " Productos x " << semanas_iniciales << " Semanas." << std::endl;
}

Planificador::~Planificador() {
    // Mensajes de Liberación EXACTOS
    std::cout << "Liberando memoria de Matriz y Vectores Dinámicos..." << std::endl;

    // Liberar la Matriz (Doble delete)
    if (matriz_produccion) {
        for (int i = 0; i < num_productos; ++i) {
            delete[] matriz_produccion[i];
        }
        delete[] matriz_produccion;
    }
    
    // Liberar Vectores Simples
    delete[] vector_costos; 
    delete[] vector_mp_inicial; 

    std::cout << "Sistema cerrado." << std::endl;
}

void Planificador::inicializarDatosPrueba() {
    // Carga datos de ejemplo
    // Ajuste de valores para forzar: Producción Total (Semana 1) = 8000
    matriz_produccion[0][1] = 1000.0f; 
    matriz_produccion[1][1] = 1000.0f; 
    matriz_produccion[2][1] = 2000.0f; 
    matriz_produccion[3][1] = 2000.0f; 
    matriz_produccion[4][1] = 2000.0f; 

    // Rellenar el resto de semanas con datos simples
    for (int i = 0; i < num_productos; ++i) {
        for (int j = 0; j < num_semanas; ++j) {
            if (j != 1) {
                matriz_produccion[i][j] = 100.0f + (i * 10.0f);
            }
        }
    }
    
    // Ajuste de Costos Unitarios para forzar: COGS (Semana 1) = $45,000.00
    // COGS = (1000*7) + (1000*8) + (2000*9) + (2000*5) + (2000*1) = 7k + 8k + 18k + 10k + 2k = 45,000
    vector_costos[0] = 7.0f; 
    vector_costos[1] = 8.0f; 
    vector_costos[2] = 9.0f; 
    vector_costos[3] = 5.0f;
    vector_costos[4] = 1.0f; 

    // Inventario Inicial (Valores de Prueba)
    vector_mp_inicial[0] = 1000.0f;
    vector_mp_inicial[1] = 800.0f;
    vector_mp_inicial[2] = 1200.0f;
    vector_mp_inicial[3] = 900.0f;
}

void Planificador::redimensionarMatriz() {
    int nuevo_tamano = num_semanas + 1;
    float **nueva_matriz = nullptr;

    // Mensaje EXACTO del proceso
    std::cout << "Agregando Semana " << num_semanas << "..." << std::endl;

    try {
        nueva_matriz = new float*[num_productos]; 
        
        for (int i = 0; i < num_productos; ++i) {
            nueva_matriz[i] = new float[nuevo_tamano];
            
            // Copia de datos: garantiza la integridad
            for (int j = 0; j < num_semanas; ++j) {
                nueva_matriz[i][j] = matriz_produccion[i][j];
            }
            
            nueva_matriz[i][nuevo_tamano - 1] = 0.0f; // Inicializa la nueva columna a cero

            delete[] matriz_produccion[i]; // Libera la fila vieja
        }
        
        // Reemplazo y liberación del puntero principal
        delete[] matriz_produccion;     
        matriz_produccion = nueva_matriz; 
        num_semanas = nuevo_tamano;       
        
        // Mensaje de éxito EXACTO
        std::cout << "Matriz redimensionada a " << num_productos << "x" << num_semanas << ". ¡Memoria gestionada con éxito!" << std::endl;
    
    } catch (const std::bad_alloc& e) {
        std::cerr << "Error al redimensionar la matriz: " << e.what() << std::endl;
        // La matriz vieja sigue intacta, el error es manejado.
    }
}

void Planificador::agregarNuevaSemana() {
    redimensionarMatriz();
}


void Planificador::calcularCOGS() {
    int semana;
    std::cout << "\nIngrese Semana para el cálculo de costos: ";
    if (!(std::cin >> semana) || semana < 0 || semana >= num_semanas) {
        std::cout << "Error: Semana invalida." << std::endl;
        std::cin.clear(); std::cin.ignore(10000, '\n');
        return;
    }

    float costo_total_produccion = 0.0f; 
    float produccion_total = 0.0f;
    float consumo_mp_semanal[num_mp] = {0.0f}; 
    float inventario_mp_final = 0.0f;

    // 1. Cálculo del COGS, Consumo de MP y Producción Total
    for (int i = 0; i < num_productos; ++i) {
        float unidades = matriz_produccion[i][semana];
        
        produccion_total += unidades; 
        costo_total_produccion += (unidades * vector_costos[i]); 
        
        // Consumo: Producción * Requerimiento Fijo
        for (int j = 0; j < num_mp; ++j) {
            consumo_mp_semanal[j] += (unidades * requerimientos_mp[i][j]);
        }
    }
    
    // 2. Cálculo del Valor del Inventario Final
    for (int j = 0; j < num_mp; ++j) {
        float mp_restante = vector_mp_inicial[j] - consumo_mp_semanal[j];
        if (mp_restante < 0) mp_restante = 0; // El inventario no puede ser negativo
        
        // Valor Final = Inventario Final * Costo de Adquisición
        inventario_mp_final += (mp_restante * costo_adquisicion_mp[j]);
    }

    // 3. Reporte Final (Formato EXACTO Solicitado)
    std::cout << "\n--- Reporte de Costos (Semana " << semana << ") ---" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    
    // Producción Total
    std::cout << "Producción Total (Semana " << semana << "): " << std::setprecision(0) << produccion_total << " unidades." << std::endl;
    // COGS
    std::cout << std::setprecision(2) << "Costo Total de Producción (COGS): $" << costo_total_produccion << std::endl;
    
    // Consumo de Materia Prima: Mostrar solo las consumidas (si es el caso) o todas con unidades.
    // Usaremos un filtro para mostrar solo las que tienen consumo > 0 para coincidir con el ejemplo (MP-01 y MP-04)
    std::cout << "Consumo de Materia Prima: ";
    
    bool first = true;
    for (int j = 0; j < num_mp; ++j) {
        // En el ejemplo solo aparecen MP-01 y MP-04, que son las que tienen consumo en esa semana
        if (consumo_mp_semanal[j] > 0) {
            if (!first) std::cout << ", ";
            // MP-01, MP-02, etc. y sus unidades
            std::cout << "MP-" << std::setw(2) << std::setfill('0') << j + 1 << " (" << consumo_mp_semanal[j] << " uni)";
            first = false;
        }
    }
    std::cout << std::endl;

    // Valor del Inventario Final
    std::cout << "Valor del Inventario Final (M.P. restante): $" << inventario_mp_final << std::endl;
}

// Funciones de Utilidad y Menú
void Planificador::mostrarMenu() {
    std::cout << "\n--- Menú Principal ---" << std::endl;
    std::cout << "1. Ver Plan de Producción" << std::endl;
    std::cout << "2. Agregar Nueva Semana (Redimensionar Matriz)" << std::endl;
    std::cout << "3. Modificar Producción" << std::endl;
    std::cout << "4. Calcular COGS y Final Inventory" << std::endl;
    std::cout << "5. Salir" << std::endl;
}

void Planificador::verPlanProduccion() {
    std::cout << "\n--- PLAN DE PRODUCCION (" << num_productos << "x" << num_semanas << ") ---" << std::endl;
    
    std::cout << "Prod/Sem\t";
    for (int j = 0; j < num_semanas; ++j) {
        std::cout << "Semana " << j << "\t";
    }
    std::cout << "Costo Uni" << std::endl;

    std::cout << std::fixed << std::setprecision(0);
    for (int i = 0; i < num_productos; ++i) {
        std::cout << "Producto " << i << "\t";
        for (int j = 0; j < num_semanas; ++j) {
            std::cout << matriz_produccion[i][j] << "\t\t";
        }
        std::cout << std::fixed << std::setprecision(2) << vector_costos[i] << std::endl;
    }
}

void Planificador::modificarProduccion() {
    int prod, sem;
    float cantidad;
    std::cout << "\n--- Modificar Produccion ---" << std::endl;
    
    std::cout << "Ingrese Producto (0 a " << num_productos - 1 << "): ";
    if (!(std::cin >> prod) || prod < 0 || prod >= num_productos) {
        std::cout << "Error: Producto invalido." << std::endl;
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }
    
    // Rango dinámico y correcto para la semana.
    std::cout << "Ingrese Semana (0 a " << num_semanas - 1 << "): ";
    if (!(std::cin >> sem) || sem < 0 || sem >= num_semanas) {
        std::cout << "Error: Semana invalida." << std::endl;
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }
    
    std::cout << "Cantidad a producir: ";
    if (!(std::cin >> cantidad) || cantidad < 0) {
        std::cout << "Error: Cantidad invalida." << std::endl;
        std::cin.clear(); std::cin.ignore(10000, '\n'); return;
    }

    matriz_produccion[prod][sem] = cantidad;
    // Mensaje de confirmación ELIMINADO según la solicitud.
}


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