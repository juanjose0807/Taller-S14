
struct Producto {
    char nombre[50];
    int cantidad;
    float precio;
};

struct Factura {
    char nombre[20];
    int cedula;
    int numProductos;
    struct Producto productos[10];
    float total;
};

int menu();
void createFactura();
void readFactura();
void editarFactura();
void eliminarFactura();
void leerCadena(char *cadena, int num);
void saveFactura(struct Factura *factura);
