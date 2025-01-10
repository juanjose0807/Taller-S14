#include <stdio.h>
#include <string.h>
#include "funciones.h"

int menu() {
    int opcion;
    printf("1. Crear factura\n");
    printf("2. Leer facturas\n");
    printf("3. Editar factura\n");
    printf("4. Eliminar factura\n");
    printf("5. Salir\n");
    printf(">> ");
    scanf("%d", &opcion);
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
    printf("Cedula del cliente: ");
    scanf("%d", &factura.cedula);
    printf("Numero de productos: ");
    scanf("%d", &factura.numProductos);
    factura.total = 0;
    for (int i = 0; i < factura.numProductos; i++) {
        printf("Nombre del producto: ");
        leerCadena(factura.productos[i].nombre, 50);
        printf("Cantidad: ");
        scanf("%d", &factura.productos[i].cantidad);
        printf("Precio: ");
        scanf("%f", &factura.productos[i].precio);
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
    printf("Cedula\t\tNombre\t\tTotal\n");
    while (fread(&factura, sizeof(struct Factura), 1, file)) {
        printf("%d\t\t%s\t\t%.2f\n", factura.cedula, factura.nombre, factura.total);
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

        printf("Ingrese la cedula de la factura que quiere editar: ");
        scanf("%d", &cedulaBuscar);

        while (fread(&factura, sizeof(struct Factura), 1, file)) {
            if (factura.cedula == cedulaBuscar) {
                printf("Ingrese los nuevos datos:\n");
                printf("Nombre del cliente: ");
                leerCadena(factura.nombre, 20);
                printf("Numero de productos: ");
                scanf("%d", &factura.numProductos);
                factura.total = 0;
                for (int i = 0; i < factura.numProductos; i++) {
                    printf("Nombre del producto: ");
                    leerCadena(factura.productos[i].nombre, 50);
                    printf("Cantidad: ");
                    scanf("%d", &factura.productos[i].cantidad);
                    printf("Precio: ");
                    scanf("%f", &factura.productos[i].precio);
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
            printf("La factura con cedula %d no fueencontrada.\n", cedulaBuscar);
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

    printf("Ingrese la cedula de la factura que quiere eliminar: ");
    scanf("%d", &cedulaEliminar);

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
