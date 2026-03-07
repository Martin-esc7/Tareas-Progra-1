#include <iostream>
using namespace std;

void saludo(){
    cout << "Hola mundo\nBienvenidos a Programacion 1\n";
}

void suma(){
    int num1, num2;
    cout << "\n--- SUMA ---\n";
    cout << "Ingresar numero 1: ";
    cin >> num1;
    cout << "Ingrese numero 2: ";
    cin >> num2;
    
    int resultado = num1 + num2;
    cout << "El resultado es: " << resultado << "\n";
}

void datos(){
    cout << "\n--- DATOS PERSONALES ---\n";
    cout << "Nombre:\t Martin\n";
    cout << "Edad:\t 21\n";
    cout << "Curso:\t C++\n"; 
    cout << "\"C++ es genial\"\n";
}

void despedida(){
    cout << "\n--- FIN DEL PROGRAMA ---\n";
    cout << "Gracias por usar el programa!\n";
}

int main(){
    saludo();
    suma();
    datos();
    despedida();
    
    return 0;
}