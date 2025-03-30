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

// pms: Pizza más vendida
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
    if(countSize > 0)
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

// dls: Fecha con menor ventas en términos de dinero
char *dls(int *size, Order *orders) {
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
    double min = (countSize > 0) ? sales[0].total : 0;
    char worst_date[32] = "";
    if(countSize > 0)
        strcpy(worst_date, sales[0].date);
    for (int j = 0; j < countSize; j++) {
        if (sales[j].total < min) {
            min = sales[j].total;
            strcpy(worst_date, sales[j].date);
        }
    }
    free(sales);
    char *result = malloc(256);
    snprintf(result, 256, "%s: %.2f", worst_date, min);
    return result;
}

// dmsp: Fecha con mayor ventas en términos de cantidad de pizzas
char *dmsp(int *size, Order *orders) {
    typedef struct {
        char date[32];
        int total;
    } DateQuantity;
    int capacity = 10, countSize = 0;
    DateQuantity *data = malloc(capacity * sizeof(DateQuantity));

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(data[j].date, orders[i].order_date) == 0) {
                data[j].total += orders[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (countSize >= capacity) {
                capacity *= 2;
                data = realloc(data, capacity * sizeof(DateQuantity));
            }
            strcpy(data[countSize].date, orders[i].order_date);
            data[countSize].total = orders[i].quantity;
            countSize++;
        }
    }
    int max = -1;
    char best_date[32] = "";
    int best_total = 0;
    for (int j = 0; j < countSize; j++) {
        if (data[j].total > max) {
            max = data[j].total;
            strcpy(best_date, data[j].date);
            best_total = data[j].total;
        }
    }
    free(data);
    char *result = malloc(256);
    snprintf(result, 256, "%s: %d", best_date, best_total);
    return result;
}

// dlsp: Fecha con menor ventas en términos de cantidad de pizzas
char *dlsp(int *size, Order *orders) {
    typedef struct {
        char date[32];
        int total;
    } DateQuantity;
    int capacity = 10, countSize = 0;
    DateQuantity *data = malloc(capacity * sizeof(DateQuantity));

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(data[j].date, orders[i].order_date) == 0) {
                data[j].total += orders[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (countSize >= capacity) {
                capacity *= 2;
                data = realloc(data, capacity * sizeof(DateQuantity));
            }
            strcpy(data[countSize].date, orders[i].order_date);
            data[countSize].total = orders[i].quantity;
            countSize++;
        }
    }
    int min = (countSize > 0) ? data[0].total : 0;
    char worst_date[32] = "";
    if(countSize > 0)
        strcpy(worst_date, data[0].date);
    for (int j = 0; j < countSize; j++) {
        if (data[j].total < min) {
            min = data[j].total;
            strcpy(worst_date, data[j].date);
        }
    }
    free(data);
    char *result = malloc(256);
    snprintf(result, 256, "%s: %d", worst_date, min);
    return result;
}

// apo: Promedio de pizzas por orden (contando órdenes únicas)
char *apo(int *size, Order *orders) {
    int totalPizzas = 0, distinctOrders = 0;
    int capacity = 10;
    int *order_ids = malloc(capacity * sizeof(int));
    for (int i = 0; i < *size; i++) {
        totalPizzas += orders[i].quantity;
        int found = 0;
        for (int j = 0; j < distinctOrders; j++) {
            if (order_ids[j] == orders[i].order_id) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (distinctOrders >= capacity) {
                capacity *= 2;
                order_ids = realloc(order_ids, capacity * sizeof(int));
            }
            order_ids[distinctOrders++] = orders[i].order_id;
        }
    }
    double avg = (distinctOrders > 0) ? (double)totalPizzas / distinctOrders : 0.0;
    free(order_ids);
    char *result = malloc(256);
    snprintf(result, 256, "%.2f", avg);
    return result;
}

// apd: Promedio de pizzas por día (agrupando por fecha)
char *apd(int *size, Order *orders) {
    int totalPizzas = 0, distinctDays = 0;
    int capacity = 10;
    char **days = malloc(capacity * sizeof(char *));
    for (int i = 0; i < *size; i++) {
        totalPizzas += orders[i].quantity;
        int found = 0;
        for (int j = 0; j < distinctDays; j++) {
            if (strcmp(days[j], orders[i].order_date) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (distinctDays >= capacity) {
                capacity *= 2;
                days = realloc(days, capacity * sizeof(char *));
            }
            days[distinctDays] = strdup(orders[i].order_date);
            distinctDays++;
        }
    }
    double avg = (distinctDays > 0) ? (double)totalPizzas / distinctDays : 0.0;
    for (int i = 0; i < distinctDays; i++)
        free(days[i]);
    free(days);
    char *result = malloc(256);
    snprintf(result, 256, "%.2f", avg);
    return result;
}

// ims: Ingrediente más vendido
char *ims(int *size, Order *orders) {
    typedef struct {
        char name[128];
        int count;
    } IngredientCount;
    int capacity = 10, countSize = 0;
    IngredientCount *ingredients = malloc(capacity * sizeof(IngredientCount));

    for (int i = 0; i < *size; i++) {
        char temp[256];
        strcpy(temp, orders[i].pizza_ingredients);
        char *token = strtok(temp, ",");
        while (token != NULL) {
            // Eliminar espacios iniciales
            while (isspace((unsigned char)*token)) token++;
            char ing[128];
            strcpy(ing, token);
            int found = 0;
            for (int j = 0; j < countSize; j++) {
                if (strcmp(ingredients[j].name, ing) == 0) {
                    ingredients[j].count += orders[i].quantity;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                if (countSize >= capacity) {
                    capacity *= 2;
                    ingredients = realloc(ingredients, capacity * sizeof(IngredientCount));
                }
                strcpy(ingredients[countSize].name, ing);
                ingredients[countSize].count = orders[i].quantity;
                countSize++;
            }
            token = strtok(NULL, ",");
        }
    }
    int max = -1;
    char best_ing[128] = "";
    for (int j = 0; j < countSize; j++) {
        if (ingredients[j].count > max) {
            max = ingredients[j].count;
            strcpy(best_ing, ingredients[j].name);
        }
    }
    free(ingredients);
    char *result = malloc(256);
    snprintf(result, 256, "%s", best_ing);
    return result;
}

/* ------------------- Arreglo de punteros a funciones para las métricas ------------------- */
typedef char* (*MetricFunc)(int *, Order *);

typedef struct {
    char *code;
    char *description;
    MetricFunc func;
} MetricEntry;

// Actualizamos el arreglo de métricas para incluir la función ims
MetricEntry metrics[] = {
    {"pms", "Pizza mas vendida", pms},
    {"pls", "Pizza menos vendida", pls},
    {"dms", "Fecha con mas ventas en terminos de dinero", dms},
    {"dls", "Fecha con menos ventas en terminos de dinero", dls},
    {"dmsp", "Fecha con mas ventas en terminos de cantidad de pizzas", dmsp},
    {"dlsp", "Fecha con menos ventas en terminos de cantidad de pizzas", dlsp},
    {"apo", "Promedio de pizzas por orden", apo},
    {"apd", "Promedio de pizzas por dia", apd},
    {"ims", "Ingrediente mas vendido", ims}
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
        // Asignar campos según el formato:
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
