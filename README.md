# SpeedyBite Delivery — Gestión multitienda

Aplicación de consola en C++17 para registrar tiendas, ventas diarias, productos y empleados. Usa estructuras, funciones, arrays dinámicos y archivos de texto.

## Compilación y ejecución

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic multitienda.cpp -o multitienda
./multitienda
```

En Windows con MinGW, ejecutar `multitienda.exe`. Los archivos se crean en la carpeta desde donde se ejecuta el programa.

Registrar primero tiendas (1), luego crear la matriz y registrar ventas (4). Las opciones 10–13 presentan los cálculos. Para seleccionar una tienda en ventas se usa su índice desde cero; para asignar un empleado se usa el código de tienda. Una venta registrada sobre la misma celda sustituye el importe anterior.

La opción 14 guarda manualmente todos los datos en `multitienda.dat`; la opción 15 los carga y **reemplaza la sesión actual** si el archivo completo es válido. No hay guardado automático ni historial de periodos. La matriz acepta hasta 366 días; reducirla descarta las columnas excedentes.

## Persistencia

`multitienda.dat` es un archivo de texto con cabecera `MULTITIENDA_V1`, dimensiones, tiendas, matriz, productos y empleados. Los campos de texto usan comillas y escapes para conservar espacios y separadores. La escritura usa primero `multitienda.tmp` y luego lo renombra; si el sistema impide reemplazar el destino, informa el error y conserva el archivo anterior. La carga valida dimensiones, códigos únicos, relaciones y rangos antes de cambiar los datos actuales.

Este formato **no importa** el antiguo `ventas.txt` de balances ni los archivos separados del avance del compañero. Conservarlos como referencia si contienen información necesaria.

## Documentación del hito

[Análisis del negocio, requerimientos, prototipo y guion de demostración](docs/HITO_1.md). El caso de negocio es una propuesta académica por confirmar. La consigna disponible corresponde al primer hito; faltan los criterios de la entrega final.

El sistema calcula recaudación, no utilidad: no registra costos. Productos y empleados se mantienen como registros de apoyo; no generan transacciones ni movimientos de inventario.

## Verificación

Requiere Python 3 y un compilador con sanitizadores (GCC/Clang en Linux):

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined -g multitienda.cpp -o multitienda
python3 tests/test_flujos.py ./multitienda
```

Las pruebas usan directorios temporales y cubren redimensionamiento después de agregar tiendas, totales y promedios, persistencia completa, carga repetida, archivos corruptos, dimensiones inválidas, entradas erróneas y fin de entrada.
