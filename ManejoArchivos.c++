#include <iostream>
#include <fstream>
#include <string>

using namespace std;


struct Alumno {
    int id;
    char nombre[30];
    double nota; 
};

int main() {
    int opcion;
    
    
    while (true) {
        cout << "--- MENU DE MI TAREA ---" << endl;
        cout << "1. Escribir en alumnos" << endl;
        cout << "2. Ver todo el archivo de texto" << endl;
        cout << "3. Buscar alumno por ID" << endl;
        cout << "4. Cambiar nota" << endl;
        cout << "5. Guardar en binario" << endl;
        cout << "6. Leer un registro especifico binario" << endl;
        cout << "7. Ver todo el binario" << endl;
        cout << "8. Salir" << endl;
        cout << "Elija una opcion: ";
        cin >> opcion;

        if (opcion == 8) {
            break; 
        }

        if (opcion == 1) {
            ofstream miArchivo;
            miArchivo.open("alumnos.txt", ios::app);
            int id; string nom; double n;
            
            cout << "Ingrese ID: "; cin >> id;
            cout << "Ingrese Nombre: "; cin >> nom;
            cout << "Ingrese Nota: "; cin >> n;
            
            miArchivo << id << " " << nom << " " << n << endl;
            miArchivo.close();
            cout << "Listo, se guardo." << endl;
        }

        if (opcion == 2) {
            ifstream leer;
            leer.open("alumnos.txt");
            int id; string nom; double n;
            
            cout << "Lista de alumnos en el TXT:" << endl;
            while (leer >> id >> nom >> n) {
                cout << "ID: " << id << " Nombre: " << nom << " Nota: " << n << endl;
            }
            leer.close();
        }

        if (opcion == 3) {
            ifstream buscar;
            buscar.open("alumnos.txt");
            int idABuscar, idFile; 
            string nomFile; double notaFile;
            bool siEsta = false;

            cout << "Que ID buscas? "; cin >> idABuscar;

            while (buscar >> idFile >> nomFile >> notaFile) {
                if (idFile == idABuscar) {
                    cout << "Lo encontre! Se llama " << nomFile << " y saco " << notaFile << endl;
                    siEsta = true;
                }
            }
            if (siEsta == false) {
                cout << "No existe ese alumno." << endl;
            }
            buscar.close();
        }

        if (opcion == 4) {
            ifstream original;
            ofstream temp;
            original.open("alumnos.txt");
            temp.open("temp.txt");

            int idMod, idF; string nomF; double nF, nuevaN;
            cout << "ID del alumno para cambiar nota: "; cin >> idMod;
            cout << "Cual es la nueva nota? "; cin >> nuevaN;

            while (original >> idF >> nomF >> nF) {
                if (idF == idMod) {
                    temp << idF << " " << nomF << " " << nuevaN << endl;
                } else {
                    temp << idF << " " << nomF << " " << nF << endl;
                }
            }
            original.close();
            temp.close();

            remove("alumnos.txt");
            rename("temp.txt", "alumnos.txt");
            cout << "Archivo actualizado." << endl;
        }

        if (opcion == 5) {
            ofstream bin;
            bin.open("alumnos.dat", ios::binary | ios::app);
            Alumno al;
            string auxNombre;

            cout << "ID (Binario): "; cin >> al.id;
            cout << "Nombre (Binario): "; cin >> auxNombre;
            // Un novato solo copia caracteres uno por uno o usa loops simples
            for(int i=0; i<30; i++) al.nombre[i] = '\0';
            for(int i=0; i<auxNombre.length() && i<29; i++) {
                al.nombre[i] = auxNombre[i];
            }
            cout << "Nota (Binario): "; cin >> al.nota;

            bin.write((char*)&al, sizeof(Alumno));
            bin.close();
        }

        if (opcion == 6) {
            ifstream binLeer;
            binLeer.open("alumnos.dat", ios::binary);
            Alumno al;
            int numReg;

            cout << "Que numero de registro quieres ver? (0, 1, 2...): ";
            cin >> numReg;

            // Uso de seekg para ir directo al lugar
            binLeer.seekg(numReg * sizeof(Alumno), ios::beg);
            
            if (binLeer.read((char*)&al, sizeof(Alumno))) {
                cout << "Datos: " << al.nombre << " con ID " << al.id << endl;
            } else {
                cout << "Ese registro no existe." << endl;
            }
            binLeer.close();
        }

        if (opcion == 7) {
            ifstream todoBin;
            todoBin.open("alumnos.dat", ios::binary);
            Alumno al;
            
            cout << "Todo lo que hay en el archivo binario:" << endl;
            while (todoBin.read((char*)&al, sizeof(Alumno))) {
                cout << "ID: " << al.id << " Nom: " << al.nombre << " Nota: " << al.nota << endl;
            }
            todoBin.close();
        }

        cout << endl; 
    }

    return 0;
}