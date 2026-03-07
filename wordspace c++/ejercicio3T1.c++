#include <iostream>
using namespace std;

int main(){
    int cantidad;
    
    cout << "cuantos numeros quieres ingresar: ";
    cin >> cantidad;
    
    int numeros[100];
    int inversos[100];
    
    for(int i = 0; i < cantidad; i++){
        cout << "ingresa el numero " << i+1 << ": ";
        cin >> numeros[i];
    }
    
    for(int i = 0; i <= cantidad; i++){   // bug: deberia ser < no <=
        inversos[i] = numeros[i] * -1;
    }
    
    cout << "arreglo original: ";
    for(int i = 0; i < cantidad; i++){
        cout << numeros[i] << " ";
    }
    cout << endl;
    
    cout << "inversos: ";
    for(int i = 0; i < cantidad; i++){
        cout << inversos[i] << " ";
    }
    cout << endl;
    
    return 0;
}