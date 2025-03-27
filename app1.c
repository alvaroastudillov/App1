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

/* ------------------- Función de Métrica: pms ------------------- */
// Calcula la Pizza Más Vendida (pms) agrupando por pizza_name y sumando la cantidad
// Firma: char *pms(int *size, Order *orders);
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

/* ------------------- Arreglo de punteros a funciones para las métricas ------------------- */
typedef char* (*MetricFunc)(int *, Order *);

typedef struct {
    char *code;
    char *description;
    MetricFunc func;
} MetricEntry;

// Por ahora se implementa solo la métrica pms.
MetricEntry metrics[] = {
    {"pms", "Pizza mas vendida", pms}
};

int num_metrics = sizeof(metrics) / sizeof(metrics[0]);

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
    
    // Leer e ignorar la cabecera
    if (fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacío o error al leer\n");
        fclose(fp);
        return 1;
    }

    // Arreglo de órdenes (usamos un arreglo dinámico para permitir crecer)
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
            continue; // Si la línea no tiene los 12 campos, se ignora

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

    // Si se especifican métricas como argumentos, procesarlas.
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
        printf("No se especificaron métricas. Se cargaron %d órdenes.\n", num_orders);
    }

    free(orders);
    return 0;
}
