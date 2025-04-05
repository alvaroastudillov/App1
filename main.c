#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "metrics.h"

// Tipo de puntero a función para las métricas
typedef char* (*MetricFunc)(int *, Order *);

// Estructura que asocia el código de la métrica, su descripción y la función
typedef struct {
    char *code;
    char *description;
    MetricFunc func;
} MetricEntry;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s archivo.csv [metrica0 metrica1 ...]\n", argv[0]);
        return 1;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return 1; 
    }
    
    char line[MAX_LINE];
    // Leer y descartar la cabecera
    if (fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacío o error al leer\n");
        fclose(fp);
        return 1;
    }
    
    int capacity = 100;
    int num_orders = 0;
    Order *orders = malloc(capacity * sizeof(Order));
    if (!orders) {
        perror("Error al asignar memoria");
        fclose(fp);
        return 1;
    }
    
    // Procesar cada línea del CSV
    while (fgets(line, MAX_LINE, fp)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0)
            continue;
        char tokens[MAX_TOKENS][256] = {{0}};
        int nTokens = parse_csv_line(line, tokens);
        if (nTokens < 11)
            continue;
        if (num_orders >= capacity) {
            capacity *= 2;
            orders = realloc(orders, capacity * sizeof(Order));
            if (!orders) {
                perror("Error al realocar memoria");
                fclose(fp); //se preocupa de cerrar el archivo
                return 1;
            }
        }
        orders[num_orders].order_id = atoi(tokens[1]);
        strcpy(orders[num_orders].order_date, tokens[4]);
        orders[num_orders].quantity = atoi(tokens[3]);
        orders[num_orders].total_price = atof(tokens[7]);
        strcpy(orders[num_orders].pizza_category, tokens[9]);
        if (nTokens == 12) {
            strcpy(orders[num_orders].pizza_ingredients, tokens[10]);
            strcpy(orders[num_orders].pizza_name, tokens[11]);
        } else if (nTokens > 12) {
            char ingredients[256] = "";
            for (int k = 10; k < nTokens - 1; k++) {
                strcat(ingredients, tokens[k]);
                if (k < nTokens - 2)
                    strcat(ingredients, ", ");
            }
            strcpy(orders[num_orders].pizza_ingredients, ingredients);
            strcpy(orders[num_orders].pizza_name, tokens[nTokens - 1]);
        } else if (nTokens == 11) {
            orders[num_orders].pizza_ingredients[0] = '\0';
            strcpy(orders[num_orders].pizza_name, tokens[10]);
        }
        num_orders++;
    }
    fclose(fp);
    
    // Definir el arreglo de métricas
    MetricEntry metrics[] = {
        {"pms", "Pizza más vendida", pms},
        {"pls", "Pizza menos vendida", pls},
        {"dms", "Fecha con mayor ventas en dinero", dms},
        {"dls", "Fecha con menor ventas en dinero", dls},
        {"dmsp", "Fecha con mayor ventas en cantidad de pizzas", dmsp},
        {"dlsp", "Fecha con menor ventas en cantidad de pizzas", dlsp},
        {"apo", "Promedio de pizzas por orden", apo},
        {"apd", "Promedio de pizzas por día", apd},
        {"ims", "Ingrediente más vendido", ims},
        {"hp", "Cantidad de pizzas por categoría", hp}
    };
    int num_metrics = sizeof(metrics) / sizeof(metrics[0]);
    
    // Procesar los argumentos para ejecutar las métricas
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            int found = 0;
            for (int j = 0; j < num_metrics; j++) {
                if (strcmp(argv[i], metrics[j].code) == 0) {
                    char *res = metrics[j].func(&num_orders, orders);
                    printf("%s: %s\n", metrics[j].description, res);
                    free(res);
                    found = 1;
                    break;
                }
            }
            if (!found)
                printf("Métrica desconocida: %s\n", argv[i]);
        }
    } else {
        printf("No se especificaron métricas. Se cargaron %d órdenes.\n", num_orders);
    }
    
    free(orders);
    return 0;
}
