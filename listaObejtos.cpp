#include <iostream>
#include <string>
using namespace std;

class Alumno {
public:
    string nombres;
    string apellidos;
    string carnet;
    string email;
    string telefono;
    float notas[5];

    float promedio() {
        float suma = 0;
        for (int i = 0; i < 5; i++)
            suma += notas[i];
        return suma / 5;
    }
};

struct Nodo {
    Alumno alumno;
    Nodo* siguiente;
};

int main() {
    Nodo* cabeza = nullptr;
    Nodo* ultimo = nullptr;
    char op;

    do {
        Alumno a;

        cout << "\n-- Ingresar datos del alumno --" << endl;
        cout << "Nombres: ";
        cin.ignore();
        getline(cin, a.nombres);
        cout << "Apellidos: ";
        getline(cin, a.apellidos);
        cout << "Carnet: ";
        getline(cin, a.carnet);
        cout << "Email: ";
        getline(cin, a.email);
        cout << "Telefono: ";
        getline(cin, a.telefono);

        for (int i = 0; i < 5; i++) {
            float nota;
            do {
                cout << "Nota " << i + 1 << " (0-10): ";
                cin >> nota;
                if (nota < 0 || nota > 10)
                    cout << "Nota invalida, intente de nuevo." << endl;
            } while (nota < 0 || nota > 10);
            a.notas[i] = nota;
        }

        Nodo* nuevo = new Nodo();
        nuevo->alumno = a;
        nuevo->siguiente = nullptr;

        if (cabeza == nullptr) {
            cabeza = nuevo;
            ultimo = nuevo;
        } else {
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
        }

        cout << "Desea agregar otro alumno? (S/N): ";
        cin >> op;

    } while (op == 'S' || op == 's');

    cout << "\n========== Lista de Alumnos ==========" << endl;

    Nodo* temp = cabeza;
    while (temp != nullptr) {
        Alumno a = temp->alumno;
        cout << "\nNombres: " << a.nombres << endl;
        cout << "Apellidos: " << a.apellidos << endl;
        cout << "Carnet: " << a.carnet << endl;
        cout << "Email: " << a.email << endl;
        cout << "Telefono: " << a.telefono << endl;
        cout << "Notas: ";
        for (int i = 0; i < 5; i++)
            cout << a.notas[i] << " ";
        cout << endl;
        cout << "Promedio: " << a.promedio() << endl;
        temp = temp->siguiente;
    }

    Nodo* aux;
    while (cabeza != nullptr) {
        aux = cabeza;
        cabeza = cabeza->siguiente;
        delete aux;
    }

    return 0;
}