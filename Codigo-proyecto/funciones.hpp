#ifndef FX_HPP
#define FX_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bits/stdc++.h>
#include <vector>
#include <algorithm>
#include <tuple> 
#include <ctime>

using namespace std;

typedef struct NODE{
    float id;
    float x;
    float y;
    float demand;
    float type;
} Node;

typedef struct ASSIGNMENT_ROUTE{   
    float id_semilla;
    float tipo_transporte;
    float demanda_total;
    float cantidad_demandada;
    float capacidad_total;
    float capacidad_disponible;
    vector<int> clientes;  // id de los clientes
    
} Route;

typedef struct ROUTETSP{
  vector<int> clientes;
  string type_print;
  float distance;
  float capacidad_disponible;
  string type;
  float id_semilla;
  int id_subtour;
} RouteTSP;

typedef struct TruckSub{
  vector<int> id_clients;
  int id_ruta;
  float distance;
  float capacidad_disponible;
} TruckSub;

int getIndex(vector<float> v, float K);
int getIndex_int(vector<int> v, int K);
void printRoutes(vector<Route> routes, Node array[]);
bool FindVechicleCostumer(Route ruta, Node array[]);
void dijMatrix(vector<vector<float>> &matrix_dij, vector<vector<float>> matrixDistances, vector<int> listSeeds, int nCustomers);
float sumCol(vector<vector<float>> matrizDistancesSeed, int column );
bool ClientInRoute (vector<RouteTSP> Subtours, int new_cliente);
void Distancefinal_init (vector<RouteTSP> &listRoutes, vector<RouteTSP> &Subtours, vector<vector<float>> matrixDistances);
void printRoutes(vector<RouteTSP> listRoutes, vector<RouteTSP> Subtours);
float allDistance(vector<RouteTSP> listRoutes, vector<RouteTSP> Subtours);
void computeSeed(vector<int> &listIndex, vector<vector<float>> matrixDistances, vector<float> &maxDistances);
void addClientToRoute(vector<int> &secuencia_ruta, vector<vector<float>> matrixDistances, vector<int> secuencia_asignacion, float &distance);
void CalculatedistanceMatrix(vector<vector<float>> &matrixDistances, Node array[], int nCustomers, int flag);
vector<Route> greedyAssigment (vector<vector<float>> matrixDistances, int nTrucks, int capacityTrailers, int capacityTrucks, int nTrailers, Node array[]);
void greedyTsp(vector<Route> rutas, Node array[], vector<vector<float>> all_distanceMatrix, vector<RouteTSP> &listRoutes,vector<RouteTSP> &Subtours);
void HillClimber(int iteraciones, vector<vector<float>> distanceMatrix, vector<RouteTSP> &listRoutes,vector<RouteTSP> &Subtours , Node nodos[], int restart);
tuple<vector<int>,vector<int>,float,float,float> changeDistance(vector<vector<float>> distanceMatrix, int cliente1, int cliente2, RouteTSP ruta_tsp1, RouteTSP ruta_tsp2, int flag, vector<RouteTSP> listRoutes,vector<RouteTSP> Subtours);

void ejecutarMejorMovimiento( vector<vector<float>> distanceMatrix,tuple<float,int,int,RouteTSP,RouteTSP> movimiento,vector<RouteTSP> &listRoutes,vector<RouteTSP> &listSubtours);
bool MovimientoFactible(RouteTSP ruta_tsp1, RouteTSP ruta_tsp2 ,int cliente1, int cliente2,Node nodos[]);


#endif