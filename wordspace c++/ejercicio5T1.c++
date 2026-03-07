#include <iostream>
using namespace std;

int addFive(int num){
    num = num + 5;   
    return num;
}

int main(){
    int numero;
    
    cout << "ingresa un numero: ";
    cin >> numero;
    
    int resultado = addFive(numero);
    
    cout << "el numero mas 5 es: " << resultado << endl;
    
    return 0;
}