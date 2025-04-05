#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Para compilar en este módulo necesitamos conocer la definición de Order y la constante MAX_TOKENS.
// En un proyecto más modular se usaría un archivo de cabecera común, pero aquí lo incluimos manualmente.

#define MAX_TOKENS 20

typedef struct {
    int pizza_id;
    int order_id;
    char pizza_name_id[128];
    int quantity;
    char order_date[32];
    float unit_price;
    float total_price;
    char pizza_size[32];
    char pizza_category[64];
    char pizza_ingredients[256];
    char pizza_name[128];
} Order;

// Función que calcula la Pizza más Vendida (pms)
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

// Función que calcula la Pizza menos Vendida (pls)
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
