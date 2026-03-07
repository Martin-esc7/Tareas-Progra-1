#include <iostream>
using namespace std;
void saludo(){

    cout<<"Hola mundo\nBienvenidos a Programacion 1" <<endl;
}

void suma(){

    int num1, num2, suma;
    cout<<"Ingresar numero 1:  " <<endl;
    cin>>num1;
    cout<<"Ingrese numero 2: " <<endl;
    cin>>num2;

   
     suma = num1 + num2;

    cout<<"El resultado es: "  <<suma <<endl;

}

void datos(){

        cout<<"Hola mundo\nBienvenidos a Programacion 1" <<endl;

    cout<<"Nombre:\t Martin" <<endl;
    cout<<"Edad:\t 21" <<endl;
    cout<<"Curso:\t C++" <<endl; 
    
    cout<<"\"c++ es genial\"" <<endl;
}




int main ()
{
    saludo();
    suma();
    datos();

    return 0;

}

