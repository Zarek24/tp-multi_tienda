#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// ==================== ESTRUCTURAS ====================
struct Tienda {
    string codigo;
    string nombre;
    string direccion;
    string distrito;
    string encargado;
    bool estado; // true = Activa, false = Suspendida
};

struct Producto {
    string codigo;
    string nombre;
    double precio;
    int stock;
};

struct Empleado {
    string codigo;
    string nombre;
    string puesto;
    double sueldo;
    string tiendaCodigo;
};

// ==================== PROTOTIPOS DE FUNCIONES ====================
bool soloEspacios(string texto);
bool textoValido(string texto);
bool leerTexto(string mensaje, string& valor);
bool leerEntero(string mensaje, int& valor, int minimo, int maximo);
bool leerMonto(string mensaje, double& valor);

// Gestión de Tiendas
bool existeTienda(Tienda* tiendas, int tam, string codigo);
void registrarTienda(Tienda*& tiendas, int& tam);
void mostrarTiendas(Tienda* tiendas, int tam);
void modificarTienda(Tienda* tiendas, int tam);

// Matriz de Ventas
void crearMatrizVentas(double**& matriz, int filasAnt, int colsAnt, int nuevasFilas, int nuevasCols);
void registrarVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias);
void mostrarMatrizVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias);

// Gestión de Productos
bool existeProducto(Producto* productos, int tam, string codigo);
void registrarProducto(Producto*& productos, int& tam);
void mostrarProductos(Producto* productos, int tam);

// Gestión de Empleados
void registrarEmpleado(Empleado*& empleados, int& tam, Tienda* tiendas, int numTiendas);
void mostrarEmpleados(Empleado* empleados, int tam);

// Cálculos del Sistema
void calcularVentasPorTienda(double** matriz, Tienda* tiendas, int numTiendas, int numDias);
void calcularPromedios(double** matriz, int numTiendas, int numDias);
void ventaMaximaMinima(double** matriz, Tienda* tiendas, int numTiendas, int numDias);
void totalGeneralYParticipacion(double** matriz, Tienda* tiendas, int numTiendas, int numDias);

// Reportes del Sistema
void reportesSistema(double** matriz, Tienda* tiendas, int numTiendas, int numDias);

// Gestión de Archivos
void guardarDatos(Tienda* tiendas, int numTiendas, double** matriz, int numDias, Producto* productos, int numProductos, Empleado* empleados, int numEmpleados);
void cargarDatos(Tienda*& tiendas, int& numTiendas, double**& matriz, int& numDias, Producto*& productos, int& numProductos, Empleado*& empleados, int& numEmpleados);

// Submenus
void menuTiendas(Tienda*& tiendas, int& numTiendas, double**& matrizVentas, int numDias);
void menuVentas(Tienda* tiendas, int numTiendas, double**& matrizVentas, int& numDias);
void menuReportes(Tienda* tiendas, int numTiendas, double** matrizVentas, int numDias);
void menuProductos(Producto*& productos, int& numProductos);
void menuEmpleados(Empleado*& empleados, int& numEmpleados, Tienda* tiendas, int numTiendas);

// ==================== FUNCION PRINCIPAL ====================
int main() {
    cout.setf(ios::fixed);
    cout.precision(2);
    Tienda* tiendas = nullptr;
    int numTiendas = 0;

    Producto* productos = nullptr;
    int numProductos = 0;

    Empleado* empleados = nullptr;
    int numEmpleados = 0;

    double** matrizVentas = nullptr;
    int numDias = 0; // Se inicializa al crear la matriz

    int opcion;

    do {
        cout << "\n============================================\n";
        cout << "           SISTEMA MULTITIENDA\n";
        cout << "============================================\n";
        cout << "1. Gestionar tiendas\n";
        cout << "2. Gestionar ventas\n";
        cout << "3. Consultar reportes\n";
        cout << "4. Gestionar productos\n";
        cout << "5. Gestionar empleados\n";
        cout << "6. Guardar datos\n";
        cout << "7. Cargar datos\n";
        cout << "0. Salir\n";
        if (leerEntero("Seleccione una opcion: ", opcion, 0, 7) == false) break;
        switch (opcion) {
        case 1:
            menuTiendas(tiendas, numTiendas, matrizVentas, numDias);
            break;
        case 2:
            menuVentas(tiendas, numTiendas, matrizVentas, numDias);
            break;
        case 3:
            menuReportes(tiendas, numTiendas, matrizVentas, numDias);
            break;
        case 4:
            menuProductos(productos, numProductos);
            break;
        case 5:
            menuEmpleados(empleados, numEmpleados, tiendas, numTiendas);
            break;
        case 6:
            guardarDatos(tiendas, numTiendas, matrizVentas, numDias, productos, numProductos, empleados, numEmpleados);
            break;
        case 7:
            cargarDatos(tiendas, numTiendas, matrizVentas, numDias, productos, numProductos, empleados, numEmpleados);
            break;
        case 0:
            cout << "Saliendo del sistema...\n";
            break;
        }
    } while (opcion != 0 && cin.eof() == false);

    if (cin.eof()) cout << "\nEntrada finalizada.\n";

    // Liberar memoria principal al salir
    delete[] tiendas;
    delete[] productos;
    delete[] empleados;
    if (matrizVentas != nullptr) {
        for (int i = 0; i < numTiendas; i++) delete[] matrizVentas[i];
        delete[] matrizVentas;
    }

    return 0;
}

// ==================== DESARROLLO DE MÓDULOS ====================

// Cada submenu repite sus opciones hasta elegir 0 para volver.
void menuTiendas(Tienda*& tiendas, int& numTiendas, double**& matrizVentas, int numDias) {
    int opcion;
    do {
        cout << "\n--- GESTION DE TIENDAS ---\n";
        cout << "1. Registrar tienda\n2. Mostrar tiendas\n3. Modificar tienda\n0. Volver\n";
        if (leerEntero("Seleccione una opcion: ", opcion, 0, 3) == false) return;
        switch (opcion) {
        case 1: {
            int tiendasAntes = numTiendas;
            registrarTienda(tiendas, numTiendas);
            // Si existe una matriz, agregar la fila de la tienda nueva.
            if (numTiendas > tiendasAntes && matrizVentas != nullptr) {
                crearMatrizVentas(matrizVentas, tiendasAntes, numDias, numTiendas, numDias);
            }
            break;
        }
        case 2:
            mostrarTiendas(tiendas, numTiendas);
            break;
        case 3:
            modificarTienda(tiendas, numTiendas);
            break;
        }
    } while (opcion != 0 && cin.eof() == false);
}

void menuVentas(Tienda* tiendas, int numTiendas, double**& matrizVentas, int& numDias) {
    int opcion;
    do {
        cout << "\n--- GESTION DE VENTAS ---\n";
        cout << "1. Configurar dias y registrar ventas\n2. Consultar ventas por tienda y dia\n0. Volver\n";
        if (leerEntero("Seleccione una opcion: ", opcion, 0, 2) == false) return;
        switch (opcion) {
        case 1: {
            if (numTiendas == 0) {
                cout << "[Error] Primero debe registrar al menos una tienda.\n";
                break;
            }
            int nuevosDias;
            if (leerEntero("Numero de dias a evaluar: ", nuevosDias, 1, 366) == false) return;
            crearMatrizVentas(matrizVentas, numTiendas, numDias, numTiendas, nuevosDias);
            numDias = nuevosDias;
            registrarVentas(matrizVentas, tiendas, numTiendas, numDias);
            break;
        }
        case 2:
            mostrarMatrizVentas(matrizVentas, tiendas, numTiendas, numDias);
            break;
        }
    } while (opcion != 0 && cin.eof() == false);
}

void menuReportes(Tienda* tiendas, int numTiendas, double** matrizVentas, int numDias) {
    int opcion;
    do {
        cout << "\n--- REPORTES ---\n";
        cout << "1. Ventas por tienda y promedios\n";
        cout << "2. Venta maxima y minima\n";
        cout << "3. Total general y participacion\n";
        cout << "4. Mayor y menor venta y ranking\n0. Volver\n";
        if (leerEntero("Seleccione una opcion: ", opcion, 0, 4) == false) return;
        if (opcion != 0 && (matrizVentas == nullptr || numDias == 0)) {
            cout << "[Aviso] Primero debe registrar ventas.\n";
        } else {
            switch (opcion) {
            case 1:
                calcularVentasPorTienda(matrizVentas, tiendas, numTiendas, numDias);
                calcularPromedios(matrizVentas, numTiendas, numDias);
                break;
            case 2:
                ventaMaximaMinima(matrizVentas, tiendas, numTiendas, numDias);
                break;
            case 3:
                totalGeneralYParticipacion(matrizVentas, tiendas, numTiendas, numDias);
                break;
            case 4:
                reportesSistema(matrizVentas, tiendas, numTiendas, numDias);
                break;
            }
        }
    } while (opcion != 0 && cin.eof() == false);
}

void menuProductos(Producto*& productos, int& numProductos) {
    int opcion;
    do {
        cout << "\n--- GESTION DE PRODUCTOS ---\n";
        cout << "1. Registrar producto\n2. Mostrar productos\n0. Volver\n";
        if (leerEntero("Seleccione una opcion: ", opcion, 0, 2) == false) return;
        switch (opcion) {
        case 1:
            registrarProducto(productos, numProductos);
            break;
        case 2:
            mostrarProductos(productos, numProductos);
            break;
        }
    } while (opcion != 0 && cin.eof() == false);
}

void menuEmpleados(Empleado*& empleados, int& numEmpleados, Tienda* tiendas, int numTiendas) {
    int opcion;
    do {
        cout << "\n--- GESTION DE EMPLEADOS ---\n";
        cout << "1. Registrar empleado\n2. Mostrar empleados\n0. Volver\n";
        if (leerEntero("Seleccione una opcion: ", opcion, 0, 2) == false) return;
        switch (opcion) {
        case 1:
            registrarEmpleado(empleados, numEmpleados, tiendas, numTiendas);
            break;
        case 2:
            mostrarEmpleados(empleados, numEmpleados);
            break;
        }
    } while (opcion != 0 && cin.eof() == false);
}


// Funciones de lectura y validacion.
// Se usan recorridos para revisar los caracteres de cada texto.
bool soloEspacios(string texto) {
    int cantidad = texto.length();
    for (int i = 0; i < cantidad; i++) {
        if (texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\r') {
            return false;
        }
    }
    return true;
}

bool textoValido(string texto) {
    bool tieneContenido = false;
    int cantidad = texto.length();
    for (int i = 0; i < cantidad; i++) {
        // La barra separa campos en los archivos; no puede ser parte de un dato.
        if (texto[i] == '|' || texto[i] == '\n') {
            return false;
        }
        if (texto[i] != ' ' && texto[i] != '\t' && texto[i] != '\r') {
            tieneContenido = true;
        }
    }
    return tieneContenido;
}

bool leerTexto(string mensaje, string& valor) {
    bool valido = false;
    do {
        cout << mensaje;
        getline(cin, valor);
        if (cin.eof()) return false;
        valido = textoValido(valor);
        if (valido == false) {
            cout << "[Error] Ingrese texto no vacio y sin el caracter |.\n";
        }
    } while (valido == false);
    return true;
}

bool leerEntero(string mensaje, int& valor, int minimo, int maximo) {
    bool valido = false;
    string resto;
    do {
        cout << mensaje;
        cin >> valor;
        if (cin.eof()) return false;
        // fail detecta una lectura incorrecta; clear permite volver a leer.
        if (cin.fail()) {
            cin.clear();
            getline(cin, resto);
        } else {
            getline(cin, resto);
            if (soloEspacios(resto) && valor >= minimo && valor <= maximo) {
                valido = true;
            }
        }
        if (valido == false) {
            cout << "[Error] Ingrese un entero entre " << minimo << " y " << maximo << ".\n";
        }
    } while (valido == false);
    return true;
}

bool leerMonto(string mensaje, double& valor) {
    bool valido = false;
    string resto;
    do {
        cout << mensaje;
        cin >> valor;
        if (cin.eof()) return false;
        if (cin.fail()) {
            cin.clear();
            getline(cin, resto);
        } else {
            getline(cin, resto);
            if (soloEspacios(resto) && valor >= 0 && valor <= 1000000000000.0) {
                valido = true;
            }
        }
        if (valido == false) {
            cout << "[Error] Ingrese un monto entre 0 y 1000000000000.\n";
        }
    } while (valido == false);
    return true;
}


// --- GESTIÓN DE TIENDAS ---
bool existeTienda(Tienda* tiendas, int tam, string codigo) {
    for (int i = 0; i < tam; i++) {
        if (tiendas[i].codigo == codigo) return true;
    }
    return false;
}

void registrarTienda(Tienda*& tiendas, int& tam) {
    if (tam >= 1000) { cout << "[Error] Limite de 1000 tiendas.\n"; return; }
    Tienda nueva;
    cout << "\n--- REGISTRAR TIENDA ---\n";
    if (leerTexto("Codigo: ", nueva.codigo) == false) return;
    if (existeTienda(tiendas, tam, nueva.codigo)) {
        cout << "[Error] El codigo ya existe.\n";
        return;
    }
    if (leerTexto("Tienda: ", nueva.nombre) == false) return;
    if (leerTexto("Direccion: ", nueva.direccion) == false) return;
    if (leerTexto("Distrito: ", nueva.distrito) == false) return;
    if (leerTexto("Encargado: ", nueva.encargado) == false) return;
    nueva.estado = true;
    Tienda* aux = new Tienda[tam + 1];
    for (int i = 0; i < tam; i++) aux[i] = tiendas[i];
    aux[tam] = nueva;
    delete[] tiendas;
    tiendas = aux;
    tam++;
    cout << "[Exito] Tienda registrada.\n";
}

void mostrarTiendas(Tienda* tiendas, int tam) {
    if (tam == 0) { cout << "No hay tiendas registradas.\n"; return; }
    cout << "\n--- LISTA DE TIENDAS ---\n";
    for (int i = 0; i < tam; i++) {
        string estado = "Suspendida";
        if (tiendas[i].estado) {
            estado = "Activa";
        }
        cout << "Codigo: " << tiendas[i].codigo << " | Tienda: " << tiendas[i].nombre
            << " | Direccion: " << tiendas[i].direccion << " (" << tiendas[i].distrito
            << ") | Encargado: " << tiendas[i].encargado
            << " | Estado: " << estado << "\n";
    }
}

void modificarTienda(Tienda* tiendas, int tam) {
    if (tam == 0) { cout << "No hay tiendas.\n"; return; }
    string cod;
    if (leerTexto("Codigo de tienda a modificar: ", cod) == false) return;
    int idx = -1;
    for (int i = 0; i < tam; i++) {
        if (tiendas[i].codigo == cod) { idx = i; break; }
    }
    if (idx == -1) { cout << "[Error] Tienda no encontrada.\n"; return; }
    int opc;
    cout << "\n--- MODIFICAR TIENDA ---\n";
    cout << "Tienda: " << tiendas[idx].nombre << "\n";
    cout << "1. Cambiar nombre de la tienda\n";
    cout << "2. Cambiar encargado\n";
    cout << "3. Cambiar direccion\n";
    cout << "4. Cambiar distrito\n";
    if (tiendas[idx].estado) {
        cout << "5. Suspender tienda\n";
    } else {
        cout << "5. Reactivar tienda\n";
    }
    cout << "0. Volver\n";
    if (leerEntero("Opcion: ", opc, 0, 5) == false) return;
    Tienda modificada = tiendas[idx];
    switch (opc) {
    case 1:
        if (leerTexto("Nueva tienda: ", modificada.nombre) == false) return;
        break;
    case 2:
        if (leerTexto("Nuevo encargado: ", modificada.encargado) == false) return;
        break;
    case 3:
        if (leerTexto("Nueva direccion: ", modificada.direccion) == false) return;
        break;
    case 4:
        if (leerTexto("Nuevo distrito: ", modificada.distrito) == false) return;
        break;
    case 5:
        if (modificada.estado) {
            modificada.estado = false;
        } else {
            modificada.estado = true;
        }
        break;
    case 0:
        return;
    }
    tiendas[idx] = modificada;
    cout << "[Exito] Tienda actualizada.\n";
}

// --- MATRIZ DE VENTAS ---
void crearMatrizVentas(double**& matriz, int filasAnt, int colsAnt, int nuevasFilas, int nuevasCols) {
    double** nuevaMatriz = new double* [nuevasFilas];
    for (int i = 0; i < nuevasFilas; i++) {
        nuevaMatriz[i] = new double[nuevasCols];
        for (int j = 0; j < nuevasCols; j++) {
            if (matriz != nullptr && i < filasAnt && j < colsAnt) {
                nuevaMatriz[i][j] = matriz[i][j]; // Mantener datos previos si existían
            }
            else {
                nuevaMatriz[i][j] = 0.0;
            }
        }
    }
    if (matriz != nullptr) {
        for (int i = 0; i < filasAnt; i++) delete[] matriz[i];
        delete[] matriz;
    }
    matriz = nuevaMatriz;
}

void registrarVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    bool hayActiva = false;
    for (int i = 0; i < numTiendas; i++) if (tiendas[i].estado) hayActiva = true;
    if (!hayActiva) { cout << "[Error] No hay tiendas activas.\n"; return; }
    int opc;
    do {
        cout << "\n--- REGISTRAR VENTAS ---\n";
        mostrarTiendas(tiendas, numTiendas);
        string codigo;
        if (leerTexto("Codigo de tienda: ", codigo) == false) return;
        int indice = -1;
        for (int i = 0; i < numTiendas; i++) {
            if (tiendas[i].codigo == codigo) { indice = i; break; }
        }
        if (indice == -1) {
            cout << "[Error] Tienda no encontrada.\n";
        } else if (!tiendas[indice].estado) {
            cout << "[Error] La tienda esta suspendida.\n";
        } else {
            int dia;
            double monto;
            cout << "Dias disponibles: 1 a " << numDias << "\n";
            if (leerEntero("Dia: ", dia, 1, numDias) == false) return;
            if (leerMonto("Monto total vendido en ese dia (reemplaza el anterior): ", monto) == false) return;
            matriz[indice][dia - 1] = monto;
            cout << "[Exito] Venta registrada.\n";
        }
        if (leerEntero("Desea registrar otra venta? (1: Si / 0: No): ", opc, 0, 1) == false) return;
    } while (opc == 1);
}

void mostrarMatrizVentas(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    if (matriz == nullptr || numDias == 0) { cout << "Matriz no inicializada.\n"; return; }
    cout << "\n--- MATRIZ DE VENTAS ---\n" << "Tienda\\Dia\t";
    for (int d = 1; d <= numDias; d++) cout << "D" << d << "\t";
    cout << "\n";
    for (int i = 0; i < numTiendas; i++) {
        cout << tiendas[i].codigo << "\t\t";
        for (int j = 0; j < numDias; j++) {
            cout << matriz[i][j] << "\t";
        }
        cout << "\n";
    }
}

// --- GESTIÓN DE PRODUCTOS ---
bool existeProducto(Producto* productos, int tam, string codigo) {
    for (int i = 0; i < tam; i++) if (productos[i].codigo == codigo) return true;
    return false;
}

void registrarProducto(Producto*& productos, int& tam) {
    if (tam >= 100000) { cout << "[Error] Limite de productos alcanzado.\n"; return; }
    Producto p;
    cout << "\n--- REGISTRAR PRODUCTO ---\n";
    if (leerTexto("Codigo: ", p.codigo) == false) return;
    if (existeProducto(productos, tam, p.codigo)) { cout << "[Error] Codigo duplicado.\n"; return; }
    if (leerTexto("Nombre: ", p.nombre) == false) return;
    if (leerMonto("Precio: ", p.precio) == false) return;
    if (leerEntero("Stock: ", p.stock, 0, 1000000000) == false) return;
    Producto* aux = new Producto[tam + 1];
    for (int i = 0; i < tam; i++) aux[i] = productos[i];
    aux[tam] = p;
    delete[] productos;
    productos = aux;
    tam++;
    cout << "[Exito] Producto registrado.\n";
}

void mostrarProductos(Producto* productos, int tam) {
    if (tam == 0) { cout << "No hay productos.\n"; return; }
    cout << "\n--- LISTA DE PRODUCTOS ---\n";
    for (int i = 0; i < tam; i++) {
        cout << "Cod: " << productos[i].codigo << " | Nombre: " << productos[i].nombre
            << " | Precio: S/ " << productos[i].precio << " | Stock: " << productos[i].stock << "\n";
    }
}

// --- GESTIÓN DE EMPLEADOS ---
void registrarEmpleado(Empleado*& empleados, int& tam, Tienda* tiendas, int numTiendas) {
    if (numTiendas == 0) { cout << "Debe registrar tiendas primero.\n"; return; }
    if (tam >= 100000) { cout << "[Error] Limite de empleados alcanzado.\n"; return; }
    Empleado e;
    cout << "\n--- REGISTRAR EMPLEADO ---\n";
    if (leerTexto("Codigo: ", e.codigo) == false) return;
    for (int i = 0; i < tam; i++) {
        if (empleados[i].codigo == e.codigo) { cout << "[Error] Codigo duplicado.\n"; return; }
    }
    if (leerTexto("Nombre: ", e.nombre) == false) return;
    if (leerTexto("Puesto: ", e.puesto) == false) return;
    if (leerMonto("Sueldo: ", e.sueldo) == false) return;
    mostrarTiendas(tiendas, numTiendas);
    if (leerTexto("Codigo de tienda asignada: ", e.tiendaCodigo) == false) return;
    if (!existeTienda(tiendas, numTiendas, e.tiendaCodigo)) {
        cout << "[Error] La tienda asignada no existe.\n";
        return;
    }
    Empleado* aux = new Empleado[tam + 1];
    for (int i = 0; i < tam; i++) aux[i] = empleados[i];
    aux[tam] = e;
    delete[] empleados;
    empleados = aux;
    tam++;
    cout << "[Exito] Empleado registrado.\n";
}

void mostrarEmpleados(Empleado* empleados, int tam) {
    if (tam == 0) { cout << "No hay empleados.\n"; return; }
    cout << "\n--- LISTA DE EMPLEADOS ---\n";
    for (int i = 0; i < tam; i++) {
        cout << "Cod: " << empleados[i].codigo << " | Nombre: " << empleados[i].nombre
            << " | Puesto: " << empleados[i].puesto << " | Sueldo: S/ " << empleados[i].sueldo
            << " | Tienda: " << empleados[i].tiendaCodigo << "\n";
    }
}

// --- CÁLCULOS DEL SISTEMA ---
void calcularVentasPorTienda(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    cout << "\n--- VENTAS TOTALES POR TIENDA ---\n";
    for (int i = 0; i < numTiendas; i++) {
        double suma = 0;
        for (int j = 0; j < numDias; j++) suma += matriz[i][j];
        cout << tiendas[i].nombre << " (" << tiendas[i].codigo << "): S/ " << suma << "\n";
    }
}

void calcularPromedios(double** matriz, int numTiendas, int numDias) {
    cout << "\n--- PROMEDIOS DE VENTAS ---\n";
    double totalGlobal = 0;


    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) {
            totalGlobal += matriz[i][j];
        }
    }

    // Calcular el promedio diario considerando el total de días entre todas las tiendas o por día general
    double promedioDiario = 0;
    if (numDias > 0) {
        promedioDiario = totalGlobal / numDias;
    }

    cout << "Promedio Diario General: S/ " << promedioDiario << "\n";
    cout << "Total del periodo: S/ " << totalGlobal << "\n";
}

void ventaMaximaMinima(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    double maxV = matriz[0][0], minV = matriz[0][0];
    int tMax = 0, dMax = 0, tMin = 0, dMin = 0;

    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) {
            if (matriz[i][j] > maxV) { maxV = matriz[i][j]; tMax = i; dMax = j; }
            if (matriz[i][j] < minV) { minV = matriz[i][j]; tMin = i; dMin = j; }
        }
    }
    cout << "\n--- VENTA MAXIMA Y MINIMA ---\n";
    cout << "Venta Maxima: S/ " << maxV << " en la tienda " << tiendas[tMax].nombre << " el Dia " << (dMax + 1) << "\n";
    cout << "Venta Minima: S/ " << minV << " en la tienda " << tiendas[tMin].nombre << " el Dia " << (dMin + 1) << "\n";
}

void totalGeneralYParticipacion(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    double totalGen = 0;
    double* totalesTienda = new double[numTiendas];

    for (int i = 0; i < numTiendas; i++) {
        totalesTienda[i] = 0;
        for (int j = 0; j < numDias; j++) {
            totalesTienda[i] += matriz[i][j];
        }
        totalGen += totalesTienda[i];
    }

    cout << "\n--- TOTAL GENERAL Y PARTICIPACION ---\n";
    cout << "Total General Vendido: S/ " << totalGen << "\n";
    for (int i = 0; i < numTiendas; i++) {
        double porcentaje = 0;
        if (totalGen > 0) {
            porcentaje = totalesTienda[i] * 100.0 / totalGen;
        }
        cout << tiendas[i].nombre << ": " << porcentaje << "% de participacion\n";
    }
    delete[] totalesTienda;
}

// --- REPORTES DEL SISTEMA ---
void reportesSistema(double** matriz, Tienda* tiendas, int numTiendas, int numDias) {
    cout << "\n--- REPORTES ESTADISTICOS ---\n";
    double* totales = new double[numTiendas];
    for (int i = 0; i < numTiendas; i++) {
        totales[i] = 0;
        for (int j = 0; j < numDias; j++) totales[i] += matriz[i][j];
    }

    // Tienda mas rentable y menor venta
    int idxMax = 0, idxMin = 0;
    for (int i = 1; i < numTiendas; i++) {
        if (totales[i] > totales[idxMax]) idxMax = i;
        if (totales[i] < totales[idxMin]) idxMin = i;
    }

    cout << "Tienda con mayor venta: " << tiendas[idxMax].nombre << " con S/ " << totales[idxMax] << "\n";
    cout << "Tienda con menor venta: " << tiendas[idxMin].nombre << " con S/ " << totales[idxMin] << "\n";

    // Ranking simple (Burbuja)
    int* indices = new int[numTiendas];
    for (int i = 0; i < numTiendas; i++) indices[i] = i;
    for (int i = 0; i < numTiendas - 1; i++) {
        for (int j = 0; j < numTiendas - i - 1; j++) {
            if (totales[indices[j]] < totales[indices[j + 1]]) {
                int temp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = temp;
            }
        }
    }

    cout << "\nRanking de Tiendas (Mayor a Menor Venta):\n";
    for (int i = 0; i < numTiendas; i++) {
        cout << i + 1 << ". " << tiendas[indices[i]].nombre << " - S/ " << totales[indices[i]] << "\n";
    }

    delete[] totales;
    delete[] indices;
}

// --- GESTION DE ARCHIVOS ---
void guardarDatos(Tienda* tiendas, int numTiendas, double** matriz, int numDias, Producto* productos, int numProductos, Empleado* empleados, int numEmpleados) {
    // Comprobar acceso sin borrar los archivos existentes.
    ofstream ft("tiendas.txt", ios::app), fv("ventas.txt", ios::app);
    ofstream fp("productos.txt", ios::app), fe("empleados.txt", ios::app);
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] No se pudieron abrir todos los archivos para guardar.\n";
        return;
    }
    ft.close(); fv.close(); fp.close(); fe.close();
    ft.open("tiendas.txt", ios::trunc); fv.open("ventas.txt", ios::trunc);
    fp.open("productos.txt", ios::trunc); fe.open("empleados.txt", ios::trunc);
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] No se pudieron preparar los archivos para guardar.\n";
        return;
    }
    fv.precision(17);
    fp.precision(17);
    fe.precision(17);
    for (int i = 0; i < numTiendas; i++) {
        ft << tiendas[i].codigo << "|" << tiendas[i].nombre << "|"
           << tiendas[i].direccion << "|" << tiendas[i].distrito << "|"
           << tiendas[i].encargado << "|" << tiendas[i].estado << "\n";
    }
    fv << numTiendas << " " << numDias << "\n";
    for (int i = 0; i < numTiendas; i++) {
        for (int j = 0; j < numDias; j++) fv << matriz[i][j] << " ";
        fv << "\n";
    }
    for (int i = 0; i < numProductos; i++) {
        fp << productos[i].codigo << "|" << productos[i].nombre << "|"
           << productos[i].precio << "|" << productos[i].stock << "\n";
    }
    for (int i = 0; i < numEmpleados; i++) {
        fe << empleados[i].codigo << "|" << empleados[i].nombre << "|"
           << empleados[i].puesto << "|" << empleados[i].sueldo << "|"
           << empleados[i].tiendaCodigo << "\n";
    }
    ft.close(); fv.close(); fp.close(); fe.close();
    if (!ft || !fv || !fp || !fe) {
        cout << "[Error] Escritura incompleta. Revise los archivos antes de cerrar.\n";
        return;
    }
    cout << "[Exito] Tiendas, ventas, productos y empleados guardados.\n";
}

// Comprueba que despues de un numero no haya texto sobrante en esa linea.
bool terminarLinea(ifstream& archivo) {
    string resto;
    getline(archivo, resto);
    if (archivo.bad()) return false;
    return soloEspacios(resto);
}

// Cuenta las lineas para reservar un arreglo del tamano necesario.
int contarRegistros(ifstream& archivo, int maximo) {
    int cantidad = 0;
    string linea;
    while (getline(archivo, linea)) {
        if (soloEspacios(linea)) return -1;
        cantidad++;
        if (cantidad > maximo) return -1;
    }
    if (archivo.bad()) return -1;
    archivo.clear();
    archivo.seekg(0); // Volver al inicio para leer los registros.
    return cantidad;
}

bool cargarTienda(ifstream& archivo, Tienda& tienda) {
    int estado = 0;
    // Cada getline lee hasta la barra que separa los campos.
    getline(archivo, tienda.codigo, '|');
    getline(archivo, tienda.nombre, '|');
    getline(archivo, tienda.direccion, '|');
    getline(archivo, tienda.distrito, '|');
    getline(archivo, tienda.encargado, '|');
    archivo >> estado;
    if (archivo.fail()) return false;
    if (terminarLinea(archivo) == false) return false;
    if (textoValido(tienda.codigo) == false) return false;
    if (textoValido(tienda.nombre) == false) return false;
    if (textoValido(tienda.direccion) == false) return false;
    if (textoValido(tienda.distrito) == false) return false;
    if (textoValido(tienda.encargado) == false) return false;
    if (estado != 0 && estado != 1) return false;
    if (estado == 1) {
        tienda.estado = true;
    } else {
        tienda.estado = false;
    }
    return true;
}

bool cargarProducto(ifstream& archivo, Producto& producto) {
    char separador = ' ';
    getline(archivo, producto.codigo, '|');
    getline(archivo, producto.nombre, '|');
    archivo >> producto.precio;
    archivo >> separador;
    archivo >> producto.stock;
    if (archivo.fail()) return false;
    if (separador != '|') return false;
    if (terminarLinea(archivo) == false) return false;
    if (textoValido(producto.codigo) == false) return false;
    if (textoValido(producto.nombre) == false) return false;
    if (producto.precio < 0 || producto.precio > 1000000000000.0) return false;
    if (producto.stock < 0 || producto.stock > 1000000000) return false;
    return true;
}

bool cargarEmpleado(ifstream& archivo, Empleado& empleado) {
    char separador = ' ';
    getline(archivo, empleado.codigo, '|');
    getline(archivo, empleado.nombre, '|');
    getline(archivo, empleado.puesto, '|');
    archivo >> empleado.sueldo;
    archivo >> separador;
    getline(archivo, empleado.tiendaCodigo);
    if (archivo.fail()) return false;
    if (separador != '|') return false;
    if (textoValido(empleado.codigo) == false) return false;
    if (textoValido(empleado.nombre) == false) return false;
    if (textoValido(empleado.puesto) == false) return false;
    // Quitar el retorno final de los archivos creados en Windows.
    string codigoLimpio = "";
    int cantidad = empleado.tiendaCodigo.length();
    for (int i = 0; i < cantidad; i++) {
        if (empleado.tiendaCodigo[i] != '\r') {
            codigoLimpio += empleado.tiendaCodigo[i];
        }
    }
    empleado.tiendaCodigo = codigoLimpio;
    if (textoValido(empleado.tiendaCodigo) == false) return false;
    if (empleado.sueldo < 0 || empleado.sueldo > 1000000000000.0) return false;
    return true;
}

void cargarDatos(Tienda*& tiendas, int& numTiendas, double**& matriz, int& numDias, Producto*& productos, int& numProductos, Empleado*& empleados, int& numEmpleados) {
    ifstream archivoTiendas("tiendas.txt");
    ifstream archivoVentas("ventas.txt");
    ifstream archivoProductos("productos.txt");
    ifstream archivoEmpleados("empleados.txt");
    if (archivoTiendas.is_open() == false || archivoVentas.is_open() == false
        || archivoProductos.is_open() == false || archivoEmpleados.is_open() == false) {
        cout << "[Error] Se necesitan tiendas.txt, ventas.txt, productos.txt y empleados.txt.\n";
        cout << "No se modificaron los datos actuales.\n";
        return;
    }

    // 1. Leer y comprobar las cantidades antes de reservar memoria.
    int nuevasTiendas = 0;
    int nuevosDias = 0;
    archivoVentas >> nuevasTiendas >> nuevosDias;
    bool valido = true;
    if (archivoVentas.fail()) valido = false;
    if (terminarLinea(archivoVentas) == false) valido = false;
    if (nuevasTiendas < 0 || nuevasTiendas > 1000) valido = false;
    if (nuevosDias < 0 || nuevosDias > 366) valido = false;
    if (nuevasTiendas == 0 && nuevosDias != 0) valido = false;
    int filasTiendas = contarRegistros(archivoTiendas, 1000);
    int nuevosProductos = contarRegistros(archivoProductos, 100000);
    int nuevosEmpleados = contarRegistros(archivoEmpleados, 100000);
    if (filasTiendas != nuevasTiendas || filasTiendas < 0) valido = false;
    if (nuevosProductos < 0 || nuevosEmpleados < 0) valido = false;
    if (valido == false) {
        cout << "[Error] Archivos invalidos o cantidades inconsistentes. No se modificaron los datos actuales.\n";
        return;
    }

    // 2. Preparar arreglos temporales. Aun no se borra la sesion actual.
    Tienda* tiendasCargadas = new Tienda[nuevasTiendas];
    Producto* productosCargados = new Producto[nuevosProductos];
    Empleado* empleadosCargados = new Empleado[nuevosEmpleados];
    double** ventasCargadas = new double*[nuevasTiendas];
    for (int i = 0; i < nuevasTiendas; i++) {
        ventasCargadas[i] = new double[nuevosDias];
        for (int j = 0; j < nuevosDias; j++) {
            ventasCargadas[i][j] = 0;
        }
    }

    // 3. Cargar tiendas y comprobar codigos repetidos.
    for (int i = 0; i < nuevasTiendas && valido; i++) {
        valido = cargarTienda(archivoTiendas, tiendasCargadas[i]);
        if (valido && existeTienda(tiendasCargadas, i, tiendasCargadas[i].codigo)) {
            valido = false;
        }
    }

    // 4. Cargar los importes por tienda y dia.
    for (int i = 0; i < nuevasTiendas && valido; i++) {
        for (int j = 0; j < nuevosDias && valido; j++) {
            archivoVentas >> ventasCargadas[i][j];
            if (archivoVentas.fail()) valido = false;
            if (ventasCargadas[i][j] < 0 || ventasCargadas[i][j] > 1000000000000.0) valido = false;
        }
    }
    string sobrante;
    while (getline(archivoVentas, sobrante)) {
        if (soloEspacios(sobrante) == false) valido = false;
    }

    // 5. Cargar productos y empleados.
    for (int i = 0; i < nuevosProductos && valido; i++) {
        valido = cargarProducto(archivoProductos, productosCargados[i]);
        if (valido && existeProducto(productosCargados, i, productosCargados[i].codigo)) {
            valido = false;
        }
    }
    for (int i = 0; i < nuevosEmpleados && valido; i++) {
        valido = cargarEmpleado(archivoEmpleados, empleadosCargados[i]);
        if (valido && existeTienda(tiendasCargadas, nuevasTiendas, empleadosCargados[i].tiendaCodigo) == false) {
            valido = false;
        }
        for (int j = 0; j < i; j++) {
            if (empleadosCargados[j].codigo == empleadosCargados[i].codigo) valido = false;
        }
    }

    // 6. Si hay un error, borrar solo los arreglos temporales.
    if (valido == false) {
        for (int i = 0; i < nuevasTiendas; i++) {
            delete[] ventasCargadas[i];
        }
        delete[] ventasCargadas;
        delete[] tiendasCargadas;
        delete[] productosCargados;
        delete[] empleadosCargados;
        cout << "[Error] Archivo incompleto, duplicado o con datos invalidos. No se modificaron los datos actuales.\n";
        return;
    }

    // 7. La lectura fue correcta. Liberar la matriz con su tamano anterior.
    if (matriz != nullptr) {
        for (int i = 0; i < numTiendas; i++) {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
    delete[] tiendas;
    delete[] productos;
    delete[] empleados;

    // 8. Usar los datos cargados y actualizar sus contadores.
    tiendas = tiendasCargadas;
    matriz = ventasCargadas;
    productos = productosCargados;
    empleados = empleadosCargados;
    numTiendas = nuevasTiendas;
    numDias = nuevosDias;
    numProductos = nuevosProductos;
    numEmpleados = nuevosEmpleados;
    cout << "[Exito] Datos cargados. Se reemplazo la sesion actual.\n";
}
