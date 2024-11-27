# Graph Server

Este proyecto implementa un servidor que utiliza un grafo dirigido para calcular rutas alcanzables entre nodos. El servidor se comunica con los clientes a través de sockets.

## Descripción

El programa principal crea un servidor que escucha en un puerto especificado. Utiliza una clase `Graph` para representar un grafo dirigido mediante listas de adyacencia. Los clientes pueden conectarse al servidor y enviar solicitudes para verificar si existe un camino entre dos nodos en el grafo.

## Estructura del Código

- **Graph**: Clase que representa un grafo dirigido.
  - `Graph(int V)`: Constructor que inicializa el grafo con `V` vértices.
  - `void addEdge(int v, int w)`: Método para añadir una arista del vértice `v` al vértice `w`.
  - `bool isReachable(int source, int destiny)`: Método que verifica si existe un camino desde el vértice `source` al vértice `destiny`.

- **Funciones Auxiliares**:
  - `void error(const char *msg)`: Función para manejar errores y terminar el programa.

- **Servidor**:
  - El servidor se crea y se enlaza a una dirección IP y un puerto.
  - Escucha conexiones entrantes y acepta conexiones de clientes.
  - Envía y recibe datos a través de los sockets.

## Uso

1. Compila el código:
   ```sh
   g++ -o graph_server graph_server.cpp


## Ejecutar servidor

./graph_server <puerto>

## Requisitos
Compilador compatible con C++ (por ejemplo, g++).

Sistema operativo compatible con sockets (por ejemplo, Linux).

## Notas
Asegúrate de proporcionar un puerto válido al ejecutar el servidor.

El servidor puede manejar múltiples conexiones de clientes, pero este ejemplo no implementa concurrencia.