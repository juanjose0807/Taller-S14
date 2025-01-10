#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu() {
    int opcion;
    do {
        printf("1. Crear factura\n");
        printf("2. Leer facturas\n");
        printf("3. Editar factura\n");
        printf("4. Eliminar factura\n");
        printf("5. Salir\n");
        printf(">> ");
        scanf("%d", &opcion);
        if (opcion < 1 || opcion > 5) {
            printf("Por favor, elija una opcion valida.\n");
        }
    } while (opcion < 1 || opcion > 5);
    return opcion;
}

void saveFactura(struct Factura *factura) {
    FILE *file = fopen("factura.dat", "ab+");
    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    fwrite(factura, sizeof(struct Factura), 1, file);
    printf("Factura guardada\n");
    fclose(file);
}

void leerCadena(char *cadena, int num) {
    fflush(stdin);
    fgets(cadena, num, stdin);
    int len = strlen(cadena) - 1;
    if (len >= 0 && cadena[len] == '\n') {
        cadena[len] = '\0';
    }
}

void createFactura() {
    struct Factura factura;
    printf("Nombre del cliente: ");
    leerCadena(factura.nombre, 20);
    do {
        printf("Cedula del cliente: ");
        scanf("%d", &factura.cedula);
        if (factura.cedula < 0) {
            printf("La cedula no puede ser negativa. Intente de nuevo.\n");
        }
    } while (factura.cedula < 0);

    do {
        printf("Numero de productos: ");
        scanf("%d", &factura.numProductos);
        if (factura.numProductos < 0) {
            printf("El numero de productos no puede ser negativo. Intente de nuevo.\n");
        }
    } while (factura.numProductos < 0);

    factura.total = 0;
    for (int i = 0; i < factura.numProductos; i++) {
        printf("Nombre del producto: ");
        leerCadena(factura.productos[i].nombre, 50);

        do {
            printf("Cantidad: ");
            scanf("%d", &factura.productos[i].cantidad);
            if (factura.productos[i].cantidad < 0) {
                printf("La cantidad no puede ser negativa. Intente de nuevo.\n");
            }
        } while (factura.productos[i].cantidad < 0);

        do {
            printf("Precio: ");
            scanf("%f", &factura.productos[i].precio);
            if (factura.productos[i].precio < 0) {
                printf("El precio no puede ser negativo. Intente de nuevo.\n");
            }
        } while (factura.productos[i].precio < 0);

        factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
    }
    saveFactura(&factura);
}

void readFactura() {
    FILE *file = fopen("factura.dat", "rb");
    struct Factura factura;

    if (file == NULL) {
        printf("Error al abrir el archivo\n");
        return;
    }
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        printf("\n-----------------------------\n");
        printf("Cedula del Cliente: %d\n", factura.cedula);
        printf("Nombre del Cliente: %s\n", factura.nombre);
        printf("\nProductos:\n");
        printf("Nombre\tCantidad\tPrecio\tTotal\n");
        for (int i = 0; i < factura.numProductos; i++) {
            float totalProducto = factura.productos[i].cantidad * factura.productos[i].precio;
            printf("%s\t%d\t%.2f\t%.2f\n", factura.productos[i].nombre, factura.productos[i].cantidad, factura.productos[i].precio, totalProducto);
        }
        printf("\nTotal de la Factura: %.2f\n", factura.total);
        printf("-----------------------------\n\n");
    }
    fclose(file);
}


void editarFactura() {
    FILE *file = fopen("factura.dat", "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    struct Factura factura;
    int cedulaBuscar;
    int encontrado = 0;

    if (file == NULL || tempFile == NULL) {
        printf("Error al abrir los archivos\n");
        return;
    }

    do {
        printf("Ingrese la cedula de la factura que quiere editar: ");
        scanf("%d", &cedulaBuscar);
        if (cedulaBuscar < 0) {
            printf("La cedula no puede ser negativa. Intente de nuevo.\n");
        }
    } while (cedulaBuscar < 0);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedulaBuscar) {
            printf("Ingrese los nuevos datos:\n");
            printf("Nombre del cliente: ");
            leerCadena(factura.nombre, 20);

            do {
                printf("Numero de productos: ");
                scanf("%d", &factura.numProductos);
                if (factura.numProductos < 0) {
                    printf("El numero de productos no puede ser negativo. Intente de nuevo.\n");
                }
            } while (factura.numProductos < 0);

            factura.total = 0;
            for (int i = 0; i < factura.numProductos; i++) {
                printf("Nombre del producto: ");
                leerCadena(factura.productos[i].nombre, 50);

                do {
                    printf("Cantidad: ");
                    scanf("%d", &factura.productos[i].cantidad);
                    if (factura.productos[i].cantidad < 0) {
                        printf("La cantidad no puede ser negativa. Intente de nuevo.\n");
                    }
                } while (factura.productos[i].cantidad < 0);

                do {
                    printf("Precio: ");
                    scanf("%f", &factura.productos[i].precio);
                    if (factura.productos[i].precio < 0) {
                        printf("El precio no puede ser negativo. Intente de nuevo.\n");
                    }
                } while (factura.productos[i].precio < 0);

                factura.total += factura.productos[i].cantidad * factura.productos[i].precio;
            }
            fwrite(&factura, sizeof(struct Factura), 1, tempFile);
            encontrado = 1;
        } else {
            fwrite(&factura, sizeof(struct Factura), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("factura.dat");
        rename("temp.dat", "factura.dat");
        printf("La factura fue editada \n");
    } else {
        printf("La factura con cedula %d no fue encontrada.\n", cedulaBuscar);
        remove("temp.dat");
    }
}

void eliminarFactura() {
    FILE *file = fopen("factura.dat", "rb");
    FILE *tempFile = fopen("temp.dat", "wb");
    struct Factura factura;
    int cedulaEliminar;
    int encontrado = 0;

    if (file == NULL || tempFile == NULL) {
        printf("Error al abrir los archivos\n");
        return;
    }

    do {
        printf("Ingrese la cedula de la factura que quiere eliminar: ");
        scanf("%d", &cedulaEliminar);
        if (cedulaEliminar < 0) {
            printf("La cedula no puede ser negativa. Intente de nuevo.\n");
        }
    } while (cedulaEliminar < 0);

    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        if (factura.cedula == cedulaEliminar) {
            printf("La factura con cedula %d fue eliminada\n", cedulaEliminar);
            encontrado = 1;
        } else {
            fwrite(&factura, sizeof(struct Factura), 1, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (encontrado) {
        remove("factura.dat");
        rename("temp.dat", "factura.dat");
    } else {
        printf("Factura con cedula %d no encontrada.\n", cedulaEliminar);
        remove("temp.dat");
    }
}
