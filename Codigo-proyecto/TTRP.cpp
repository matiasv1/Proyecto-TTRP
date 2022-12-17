#include"funciones.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <tuple> 

using namespace std;



int main(int argc, char *argv[]) {
  string cdFile = argv[1];
  string maxIter = argv[2];
  string restart = argv[3];
  string parameters[5];
  string firstLine;
  
  clock_t start, end;
  start = clock();

  vector<RouteTSP> listRoutes;
  vector<RouteTSP> Subtours;
 

  
  //Leer primera linea del archivo y sacar argumentos
  ifstream file(cdFile);

  if (file.good())
  {
    
    getline(file, firstLine);
    
  }
  file.close();
  
  //Parametros del arreglo
  int i = 0;

  stringstream ssin(firstLine);
    while (ssin.good() && i < 5){
        ssin >> parameters[i];
        ++i;
    }
    
  
  // Parametros del problema en variables
  const int nTrucks = stoi(parameters[0]);
  const int capacityTrucks = stoi(parameters[1]);
  const int nTrailers = stoi(parameters[2]);
  const int capacityTrailers = stoi(parameters[3]); 
  const int nCustomers = stoi(parameters[4]); 
  

  string line;
  ifstream file2(cdFile);
  getline (file2,line);

  Node arrayNode [nCustomers];
  int index =0;
  while (getline (file2,line)){
    
    istringstream is( line );
    is >> arrayNode[index].id;
    is >> arrayNode[index].x;
    is >> arrayNode[index].y;
    is >> arrayNode[index].demand;
    is >> arrayNode[index].type;
    index++;
    
  }
  vector<vector<float>> distanceMatrix;
  CalculatedistanceMatrix(distanceMatrix, arrayNode, nCustomers,1);

  vector<Route> rutasAsignadas=greedyAssigment(distanceMatrix, nTrucks, capacityTrailers, capacityTrucks, nTrailers, arrayNode);
  greedyTsp(rutasAsignadas,arrayNode,distanceMatrix,listRoutes, Subtours);

  HillClimber(1000, distanceMatrix, listRoutes,Subtours , arrayNode,stoi(restart));

  end = clock();
  
    // Calculating total time taken by the program.
  double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
  cout << "Tiempo de ejecución del programa : " << fixed 
        << time_taken << setprecision(5);
  cout << " [s] " << endl;

  
  
  
  return 0;
}