#include <iostream>
#include <string>
using namespace std;

int main(){
    string binario;
    int decimal = 0;
    int potencia = 1;
    
    cout << "ingresa el numero binario: ";
    cin >> binario;
    
    int longitud = binario.length();
    
    for(int i = longitud - 1; i >= 0; i--){
        if(binario[i] == '1'){
            decimal = decimal + potencia;
        }
        potencia = potencia * 2;
    }
    
    cout << "el numero en decimal es: " << decimal << endl;
    
    return 0;
}