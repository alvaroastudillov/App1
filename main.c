#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LINE 1024
#define MAX_TOKENS 20

// Declaración de la estructura Order (debe coincidir con la de utils.c)
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

// Incluir la cabecera de utils
#include "utils.h"
// Incluir la cabecera de metrics (si tienes uno)
char *pms(int *size, Order *orders);
char *pls(int *size, Order *orders);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s archivo.csv\n", argv[0]);
        return 1;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }
    
    char line[MAX_LINE];
    if (fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacío o error al leer\n");
        fclose(fp);
        return 1;
    }
    
    int capacity = 100;
    int num_orders = 0;
    Order *orders = malloc(capacity * sizeof(Order));
    
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
    
    char *resultado_pms = pms(&num_orders, orders);
    char *resultado_pls = pls(&num_orders, orders);
    printf("Pizza más vendida: %s\n", resultado_pms);
    printf("Pizza menos vendida: %s\n", resultado_pls);
    free(resultado_pms);
    free(resultado_pls);
    
    free(orders);
    return 0;
}
