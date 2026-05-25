#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

using namespace std;

struct Cancion {
    string nombre;
    string artista;
    string duracion;
    Cancion* siguiente;
    Cancion* anterior;
};

class Reproductor {
private:
    Cancion* cabeza;
    int totalCanciones;

public:
    Reproductor() {
        cabeza = nullptr;
        totalCanciones = 0;
    }

    ~Reproductor() {
        if (cabeza == nullptr) return;
        Cancion* actual = cabeza;
        Cancion* sig = nullptr;
        cabeza->anterior->siguiente = nullptr;
        while (actual != nullptr) {
            sig = actual->siguiente;
            delete actual;
            actual = sig;
        }
    }

    void agregarCancion(string nombre, string artista, string duracion) {
        // crear nuevo nodo
        Cancion* nuevo = new Cancion();
        nuevo->nombre = nombre;
        nuevo->artista = artista;
        nuevo->duracion = duracion;
        nuevo->siguiente = nullptr;
        nuevo->anterior = nullptr;

        if (cabeza == nullptr) {
            
            nuevo->siguiente = nuevo;
            nuevo->anterior = nuevo;
            cabeza = nuevo;
        } else {
            
            Cancion* ultimo = cabeza->anterior;
            ultimo->siguiente = nuevo;
            nuevo->anterior = ultimo;
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
        }

        totalCanciones++;
        cout << "\n  Cancion agregada correctamente.\n";
    }

    void mostrarLista() {
        if (cabeza == nullptr) {
            cout << "\n  La playlist esta vacia.\n";
            return;
        }

        cout << "\n  === PLAYLIST ===\n\n";

        Cancion* actual = cabeza;
        int numero = 1;

        
        do {
            cout << "  [" << numero << "] " << actual->nombre << "\n";
            cout << "      Artista : " << actual->artista << "\n";
            cout << "      Duracion: " << actual->duracion << "\n\n";
            actual = actual->siguiente;
            numero++;
        } while (actual != cabeza);

        cout << "  Total: " << totalCanciones << " cancion(es)\n";
    }

    void reproducirPlaylist() {
        if (cabeza == nullptr) {
            cout << "\n  No hay canciones en la playlist.\n";
            system("pause");
            return;
        }

        Cancion* actual = cabeza;
        bool reproduciendo = true;

        while (reproduciendo) {
            system("cls");

            cout << "\n  === REPRODUCIENDO ===\n\n";
            cout << "  Cancion : " << actual->nombre << "\n";
            cout << "  Artista : " << actual->artista << "\n";
            cout << "  Duracion: " << actual->duracion << "\n\n";
            cout << "  [<-] Anterior   [->] Siguiente   [ESC] Menu\n";

            int tecla = _getch();

            if (tecla == 224) {
                int especial = _getch();
                if (especial == 75) {
                    actual = actual->anterior;
                } else if (especial == 77) {
                    actual = actual->siguiente; 
                }
            } else if (tecla == 27) {
                reproduciendo = false;
            }
        }
    }
};

void mostrarMenu() {
    cout << "\n  === REPRODUCTOR DE MUSICA ===\n\n";
    cout << "  1. Agregar cancion\n";
    cout << "  2. Mostrar lista completa\n";
    cout << "  3. Reproducir playlist\n";
    cout << "  4. Salir\n\n";
    cout << "  Seleccione una opcion: ";
}

int main() {
    Reproductor reproductor;
    int opcion;

    do {
        system("cls");
        mostrarMenu();
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: {
                string nombre, artista, duracion;
                system("cls");
                cout << "\n  -- Agregar cancion --\n\n";
                cout << "  Nombre: ";
                getline(cin, nombre);
                cout << "  Artista: ";
                getline(cin, artista);
                cout << "  Duracion (ej: 3:45): ";
                getline(cin, duracion);
                reproductor.agregarCancion(nombre, artista, duracion);
                system("pause");
                break;
            }
            case 2: {
                system("cls");
                reproductor.mostrarLista();
                system("pause");
                break;
            }
            case 3: {
                reproductor.reproducirPlaylist();
                break;
            }
            case 4: {
                cout << "\n  Hasta luego.\n\n";
                break;
            }
            default: {
                cout << "\n  Opcion no valida.\n";
                system("pause");
            }
        }

    } while (opcion != 4);

    return 0;
}