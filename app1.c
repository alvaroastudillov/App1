#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_TOKENS 12  // Se asume que cada línea tiene 12 campos

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

// Función simple para parsear una línea CSV separándola en tokens por coma.
int parse_csv_line(char *line, char tokens[][256]) {
    int token_index = 0;
    char *token = strtok(line, ",");
    while (token != NULL && token_index < MAX_TOKENS) {
        strcpy(tokens[token_index++], token);
        token = strtok(NULL, ",");
    }
    return token_index;
}

/* ------------------- Funciones de Métricas ------------------- */

// pms: Pizza más vendida (ya implementada previamente)
char *pms(int *size, Order *orders) {
    typedef struct {
        char name[128];
        int total;
    } PizzaCount;
    int capacity = 10, countSize = 0;
    PizzaCount *counts = malloc(capacity * sizeof(PizzaCount));

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(counts[j].name, orders[i].pizza_name) == 0) {
                counts[j].total += orders[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (countSize >= capacity) {
                capacity *= 2;
                counts = realloc(counts, capacity * sizeof(PizzaCount));
            }
            strcpy(counts[countSize].name, orders[i].pizza_name);
            counts[countSize].total = orders[i].quantity;
            countSize++;
        }
    }
    int max = -1;
    char best[128] = "";
    for (int j = 0; j < countSize; j++) {
        if (counts[j].total > max) {
            max = counts[j].total;
            strcpy(best, counts[j].name);
        }
    }
    free(counts);
    char *result = malloc(256);
    snprintf(result, 256, "%s", best);
    return result;
}

// pls: Pizza menos vendida
char *pls(int *size, Order *orders) {
    typedef struct {
        char name[128];
        int total;
    } PizzaCount;
    int capacity = 10, countSize = 0;
    PizzaCount *counts = malloc(capacity * sizeof(PizzaCount));

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(counts[j].name, orders[i].pizza_name) == 0) {
                counts[j].total += orders[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (countSize >= capacity) {
                capacity *= 2;
                counts = realloc(counts, capacity * sizeof(PizzaCount));
            }
            strcpy(counts[countSize].name, orders[i].pizza_name);
            counts[countSize].total = orders[i].quantity;
            countSize++;
        }
    }
    int min = (countSize > 0) ? counts[0].total : 0;
    char worst[128] = "";
    if (countSize > 0)
        strcpy(worst, counts[0].name);
    for (int j = 0; j < countSize; j++) {
        if (counts[j].total < min) {
            min = counts[j].total;
            strcpy(worst, counts[j].name);
        }
    }
    free(counts);
    char *result = malloc(256);
    snprintf(result, 256, "%s", worst);
    return result;
}

// dms: Fecha con mayor ventas en términos de dinero
char *dms(int *size, Order *orders) {
    typedef struct {
        char date[32];
        double total;
    } DateSales;
    int capacity = 10, countSize = 0;
    DateSales *sales = malloc(capacity * sizeof(DateSales));

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(sales[j].date, orders[i].order_date) == 0) {
                sales[j].total += orders[i].total_price;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (countSize >= capacity) {
                capacity *= 2;
                sales = realloc(sales, capacity * sizeof(DateSales));
            }
            strcpy(sales[countSize].date, orders[i].order_date);
            sales[countSize].total = orders[i].total_price;
            countSize++;
        }
    }
    double max = -1.0;
    char best_date[32] = "";
    double best_total = 0;
    for (int j = 0; j < countSize; j++) {
        if (sales[j].total > max) {
            max = sales[j].total;
            strcpy(best_date, sales[j].date);
            best_total = sales[j].total;
        }
    }
    free(sales);
    char *result = malloc(256);
    snprintf(result, 256, "%s: %.2f", best_date, best_total);
    return result;
}

//dls: Fecha con menos ventas en términos de dinero (junto a la cantidad de dinero recaudado)
char *dls(int *size, Order *orders){
    typedef struct {
        char date[32];
        double total;
    } DateSales;
    
    int capacity = 10, countSize = 0;
    DateSales *sales = malloc(capacity * sizeof(DateSales));

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(sales[j].date, orders[i].order_date) == 0) {
                sales[j].total += orders[i].total_price;
                found = 1;
                break;
            }
        }
        if (!found) { //La fecha no se encontro en el arreglo sales[]
            if (countSize >= capacity) { //si el arreglo esta lleno, lo expandimos
                capacity *= 2; //duplicamos el tamaño de la capacidad
                sales = realloc(sales, capacity * sizeof(DateSales)); //para reservar mas memoria
            }
            strcpy(sales[countSize].date, orders[i].order_date); //
            sales[countSize].total = orders[i].total_price;
            countSize++;
        }
    }

    double min = (countSize > 0) ? sales[0].total : 0;
    char worst_date[32] = "";
    double worst_total = 0; //cantidad de ventas en la fecha de menor ventas
    if (countSize > 0) {
        strcpy(worst_date, sales[0].date);
        worst_total = sales[0].total;
    }
    for (int j = 0; j < countSize; j++) { //guarda la fecha con menos ventas
        if (sales[j].total < min) {
            min = sales[j].total;
            strcpy(worst_date, sales[j].date);
            worst_total = sales[j].total;
        }
    }

    free(sales); //liberamos la memoria, para no tener perdidas de memoria
    char *result = malloc(256); //reservamos memoria para el resultado
    snprintf(result, 256, "%s: %.2f", worst_date, worst_total); //guardamos el resultado en el formato que describamos
    return result;
}


/* ------------------- Arreglo de punteros a funciones para las métricas ------------------- */
typedef char* (*MetricFunc)(int *, Order *);

typedef struct {
    char *code;
    char *description;
    MetricFunc func;
} MetricEntry;

// Actualizamos el arreglo de métricas con las nuevas funciones
MetricEntry metrics[] = {
    {"pms", "Pizza mas vendida", pms},
    {"pls", "Pizza menos vendida", pls},
    {"dms", "Fecha con mas ventas en terminos de dinero", dms},
    {"dls", "Fecha con menos ventas en terminos de dinero", dls}
};

int num_metrics = sizeof(metrics) / sizeof(metrics[0]);


/* Metricas que faltan: 
    dmsp: Fecha con más ventas en términos de cantidad de pizzas (junto a la cantidad de pizzas)
    dlsp: Fecha con menos ventas en términos de cantidad de pizzas (junto a la cantidad de pizzas)
    apo: Promedio de pizzas por orden
    apd: Promedio de pizzas por día
    ims: Ingrediente más vendido
    hp: Cantidad de pizzas por categoría vendidas
    */


/* ------------------- Función main ------------------- */
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s archivo.csv [metrica0 metrica1 ...]\n", argv[0]);
        return 1;
    }

    // Abrir el archivo CSV
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    char line[MAX_LINE];
    if (fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacio o error al leer\n");
        fclose(fp);
        return 1;
    }

    // Arreglo dinámico de órdenes
    int capacity = 100;
    int num_orders = 0;
    Order *orders = malloc(capacity * sizeof(Order));

    // Procesar cada línea del CSV
    while (fgets(line, MAX_LINE, fp)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0)
            continue;
        char tokens[MAX_TOKENS][256] = {{0}};
        int token_count = parse_csv_line(line, tokens);
        if (token_count < MAX_TOKENS)
            continue;
        if (num_orders >= capacity) {
            capacity *= 2;
            orders = realloc(orders, capacity * sizeof(Order));
        }
        // Asignación de campos según el formato:
        // 0: pizza_id, 1: order_id, 2: pizza_name_id, 3: quantity, 4: order_date,
        // 5: order_time, 6: unit_price, 7: total_price, 8: pizza_size,
        // 9: pizza_category, 10: pizza_ingredients, 11: pizza_name
        orders[num_orders].order_id = atoi(tokens[1]);
        strcpy(orders[num_orders].order_date, tokens[4]);
        orders[num_orders].quantity = atoi(tokens[3]);
        orders[num_orders].total_price = atof(tokens[7]);
        strcpy(orders[num_orders].pizza_category, tokens[9]);
        strcpy(orders[num_orders].pizza_ingredients, tokens[10]);
        strcpy(orders[num_orders].pizza_name, tokens[11]);
        num_orders++;
    }
    fclose(fp);

    // Procesar las métricas especificadas como argumentos
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
            if (!found) {
                printf("Metrica desconocida: %s\n", argv[i]);
            }
        }
    } else {
        printf("No se especificaron metricas. Se cargaron %d ordenes.\n", num_orders);
    }

    free(orders);
    return 0;
}
