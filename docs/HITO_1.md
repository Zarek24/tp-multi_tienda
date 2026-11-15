# Hito 1 — SpeedyBite Delivery

Borrador académico basado en la consigna compartida. El nombre proviene del proyecto existente. La situación de la empresa, sus problemas y beneficios son supuestos de la propuesta; deben ser confirmados por el equipo. Este documento no acredita cumplimiento de una rúbrica de entrega final aún no disponible.

## Análisis del negocio

**Nombre del proceso:** Registro y análisis de recaudación diaria por tienda.

**Descripción:** El usuario registra las sedes del negocio y sus montos diarios de venta durante un periodo de evaluación. Consulta resultados por sede y del conjunto, y conserva los registros para una próxima sesión. Como registros de apoyo, administra un catálogo de productos y una lista de empleados asignados a tiendas.

**Objetivo general:** Desarrollar un sistema de consola en C++ que centralice el registro y análisis de recaudación de las tiendas de SpeedyBite Delivery.

**Objetivos específicos:**

- Registrar tiendas, sus responsables y su estado.
- Organizar ventas en una matriz de tiendas por días.
- Calcular totales, promedios diarios, extremos y participación por tienda.
- Comparar la recaudación mediante un ranking.
- Mantener registros básicos de productos y empleados.
- Guardar y recuperar información con validación de integridad.

**Problema actual propuesto:** El registro separado de ventas de cada sede dificulta consolidar montos y comparar resultados. La transcripción manual puede producir errores y pérdida de información entre sesiones. Estos problemas requieren validación con el caso de negocio real.

**Solución propuesta:** Aplicación de consola con menús, estructuras para los registros, arrays dinámicos unidimensionales y una matriz dinámica bidimensional para ventas. Un archivo de texto versionado conserva los datos relacionados en conjunto.

**Beneficios esperados:** Menor tiempo de consolidación, cálculos consistentes, comparación entre sedes, recuperación de información y rechazo de entradas inválidas. No se han medido beneficios cuantitativos.

**Alcance:** Un usuario local, hasta 1000 tiendas, un periodo de 1 a 366 días, catálogo de hasta 100000 productos y hasta 100000 empleados. Las ventas son montos agregados por tienda y día; registrar otra vez la misma celda reemplaza su importe. Los ceros iniciales se consideran ventas de cero en los cálculos. Los datos se guardan manualmente. El catálogo y los empleados son registros independientes de apoyo: no calculan ventas ni descuentan existencias. El ranking mide recaudación; para medir utilidad se necesitarían costos y gastos.

**Fuera del alcance actual:** Facturación, pedidos, pagos, entregas, usuarios con roles, inventario por sede, costos, base de datos, acceso simultáneo, fechas calendario y múltiples periodos históricos. Reducir los días de la matriz elimina las columnas excedentes en la sesión; se debe guardar una copia del archivo si se requiere conservarlas.

## Requerimientos funcionales

| ID | Requerimiento | Opción |
|---|---|---|
| RF01 | Registrar tiendas con código único, nombre, dirección, distrito y encargado | 1 |
| RF02 | Consultar tiendas y estado | 2 |
| RF03 | Cambiar encargado, dirección/distrito o estado | 3 |
| RF04 | Crear/redimensionar la matriz conservando celdas que permanezcan en el rango | 4 |
| RF05 | Registrar un importe no negativo en una tienda activa y día válido | 4 |
| RF06 | Consultar la matriz con códigos de tienda y días | 5 |
| RF07 | Registrar productos con código único, precio y stock no negativos | 6 |
| RF08 | Consultar el catálogo de productos | 7 |
| RF09 | Registrar empleados con código único y sueldo no negativo en una tienda existente | 8 |
| RF10 | Consultar empleados y su tienda asignada | 9 |
| RF11 | Mostrar total y promedio diario por tienda, promedio diario general y total del periodo | 10 |
| RF12 | Mostrar celda de venta máxima y mínima con tienda y día | 11 |
| RF13 | Calcular total general y porcentaje de participación de cada tienda | 12 |
| RF14 | Mostrar tiendas con mayor/menor recaudación y ranking descendente | 13 |
| RF15 | Guardar todos los registros en un archivo de texto | 14 |
| RF16 | Cargar todos los registros; rechazar archivos inválidos sin alterar la sesión | 15 |
| RF17 | Salir liberando la memoria dinámica | 0 |

## Prototipo y secuencia de menús

El prototipo funcional es la consola implementada. El menú principal presenta las opciones 1 a 15 y 0 de la tabla anterior y vuelve a mostrarse después de cada operación.

```text
INICIO → Menú principal
  1 → Código → Nombre → Dirección → Distrito → Encargado → Confirmación → Menú
  2 → Lista de tiendas → Menú
  3 → Código de tienda → Submenú:
        1. Nuevo encargado → Menú
        2. Nueva dirección → Nuevo distrito → Menú
        3. Alternar estado activa/inactiva → Menú
  4 → Número de días → Lista de tiendas → Índice de tienda (desde 0)
      → Día (desde 1) → Monto → ¿Otra venta?
        1. Sí → Lista y selección nuevamente
        0. No → Menú
  5 → Tabla tienda/día → Menú
  6 → Código → Nombre → Precio → Stock → Confirmación → Menú
  7 → Lista de productos → Menú
  8 → Código → Nombre → Puesto → Sueldo → Lista de tiendas
      → Código de tienda asignada → Confirmación → Menú
  9 → Lista de empleados → Menú
 10 → Totales y promedios → Menú
 11 → Venta máxima y mínima → Menú
 12 → Total y participaciones → Menú
 13 → Mayor/menor recaudación y ranking → Menú
 14 → Guardar archivo → Resultado → Menú
 15 → Validar archivo → Reemplazar sesión si es válido → Resultado → Menú
  0 → Liberar memoria → FIN
```

Las opciones con dependencias informan si faltan tiendas o matriz. Los códigos duplicados y las tiendas inexistentes cancelan el registro. La entrada numérica inválida vuelve a solicitarse. EOF termina la sesión y libera memoria; no hay guardado automático.

## Funciones de usuario y conceptos del curso

| Concepto | Implementación |
|---|---|
| Tipos de datos y estructuras | `Tienda`, `Producto`, `Empleado`, `string`, `int`, `double`, `bool` |
| Selectivas | `switch` del menú; `if` para validación y búsquedas |
| Repetitivas | `do/while` del menú y `for` de recorridos y ranking |
| Funciones | Registro, consulta, modificación, cálculos, reportes y archivos |
| Array unidimensional | Arrays dinámicos de tiendas, productos, empleados y totales |
| Array bidimensional | `double** matrizVentas`, con una fila por tienda |
| Archivos | `guardarDatos` y `cargarDatos`, flujos de texto y formato versionado |
| Buenas prácticas | Funciones con responsabilidad concreta, validación, precisión monetaria y liberación de memoria |

## Guion de demostración

1. Registrar T1 y T2.
2. Crear matriz de 2 días: T1 = 100 y 200; T2 = 50 y 150.
3. Mostrar matriz y reportes: totales 300 y 200; promedios 150 y 100; total general 500; promedio diario general 250; participación 60% y 40%; máximo 200 (T1, día 2), mínimo 50 (T2, día 1).
4. Registrar un producto y un empleado en T1.
5. Agregar T3 después de crear la matriz y verificar que sus ventas parten de cero.
6. Guardar, salir, ejecutar de nuevo y cargar; verificar los cuatro tipos de datos.
7. Probar letras en campos numéricos, importes negativos y códigos duplicados.

## Pendiente de validación del equipo

Confirmar el caso de negocio, si productos y empleados forman parte del alcance solicitado, si se requiere informe con formato institucional y cuáles son los requisitos adicionales de la entrega final.
