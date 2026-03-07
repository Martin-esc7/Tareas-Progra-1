#include <iostream>
#include <string>
using namespace std;

int main ()
{
    string bin;
    long long decimal = 0;

    cout << "Ingrese un numero binario (ejemplo: 1011): ";
    cin >> bin;


    for (char c : bin){
        if(c != '0' && c != '1')
        {
            cout<<"Error; cadena no es binaria";
            return 0;
        }
        decimal = decimal * 2 +(c - '0');
       
    }

    cout<< "Decimal: " <<decimal<<"\n";

    return 0;
}    