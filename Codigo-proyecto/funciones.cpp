#include"funciones.hpp"
//Funcion utilizada para obtener el indice de un elemento dentro de un vector de float
int getIndex(vector<float> v, float K)
{
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()) 
    {
        int index = it - v.begin();
        return index;
    }
    else {
        return -1;
    }
}

//Funcion utilizada para obtener el indice de un elemento dentro de un vector de float
int getIndex_int(vector<int> v, int K)
{
    auto it = find(v.begin(), v.end(), K);
    if (it != v.end()) 
    {
        int index = it - v.begin();
        return index;
    }
    else {
        return -1;
    }
}
// Funcion para encontrar si el cliente de camión ya fue asignado a otra ruta
bool ClientInRoute (vector<RouteTSP> Subtours, int new_cliente){
  for (auto & subtour : Subtours){
    for (auto & client : subtour.clientes){
      if (client == new_cliente){
        return false;
      }
      
    }
  }
  return true;
}
//Funcion utilizada para imprimir las rutas del problema de asignación
void printRoutes(vector<Route> routes, Node array[]){
  for (int i=0; i<routes.size();i++){
    cout<<"Ruta "<<i+1<<endl;
    cout<<"Semilla: "<<routes[i].id_semilla<<endl;
    cout<<"Tipo Transporte: "<<routes[i].tipo_transporte<<endl;
    cout<<"Capacidad Disponible: "<<routes[i].capacidad_disponible<<endl;
    cout<<"Demanda total: "<<routes[i].cantidad_demandada<<endl;
    
    for(int j=0;j<routes[i].clientes.size();j++){
      cout<<"- "<<routes[i].clientes[j]<<"-"<<array[routes[i].clientes[j]].type<< endl;
      
    }
  }
}
//Funcion para preguntar si el cliente no ha sido asignado en otra ruta
bool FindVechicleCostumer(Route ruta, Node array[]){
  int count =0;
  for( auto &cliente :ruta.clientes){
    if (cliente !=0){
      if (array[cliente].type == 0){
        count++;
      }
    }
  }
   if (count >= 1){
      return true;
    }
    else{
      return false;
    }
  
}
// Funcion para calcular la matriz dij
void dijMatrix(vector<vector<float>> &matrix_dij, vector<vector<float>> matrixDistances, vector<int> listSeeds, int nCustomers){
  for(int i=1;i<=nCustomers;i++){
    vector<float> distances;
    for(int j=1 ; j<listSeeds.size(); j++){
      int seed = listSeeds[j];
      float dij = matrixDistances[0][i]+ matrixDistances[i][seed] - matrixDistances[0][seed];
      distances.push_back(dij);
    }
    matrix_dij.push_back(distances);
  }

}
// Funcion para sumar una columna, permite ver cual es la semilla más lejana.
float sumCol(vector<vector<float>> matrizDistancesSeed, int column ){
  float sum=0.0;
  for (int i=0;i<matrizDistancesSeed.size();i++){
    sum += matrizDistancesSeed[i][column];
  }
  return sum;

}
// Funcion para calcular la distancia entre todos los nodos finales e iniciales 
void Distancefinal_init (vector<RouteTSP> &listRoutes, vector<RouteTSP> &Subtours, vector<vector<float>> matrixDistances){

  //Calcular distancia final e inicial de los nodos de main tour o truck puro o vehiculo puro
  for (auto & route :listRoutes){
    int last_client = route.clientes[route.clientes.size()-2];
    int init_client = route.clientes[0];
    
    float distance = matrixDistances[init_client][last_client];
    
    route.distance += distance;
  }
  // Calcular distancia final e inicial de los subtours
  for (auto & subroute : Subtours){
    int last_client = subroute.clientes[subroute.clientes.size()-1];
    int init_client = subroute.clientes[0];
    float distance = matrixDistances[init_client][last_client];
    subroute.distance += distance;
    subroute.clientes.push_back(init_client);
  }
}
// Funcion para imprimir la ruta TSP en el formato solicitado.
void printRuta(vector<RouteTSP> listRoutes, vector<RouteTSP> Subtours){

  for (auto & ruta : listRoutes){
    int count =0;
    for (auto & clientes : ruta.clientes){
      if (count == ruta.clientes.size()-1){
        cout<<clientes;
      }
      else{
         cout<<clientes<<"-";
      }
      count++;
    }
    cout<<" "<<ruta.distance<<" "<<ruta.type<<endl;
    
    
  }

  for( auto &sub : Subtours){
    int count =0;
    for (auto & client : sub.clientes){
        if (count == sub.clientes.size()-1){
          cout<<client;
        }
        else{
          cout<<client<<"-";
        }
        count++;
      }
    cout<<" "<<sub.distance<<" "<<"ST"<<endl;
  }
}

// Funcion para imprimir el costo total de la ruta
float allDistance(vector<RouteTSP> listRoutes, vector<RouteTSP> Subtours){
  float Distance =0;
  for (auto & ruta : listRoutes){
    Distance+= ruta.distance;
  }

  for( auto &sub : Subtours){
    Distance+= sub.distance;
  }
  return Distance;
}

// Funcion para crear una semilla
void computeSeed(vector<int> &listIndex, vector<vector<float>> matrixDistances, vector<float> &maxDistances){
  vector<vector<float>> matrizDistancesSeed;
  vector<float> sumDistances;
  // Crear una lista solo con las distancias de las semillas precedentes y la actual, para luego sumar las columnas y ver que semilla sera la nueva
  for (int i=0; i< listIndex.size() ; i++ ){
    matrizDistancesSeed.push_back(matrixDistances[listIndex[i]]);
  }
  for(int i=0; i<matrizDistancesSeed[0].size();i++){
    float sum=sumCol(matrizDistancesSeed,i);
    sumDistances.push_back(sum);
  }
  bool flag = true;

  vector<float> aux = sumDistances;

  // Encontrar la semilla que sea mas lejana a los clientes precedentes y si esa semilla fue seleccionada se busca una nueva hasta
  // que cambie la flag
  while (flag){
    float maxDistance = *max_element(sumDistances.begin(), sumDistances.end());
    int index = getIndex(sumDistances,maxDistance);
    int index2 = getIndex(aux,maxDistance);
    
    if (find(listIndex.begin(), listIndex.end(), index2) != listIndex.end()){
      
      sumDistances.erase(sumDistances.begin()+index);  
    }
    else{
      listIndex.push_back(index2);
      maxDistances.push_back(maxDistance);
      flag = false;
    }
  }
}
// Agregar el cliente mas cercano a la ruta TSP, se le entrega una lista con los nodos hasta el cliente actual y se ingresa el más cercano al ultimo
void addClientToRoute(vector<int> &secuencia_ruta, vector<vector<float>> matrixDistances, vector<int> secuencia_asignacion, float &distance){
  int clientActual = secuencia_ruta[secuencia_ruta.size()-1];  // cliente actual de la ruta
  int index_cliente = getIndex_int(secuencia_asignacion,clientActual);  // indice del cliente
  vector<float> distancias_cliente= matrixDistances[index_cliente]; // vector de distancias a los otros clientes
  vector <float> aux_distancias = distancias_cliente; 
  
  // Ordenar las distancias en orden, para encontrar un nodo vecino que tenga la minima distancia
  sort(distancias_cliente.begin(),distancias_cliente.end());
  int cont=0;
  for (auto &distancia : distancias_cliente){
    int index_distancia = getIndex(aux_distancias,distancia);
    
    if (find(secuencia_ruta.begin(), secuencia_ruta.end(),secuencia_asignacion[index_distancia]) == secuencia_ruta.end() && distancia !=0){
      secuencia_ruta.push_back(secuencia_asignacion[index_distancia]);
      
      distance += distancia;
      break;
    }
    // En caso que el arreglo tenga dos distancias iguales, caso poco frecuente
    else if (distancia == aux_distancias[cont+index_distancia] && distancia !=0 ){
      secuencia_ruta.push_back(secuencia_asignacion[index_distancia+cont]);
      distance += distancia;
      
      break;
    }
    cont++;
  }

}
// Calcular la matriz de distancias entre todos los clientes
void CalculatedistanceMatrix(vector<vector<float>> &matrixDistances, Node array[], int nCustomers, int flag){

  if (flag ==1){
    for(int i=0 ; i<=nCustomers ; i++){
      float x1 = array[i].x;
      float y1 = array[i].y;
      vector<float> distances;
      for(int j=0; j<= nCustomers; j++){
      float distance = 0;
      if (i != j){
        distance = sqrt(pow((array[j].x - x1),2) + pow((array[j].y - y1),2));
        distances.push_back(distance);
      }
      else{
        distances.push_back(0);
      }
    }
    matrixDistances.push_back(distances);
  }
  }
  // Matriz que no incluye el deposito o bien una mas pequeña, como el caso de todos los clientes del main tour
  if (flag ==2){
    for(int i=0 ; i<nCustomers ; i++){
      float x1 = array[i].x;
      float y1 = array[i].y;
      vector<float> distances;
      for(int j=0; j<nCustomers; j++){
      float distance = 0;
      if (i != j){
        distance = sqrt(pow((array[j].x - x1),2) + pow((array[j].y - y1),2));
        distances.push_back(distance);
      }
      else{
        distances.push_back(0);
      }
    }
    matrixDistances.push_back(distances);
  }
  }
}

// Funcion para resolver el problema de asignación
vector<Route> greedyAssigment (vector<vector<float>> matrixDistances, int nTrucks, int capacityTrailers, int capacityTrucks, int nTrailers, Node array[]){
  vector<float> maxDistances;
  vector<int> listSeeds;
  vector<vector<float>> matrix_dij;
  int cantidadTrailers = nTrailers;
  int contador_insatisfecho =0;
  listSeeds.push_back(0);

  // selecionar la primera semilla, buscar su indice e ingresarla a la lista
  float firstSeed = *max_element(matrixDistances[0].begin(), matrixDistances[0].end());
  int index = getIndex(matrixDistances[0], firstSeed);
  maxDistances.push_back(firstSeed);
  listSeeds.push_back(index);

  // computar una semilla por cada camión que haya
  for(int i=0;i<nTrucks-1;i++){
    computeSeed(listSeeds, matrixDistances, maxDistances);
    
  }
  //calcular la matriz de dij 
  dijMatrix(matrix_dij,matrixDistances,listSeeds, matrixDistances.size()-1);
  listSeeds.erase(listSeeds.begin());

  vector<Route> Routes;

  int cont=0;
  
  // inicializar una ruta por cada semilla
  for(auto & id_semilla : listSeeds){
    Route ruta;
    ruta.id_semilla = listSeeds[cont];
    ruta.tipo_transporte = 0;
    ruta.demanda_total = 0;
    ruta.capacidad_total = capacityTrucks;
    ruta.capacidad_disponible = capacityTrucks;
    ruta.cantidad_demandada =0;
    ruta.clientes.push_back(0);
    ruta.clientes.push_back(id_semilla);
    ruta.capacidad_disponible -= array[id_semilla].demand;
    
    ruta.cantidad_demandada += array[id_semilla].demand;
    Routes.push_back(ruta);
    cont++;
  }
  
  // Asignar todos los clientes y preguntar cual es la semilla mas cercana
  for (int i=0 ; i< matrix_dij.size(); i++){
    vector<float> aux = matrix_dij[i];
    bool flag = true;
    if (find(listSeeds.begin(), listSeeds.end(),i+1) == listSeeds.end()){ // comprobar que no sea una semilla
      while(flag){
      float min = *min_element(matrix_dij[i].begin(), matrix_dij[i].end());

      int index = getIndex(aux, min);
      int index2 = getIndex(matrix_dij[i],min); // indice que mantiene la inmutabilidad


      // Verificar si hay capacidad disponible para agregar el cliente a esa ruta  
      if (Routes[index].capacidad_disponible >= array[i+1].demand){
        
        Routes[index].capacidad_disponible -= array[i+1].demand ;
        Routes[index].cantidad_demandada += array[i+1].demand;
        Routes[index].clientes.push_back(i+1);
        flag = false;        
        
      }
      // Verificar que al menos hay un 0  count(v.begin(), v.end(), key))
      else if (array[i+1].demand >= Routes[index].capacidad_disponible && Routes[index].tipo_transporte == 0 && cantidadTrailers >0 && FindVechicleCostumer(Routes[index],array)){
        Routes[index].tipo_transporte =1;
        Routes[index].capacidad_disponible += capacityTrailers;
        Routes[index].capacidad_disponible -= array[i+1].demand;
        Routes[index].clientes.push_back(i+1);
        Routes[index].cantidad_demandada += array[i+1].demand;
        cantidadTrailers--;
        flag = false;
        
      }
      
      else{
        // no se pudo satisfaser el cliente con las rutas      
        if (matrix_dij[i].size()==1){
          flag = false;
          contador_insatisfecho ++;
        }
        // Eliminar la distancia y buscar una nueva distancia minima
        else{
          matrix_dij[i].erase(matrix_dij[i].begin()+index2);
        }
      }
    }
  }
  }
  //printRoutes(Routes,array);
  cout<<"Cantidad de clientes insatisfechos: "<<contador_insatisfecho<<endl;
  return Routes;   

}

// Resolver el problema TSP con enfoque greedy
void greedyTsp(vector<Route> rutas, Node array[], vector<vector<float>> all_distanceMatrix, vector<RouteTSP> &listRoutes,vector<RouteTSP> &Subtours){
float TotalDistance =0;
int contador=0;


  // Iterar por cada ruta de asignación
  for( auto &ruta: rutas){
    int cont=0;
    int contVC =0;
    int contTC =0;
    vector<int> indicesVC;
    vector<int> indicesTC;
    Node clientes[ruta.clientes.size()];
    vector<vector<float>> matrixDistances;
    vector<vector<float>> matrixDistancesTC;
    Node clientesTC[indicesTC.size()];

    // Obtener cuantos clientes hay de camión y de vehiculo
    for (auto & id_cliente : ruta.clientes){
      Node cliente = array[id_cliente];
      clientes[cont]= cliente;
      cont++;
      if (cliente.type == 0){
        contVC++;
        indicesVC.push_back(id_cliente);
      }
      else{
        indicesTC.push_back(id_cliente);
        contTC++;
      }
    }
    CalculatedistanceMatrix(matrixDistances,clientes,ruta.clientes.size(),2);
    
    // crear arreglo de solo clientes tipo vehiculo
    contVC =0;  
    vector<vector<float>> matrixDistancesVC;
    Node clientesVC[indicesVC.size()];
    for (auto & id_cliente : indicesVC){
      Node clienteVC = array[id_cliente];
      clientesVC[contVC] = clienteVC;
      contVC ++;
    }
    // Calcular la matriz de distancias entre solo clientes de vehiculo, se utiliza para los main tour
    CalculatedistanceMatrix(matrixDistancesVC,clientesVC,indicesVC.size(),2);

    // crear arreglo de solo clientes tipo camión
    if (contTC>0){
      contTC =0;  // solo nodos con tipo 0
      for (auto & id_cliente : indicesTC){
        Node clienteTC = array[id_cliente];
        clientesTC[contTC] = clienteTC;
        contTC ++;
      }
    }
    

    //inciar la ruta
    vector<int> secuencia_ruta;
    RouteTSP routeTsp;
    routeTsp.distance=0;  
    float distanceTotal =0;
    secuencia_ruta.push_back(0);

    //Ruta camión puro sin trailer
    if (ruta.tipo_transporte ==0){
      for (int i=0; i<ruta.clientes.size()-1; i++){
        
        addClientToRoute(secuencia_ruta,matrixDistances,ruta.clientes, distanceTotal);
      }
      routeTsp.clientes = secuencia_ruta;
      routeTsp.type_print = "T";
      routeTsp.type = "TP";
      routeTsp.id_semilla = ruta.id_semilla;
      routeTsp.clientes.push_back(0);
      routeTsp.id_subtour = -1;
      routeTsp.distance=distanceTotal;

      
    }
    // Ruta camión con trailer
    else if (ruta.tipo_transporte == 1){
      //Ruta de vehiculo pura
      if (contVC == ruta.clientes.size()){
        for (int i=0; i<ruta.clientes.size()-1; i++){
        
        addClientToRoute(secuencia_ruta,matrixDistances,ruta.clientes, distanceTotal);
        }
        routeTsp.clientes = secuencia_ruta;
        routeTsp.type_print = "V";
        routeTsp.type = "VP";
        routeTsp.id_semilla = ruta.id_semilla;
        routeTsp.id_subtour = -1;
        routeTsp.clientes.push_back(0);
        routeTsp.distance=distanceTotal;
        
      }
      // Ruta de vehiculo completa
      else{
        // Crear main tour de los clientes de vehiculo
        for (int i=0; i<indicesVC.size()-1;i++){
          
          addClientToRoute(secuencia_ruta,matrixDistancesVC,indicesVC, distanceTotal);
        }
        routeTsp.clientes = secuencia_ruta;
        routeTsp.type_print = "V";
        routeTsp.type = "VC";
        routeTsp.id_semilla = ruta.id_semilla;
        routeTsp.clientes.push_back(0);
        routeTsp.id_subtour = -1;
        routeTsp.distance=distanceTotal;
        
        
        //iniciar los subtours iterando sobre los clientes camion
        for(int i=0; i<indicesTC.size();i++){
          int index= getIndex_int (ruta.clientes,clientesTC[i].id);
          vector<float> distances = matrixDistances[index];
          vector<float> auxDistances = matrixDistances[index];
          sort(distances.begin(),distances.end());
          RouteTSP subtour;
          subtour.distance=0;
          

          // Si es el primer cliente de camión buscar el nodo más cercano al main tour
          if (i==0){
            for (auto & d : distances){
              int index_distance = getIndex (auxDistances,d);
              if (count(indicesVC.begin(), indicesVC.end(), ruta.clientes[index_distance]) && d!=0 && ruta.clientes[index_distance] !=0){
                
                subtour.clientes.push_back(ruta.clientes[index_distance]);
                subtour.clientes.push_back(clientesTC[i].id);
                subtour.id_semilla;
                subtour.distance += d;
                subtour.type = "T";
                subtour.id_subtour = contador;
                Subtours.push_back(subtour);
                contador++;
                break;
              }
            }
          }
          else {         
            for (auto & d : distances){
              //Buscar el id del cliente más cercano al cliente de camión
              int index_distance = getIndex (auxDistances,d);
              //Si ese cliente es del main tour, crear un nuevo subtour
              if (count(indicesVC.begin(), indicesVC.end(), ruta.clientes[index_distance]) && d!=0 && ruta.clientes[index_distance] !=0 && ClientInRoute(Subtours, clientesTC[i].id) ){
                subtour.clientes.push_back(ruta.clientes[index_distance]);
                subtour.clientes.push_back(clientesTC[i].id);
                subtour.id_semilla;
                subtour.type = "T";
                subtour.id_subtour = contador;
                contador++;
                subtour.distance += d;
                
                Subtours.push_back(subtour);
                break;
              }
              //Unir el cliente de camión a un subtour ya creado
              else if ( d!=0){
                for(auto & subtour : Subtours){
                  if (subtour.clientes[subtour.clientes.size()-1] == ruta.clientes[index_distance]  && ClientInRoute(Subtours, clientesTC[i].id)){
                    subtour.clientes.push_back(clientesTC[i].id);
                    subtour.distance += d;
                    
                    break;
                  }
                }
              }
            }
          }
        }  
      }
    }
    listRoutes.push_back (routeTsp);
  }
  
  // AGREGAR DISTANCIA A LA SUBRUTA DEL NODO FINAl CON EL INICIAL
  Distancefinal_init (listRoutes, Subtours,all_distanceMatrix);
  
  
}

// Verificar que se siga satisfaciendo la demanda de la ruta
bool MovimientoFactible(RouteTSP ruta_tsp1, RouteTSP ruta_tsp2 ,int cliente1, int cliente2,Node nodos[]){
  
  ruta_tsp1.capacidad_disponible += nodos[cliente1].demand;
  ruta_tsp1.capacidad_disponible -= nodos[cliente2].demand;

  ruta_tsp2.capacidad_disponible += nodos[cliente2].demand;
  ruta_tsp2.capacidad_disponible -= nodos[cliente1].demand;

  if (ruta_tsp1.capacidad_disponible >=0 && ruta_tsp2.capacidad_disponible >=0 ){
    return true;
  }
  else{
    return false;
  }


}

float calculateAlldistance(int cliente1, int cliente2, vector<RouteTSP> listRoutes,vector<RouteTSP> Subtours, int flag, float distance1, float distance2 ){
  float suma_total =0;

  if (flag ==2){
    for (auto & ruta: listRoutes){
      if (find(ruta.clientes.begin(),ruta.clientes.end(),cliente1) != ruta.clientes.end()){
        suma_total += distance1;
      }
      else{
        suma_total += ruta.distance;
      }
    }

    for (auto & ruta: Subtours){
      if (find(ruta.clientes.begin(),ruta.clientes.end(),cliente1) != ruta.clientes.end()){
        suma_total += distance1;
      }
      else{
        suma_total += ruta.distance;
      }
    }
    return suma_total;
  }


  else{
    for (auto & ruta: listRoutes){
      if (find(ruta.clientes.begin(),ruta.clientes.end(),cliente1) != ruta.clientes.end()){
        suma_total += distance1;
      }
      else if (find(ruta.clientes.begin(),ruta.clientes.end(),cliente2) != ruta.clientes.end()){
        suma_total+=distance2;
      }
      else{
        suma_total += ruta.distance;
      }
    }

    for (auto & ruta: Subtours){
      if (find(ruta.clientes.begin(),ruta.clientes.end(),cliente1) != ruta.clientes.end()){
        suma_total += distance1;
      }
      else if (find(ruta.clientes.begin(),ruta.clientes.end(),cliente2) != ruta.clientes.end()){
        suma_total += distance2;
      }
      else{
        suma_total += ruta.distance;
      }
    }
    return suma_total;
    
  }
}

tuple<vector<int>,vector<int>,float,float,float> changeDistance(vector<vector<float>> distanceMatrix, int cliente1, int cliente2, RouteTSP ruta_tsp1, RouteTSP ruta_tsp2, int flag, vector<RouteTSP> listRoutes,vector<RouteTSP> Subtours){
  
  // computar un cambio de distancia entre dos rutas diferentes
  if (flag == 1){
  vector<int> nueva_ruta1;
  vector<int> nueva_ruta2;
  float sum_distancia1=0;
  float sum_distancia2=0;

  // agregar el cliente 2 a la ruta 1 en la posicion correspondiente
  for (auto &cliente : ruta_tsp1.clientes){
    if ( cliente != cliente1){
      nueva_ruta1.push_back(cliente);
    }
    else{
      nueva_ruta1.push_back(cliente2);
    }
  }

  // agregar cliente 1 a la ruta 2 en la posicion correspondiente
  for (auto &cliente : ruta_tsp2.clientes){
    if ( cliente != cliente2){
      nueva_ruta2.push_back(cliente);
    }
    else{
        nueva_ruta2.push_back(cliente1);
      }
    }

    //computar los nuevos costos  ruta 1
    int cont =0;
    for (auto & cliente : nueva_ruta1){
      
      int indice_cliente = getIndex_int(nueva_ruta1,cliente);
      if (cont == nueva_ruta1.size()-1){
        int x=2;
      }
      else{
        int index_siguiente = nueva_ruta1[indice_cliente+1];
        float distance = distanceMatrix[cliente][index_siguiente];
        sum_distancia1 += distance;
      }
      cont++;
      
    }

    // computar los costos ruta 2
    cont =0;
    for (auto & cliente : nueva_ruta2){
      int indice_cliente = getIndex_int(nueva_ruta2,cliente);
      if (cont == nueva_ruta2.size()-1){
        int x=2;
      }
      else{
        int index_siguiente = nueva_ruta2[indice_cliente+1];
        float distance = distanceMatrix[cliente][index_siguiente];
        sum_distancia2 += distance;
      }
      cont++;
    }
    float sum_total = calculateAlldistance(cliente1, cliente2, listRoutes,Subtours, 1, sum_distancia1,  sum_distancia2);
    return make_tuple(nueva_ruta1,nueva_ruta2,sum_total,sum_distancia1,sum_distancia2);
  }
  
  // computar cambio de distancia en la misma ruta

  else {
    vector<int> nueva_ruta1;
    float sum_distancia1=0;
    for (auto &cliente : ruta_tsp1.clientes){
      if ( cliente == cliente1){
        nueva_ruta1.push_back(cliente2);
      }
      else if (cliente == cliente2){
        nueva_ruta1.push_back(cliente1);
      }
      else{
        nueva_ruta1.push_back(cliente);
      }
    }

    int cont =0;
    for (auto & cliente : nueva_ruta1){
      
      int indice_cliente = getIndex_int(nueva_ruta1,cliente);
      if (cont == nueva_ruta1.size()-1){
        int x=2;
      }
      else{
        int index_siguiente = nueva_ruta1[indice_cliente+1];
        float distance = distanceMatrix[cliente][index_siguiente];
        sum_distancia1 += distance;
      }
      cont++;
    } 
    /*for(auto &cliente : nueva_ruta1){
        cout<< cliente<<"-";
      }
      cout<<endl;*/
    float sum_total = calculateAlldistance(cliente1, cliente2, listRoutes,Subtours, 2, sum_distancia1,  0);
    return make_tuple(nueva_ruta1,nueva_ruta1,sum_total,sum_distancia1,0);

  }
}

// Funcion para ejecutar el movimiento y calcular las nuevas distancias de la ruta
void ejecutarMejorMovimiento( vector<vector<float>> distanceMatrix,tuple<float,int,int,RouteTSP,RouteTSP> movimiento,vector<RouteTSP> &listRoutes,vector<RouteTSP> &listSubtours){
  float distancia_total = get<0> (movimiento);
  int cliente_1 = get<1> (movimiento);
  int cliente_2 = get<2> (movimiento);
  RouteTSP rutatsp_1 = get<3> (movimiento);
  RouteTSP rutatsp_2 = get<4> (movimiento);

  vector<int> nueva_ruta1;
  vector<int> nueva_ruta2;
  float suma_total=0;
  float suma_distancia1=0;
  float suma_distancia2=0;
  if (rutatsp_1.id_semilla == rutatsp_2.id_semilla && rutatsp_1.id_subtour == rutatsp_2.id_subtour){
    auto tuple = changeDistance(distanceMatrix,cliente_1,cliente_2,rutatsp_1,rutatsp_2,2,listRoutes,listSubtours);
    nueva_ruta1 = get<0> (tuple);
    nueva_ruta2 = get<1> (tuple);
    suma_total = get<2> (tuple);
    suma_distancia1 = get<3> (tuple);
    suma_distancia2 = get<4> (tuple);
  }
  else{
    auto tuple = changeDistance(distanceMatrix,cliente_1,cliente_2,rutatsp_1,rutatsp_2,1,listRoutes,listSubtours);
    nueva_ruta1 = get<0> (tuple);
    nueva_ruta2 = get<1> (tuple);
    suma_total = get<2> (tuple);
    suma_distancia1 = get<3> (tuple);
    suma_distancia2 = get<4> (tuple);
  }

  for (auto &ruta: listRoutes){
    if (ruta.id_semilla == rutatsp_1.id_semilla && rutatsp_1.type!="T"){
      ruta.clientes = nueva_ruta1;
      ruta.distance = suma_distancia1;
    }
    else if (ruta.id_semilla == rutatsp_2.id_semilla && rutatsp_2.type!="T"){
      if (suma_distancia2 !=0){
        ruta.clientes = nueva_ruta2;
        ruta.distance = suma_distancia2;
      }
      
    }
  }
  
  for (auto &ruta: listSubtours){
    if (ruta.id_subtour == rutatsp_1.id_subtour&& rutatsp_1.type=="T"){
      
      ruta.clientes = nueva_ruta1;
      ruta.distance = suma_distancia1;
    }
    else if (ruta.id_subtour == rutatsp_2.id_subtour && rutatsp_2.type=="T"){
      if (suma_distancia2 !=0){
        
        ruta.clientes = nueva_ruta2;
        ruta.distance = suma_distancia2;
      }
      
    }
  }
  
}

float calcularDistancia (vector<int> ruta, vector<vector<float>> distanceMatrix){
  int cont =0;
  float sum_distancia =0;
    for (auto & cliente : ruta){
      
      int indice_cliente = getIndex_int(ruta,cliente);
      if (cont == ruta.size()-1){
        int x=0;
      }
      else{
        int index_siguiente = ruta[indice_cliente+1];
        float distance = distanceMatrix[cliente][index_siguiente];
        sum_distancia += distance;
      }
      cont++;
    }
    return sum_distancia;
}

// Funcion que genera una nueva ruta aleatoria
void shuffleVector(vector<RouteTSP> &listRoutes,vector<RouteTSP> &Subtours,vector<vector<float>> distanceMatrix){

  srand((unsigned) time(NULL));
  for (auto &ruta : listRoutes){
    vector<int> aux_list;
    for (auto &cliente : ruta.clientes){
      if (cliente != ruta.clientes[0]){
        aux_list.push_back(cliente);
      }
    }
    random_shuffle(aux_list.begin(), aux_list.end());
    aux_list.insert(aux_list.begin()+0,0);
    aux_list.insert(aux_list.begin()+aux_list.size(),0);
    ruta.clientes = aux_list;
    ruta.distance = calcularDistancia(ruta.clientes, distanceMatrix);
  }

  for (auto &ruta : Subtours){
    vector<int> aux_list;
    for (auto &cliente : ruta.clientes){
      if (cliente != ruta.clientes[0]){
        aux_list.push_back(cliente);
      }
    }
    
    random_shuffle(aux_list.begin(), aux_list.end());
    aux_list.insert(aux_list.begin()+0,ruta.clientes[0]);
    aux_list.insert(aux_list.begin()+aux_list.size(),ruta.clientes[0]);
    ruta.clientes = aux_list;
    ruta.distance = calcularDistancia(ruta.clientes, distanceMatrix);
  }

}


void HillClimber(int iteraciones, vector<vector<float>> distanceMatrix, vector<RouteTSP> &listRoutes,vector<RouteTSP> &Subtours , Node nodos[], int restart){

  int contador =0;

  while(contador<restart){
    bool flag = true;
    while(flag){
      vector<tuple<float,int,int,RouteTSP,RouteTSP>> listMovimientos;
      float actual_min= allDistance(listRoutes,Subtours); 
      for (auto &ruta : listRoutes){
        
        // Movimientos de clientes, cliente inicial, cliente final, funcion de evaluación(distancia)
        for (auto &cliente : ruta.clientes){
          // lista de movimientos de cada cliente
          for (auto & ruta2 : listRoutes){
              for(auto & cliente2 : ruta2.clientes ){
                // ruta vehiculo pura solo puedo incluir clientes 0
                if (ruta.type== "VP" || ruta.type == "VC"){
                  if (nodos[cliente2].type == 0  && cliente !=0 && cliente2 !=0){
                    if (ruta.id_semilla != ruta2.id_semilla){
                      if (MovimientoFactible(ruta, ruta2 ,cliente, cliente2,nodos)){
                      // m1  mover los 
                      // m2
                      // computar costos de m1 y m2
                      auto tuple = changeDistance(distanceMatrix,cliente,cliente2,ruta,ruta2,1,listRoutes,Subtours);
                      float suma_total= get<2> (tuple);
                      
                      
                      listMovimientos.push_back(make_tuple(suma_total,cliente,cliente2,ruta,ruta2));
                      }
                    }
                    // agregar movimientos de los nodos de la misma ruta
                    else if (ruta.id_semilla == ruta2.id_semilla && cliente != cliente2){
                      auto tuple = changeDistance(distanceMatrix,cliente,cliente2,ruta,ruta2,2,listRoutes,Subtours);
                      float suma_total = get<2> (tuple);
                      listMovimientos.push_back(make_tuple(suma_total,cliente,cliente2,ruta,ruta2));
                    }
                  }
                }
                else if (ruta.type == "TP" && cliente !=0 && cliente2 !=0 ){
                  if (ruta.id_semilla != ruta2.id_semilla){
                      if (MovimientoFactible(ruta, ruta2 ,cliente, cliente2,nodos)){
                      // m1  mover los 
                      // m2
                      // computar costos de m1 y m2
                        auto tuple = changeDistance(distanceMatrix,cliente,cliente2,ruta,ruta2,1,listRoutes,Subtours);
                        float suma_total = get<2> (tuple);
                        
                        listMovimientos.push_back(make_tuple(suma_total,cliente,cliente2,ruta,ruta2));
                      }
                    }
                    else if (ruta.id_semilla == ruta2.id_semilla && cliente != cliente2){
                      auto tuple = changeDistance(distanceMatrix,cliente,cliente2,ruta,ruta2,2,listRoutes,Subtours);
                      float suma_total = get<2> (tuple);
                      listMovimientos.push_back(make_tuple(suma_total,cliente,cliente2,ruta,ruta2));
                    }
                }
              }
            
          }
          
        } 
      }
      
      // ejecutar movimiento con los subtours ahora

      for ( auto &ruta : Subtours){
        for (auto & cliente : ruta.clientes){
          if (cliente != ruta.clientes[0]){
            for (auto & ruta2 : Subtours){
              for (auto & cliente2 : ruta2.clientes){
                if (cliente2 != ruta2.clientes[0]){
                  if (ruta.id_subtour!= ruta2.id_subtour ){
                    auto tuple = changeDistance(distanceMatrix,cliente,cliente2,ruta,ruta2,1,listRoutes,Subtours);
                    float suma_total = get<2> (tuple);
                    
                    listMovimientos.push_back(make_tuple(suma_total,cliente,cliente2,ruta,ruta2)); 
                  }
                  else if (ruta.id_subtour == ruta2.id_subtour && cliente != cliente2){
                    auto tuple = changeDistance(distanceMatrix,cliente,cliente2,ruta,ruta2,2,listRoutes,Subtours);
                    float suma_total = get<2> (tuple);
                    listMovimientos.push_back(make_tuple(suma_total,cliente,cliente2,ruta,ruta2));
                  }
                }
              }
            } 
            for (auto &ruta3 : listRoutes){
                if (ruta3.type == "TP"){
                  for (auto &cliente3 : ruta3.clientes){
                    
                    if(cliente3 != 0 && MovimientoFactible(ruta,ruta3,cliente,cliente3,nodos)){
                      auto tuple = changeDistance(distanceMatrix,cliente,cliente3,ruta,ruta3,1,listRoutes,Subtours);
                      float suma_total = get<2> (tuple);
                      
                      listMovimientos.push_back(make_tuple(suma_total,cliente,cliente3,ruta,ruta3));
                    }
                  
                  }
                    
                }
              }
          }
        }
      }
      // Encontrar la minima distancia
      float init = 1000000000;
      tuple<float,int,int,RouteTSP,RouteTSP> best_movement;

      for (auto & movimiento : listMovimientos){
        float suma_total = get<0> (movimiento);
        if (suma_total <= init){
          init = suma_total;
          best_movement = movimiento;
        }
      } 

      float distance_mejor_movimiento = get<0> (best_movement);
      float cliente1 = get<1> (best_movement);
      float cliente2 = get<2> (best_movement);
      RouteTSP clientetsp_1 = get<3> (best_movement);
      RouteTSP clientetsp_2 = get<4> (best_movement);
      if (distance_mejor_movimiento< actual_min){
        
        ejecutarMejorMovimiento(distanceMatrix, best_movement,listRoutes,Subtours);
      }
      else{
        flag = false;
      }
      
      
    }
    cout<<endl;
    cout<<"Costo/Distancia total de viaje: "<<allDistance(listRoutes,Subtours)<<endl;
    printRuta(listRoutes,Subtours);
    shuffleVector(listRoutes,Subtours,distanceMatrix);
    contador ++;
    
  }
  

  


}
