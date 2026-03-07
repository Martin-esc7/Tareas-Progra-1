#include <iostream>

using namespace std;

int main() {
   
    int numero;
    
    cout << "Ingresar un numero positivo: ";
    cin >> numero;

    
    int lista[numero];

    
    for (int i = 0; i < numero; i++) {
        lista[i] = numero - i; 
    }

    
    cout << "Los numeros son: ";
    for (int i = 0; i < numero; i++) {
        cout << lista[i];

        
        if (i < numero - 1) {
            cout << ", ";
        }
    }

    return 0;
}