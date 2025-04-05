// Habilitamos funciones POSIX (incluida strdup) ya que strdup no forma parte de C99.
#define _POSIX_C_SOURCE 200809L

#include "metrics.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    if (countSize > 0)
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
    if (countSize > 0)
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

// hp: Cantidad de pizzas por categoría vendidas.
char *hp(int *size, Order *orders) {
    typedef struct {
        char category[64];
        int total;
    } CatCount;
    int capacity = 10, countSize = 0;
    CatCount *cats = malloc(capacity * sizeof(CatCount));
    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < countSize; j++) {
            if (strcmp(cats[j].category, orders[i].pizza_category) == 0) {
                cats[j].total += orders[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            if (countSize >= capacity) {
                capacity *= 2;
                cats = realloc(cats, capacity * sizeof(CatCount));
            }
            strcpy(cats[countSize].category, orders[i].pizza_category);
            cats[countSize].total = orders[i].quantity;
            countSize++;
        }
    }
    char *result = malloc(512);
    result[0] = '\0';
    for (int j = 0; j < countSize; j++) {
        char buffer[128];
        snprintf(buffer, 128, "%s: %d", cats[j].category, cats[j].total);
        strcat(result, buffer);
        if (j < countSize - 1)
            strcat(result, ", ");
    }
    free(cats);
    return result;
}
