#include <iostream>
using namespace std;

struct Nodo {
    int dato;
    Nodo* siguiente;
};

int main() {
    Nodo* cabeza = nullptr;
    Nodo* ultimo = nullptr;
    char op;
    int val;

    do {
        cout << "Desea ingresar un nuevo valor? (S/N): ";
        cin >> op;

        if (op == 'S' || op == 's') {
            cout << "Ingrese el valor: ";
            cin >> val;

            Nodo* nuevo = new Nodo();
            nuevo->dato = val;
            nuevo->siguiente = nullptr;

            if (cabeza == nullptr) {
                cabeza = nuevo;
                ultimo = nuevo;
            } else {
                ultimo->siguiente = nuevo;
                ultimo = nuevo;
            }
        }

    } while (op != 'N' && op != 'n');

    cout << "\nContenido de la lista:" << endl;

    Nodo* temp = cabeza;
    while (temp != nullptr) {
        if (temp->siguiente != nullptr)
            cout << "[" << temp->dato << " | *] --> ";
        else
            cout << "[" << temp->dato << " | NULL]";
        temp = temp->siguiente;
    }
    cout << endl;

    Nodo* aux;
    while (cabeza != nullptr) {
        aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
    }

    return 0;
}