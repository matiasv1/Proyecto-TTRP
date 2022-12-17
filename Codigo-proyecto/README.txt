Se agrego un nuevo parámetro que es la cantidad de re-start, los correctos parámetros a entregar son los del siguiente ejemplo:
./TTRP instances/TTRP_01.txt 1000 1 ( 1000 representa la cantidad de iteraciones fijas y 1 representa entregar solo la solución greedy)
./TTRP instances/TTRP_01.txt 1000 6 ( 1000 representa la cantidad de iteraciones fijas y 6 representa entregar la solución greedy inicial + 5 soluciones con solucion inicial aleatoria)

El formato de las subrutas es el siguiente:
4-17-37-4  (la subruta comienta con el nodo raíz 4)

Ejemplo de formato de salida:

0-1-8-31-28-36-3-22-0 106.296 TP
0-33-4-18-25-13-40-42-0 163.291 VC
0-44-49-27-48-26-23-43-24-14-6-0 169.298 VC
0-12-15-45-39-10-9-5-0 100.702 TP
0-32-2-11-38-30-34-21-29-35-20-0 132.024 VC
4-37-17-4 25.3639 ST
42-19-41-42 26.5824 ST
23-7-23 12.1655 ST
6-47-6 31.0483 ST
2-16-2 17.088 ST
32-46-32 18.4391 ST

TP -> ruta camión puro
VC -> main tour de ruta vehículo completo
VP -> ruta vehículo puro
ST -> subtour