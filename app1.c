#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

// Definición de la estructura para cada venta (Order)
typedef struct {
    int order_id;
    char order_date[32];
    int quantity;
    double total_price;
    char pizza_category[64];
    char pizza_ingredients[256];
    char pizza_name[128];
} Order;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s archivo.csv\n", argv[0]);
        return 1;
    }

    // Abrir el archivo CSV
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    // Leer e ignorar la cabecera
    char line[MAX_LINE];
    if (fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacío o error al leer\n");
        fclose(fp);
        return 1;
    }

    printf("Contenido del archivo CSV:\n");
    // Lectura del archivo y despliegue línea a línea
    while (fgets(line, MAX_LINE, fp)) {
        printf("%s", line);
    }

    fclose(fp);
    return 0;
}
