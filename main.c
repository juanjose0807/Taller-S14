#include <stdio.h>
#include "funciones.h"

int main() {
    int opcion;
    do {
        opcion = menu();
        switch (opcion) {
            case 1:
                createFactura();
                break;
            case 2:
                readFactura();
                break;
            case 3:
                editarFactura();
                break;
            case 4:
                eliminarFactura();
                break;
            case 5:
                break;
            default:
                printf("Opcion no valida\n");
        }
    } while (opcion != 5);
    return 0;
}   