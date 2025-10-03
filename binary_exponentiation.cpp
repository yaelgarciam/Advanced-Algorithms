#include <iostream>

long binaryPower(int x, int n){
    if (n == 0){
        return 1;
    }
    if (n%2 == 0){
        long y = binaryPower(x, n/2);
        return y*y;
    } else {
        long y = binaryPower(x, (n-1)/2);
        return y*y*x;
    }
}

int main (){
    int j = 12;
    for (int i = 1; i <= 10; i++){
        std::cout << "operación: " << i << "^" << j - 1 << std::endl;
        std::cout << "Exponenciación Binaria: " << binaryPower(i, j - 1) << std::endl;
        std::cout << "Uso de pow: " << pow(i, j - 1) << std::endl;
        j -= 1;
    }

    return 0;
}