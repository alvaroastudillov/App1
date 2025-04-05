#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_TOKENS 20

// Definición de la estructura para cada venta (Order)
// (Esta definición debe coincidir con la de utils.c y metrics.c)
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

// Declaración de las funciones de utils (parse_csv_line y trim)
void trim(char *str);
int parse_csv_line(char *line, char tokens[][256]);

// Declaración de las funciones de métricas
char *pms(int *size, Order *orders);
char *pls(int *size, Order *orders);

void trim(char *str) {
    while (isspace((unsigned char)*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0';
}

int parse_csv_line(char *line, char tokens[][256]) {
    int token_index = 0;
    int i = 0, len = strlen(line);
    while (i < len && token_index < MAX_TOKENS) {
        char field[256] = {0};
        int field_index = 0;
        if (line[i] == '"') {
            i++;
            while (i < len && line[i] != '"') {
                field[field_index++] = line[i++];
            }
            field[field_index] = '\0';
            i++;
            if (line[i] == ',') i++;
        } else {
            while (i < len && line[i] != ',') {
                field[field_index++] = line[i++];
            }
            field[field_index] = '\0';
            if (line[i] == ',') i++;
        }
        strcpy(tokens[token_index], field);
        token_index++;
    }
    return token_index;
}

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
    // Leer y descartar la cabecera
    if (fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacío o error al leer\n");
        fclose(fp);
        return 1;
    }
    
    int capacity = 100;
    int num_orders = 0;
    Order *orders = malloc(capacity * sizeof(Order));
    
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
        }
        orders[num_orders].order_id = atoi(tokens[1]);
        strcpy(orders[num_orders].order_date, tokens[4]);
        orders[num_orders].quantity = atoi(tokens[3]);
        orders[num_orders].total_price = atof(tokens[7]);
        strcpy(orders[num_orders].pizza_category, tokens[9]);
        
        // Reconstruir los campos pizza_ingredients y pizza_name según el número de tokens
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
    
    // Ejemplo: calcular y mostrar las métricas pms y pls
    char *resultado_pms = pms(&num_orders, orders);
    char *resultado_pls = pls(&num_orders, orders);
    printf("Pizza más vendida: %s\n", resultado_pms);
    printf("Pizza menos vendida: %s\n", resultado_pls);
    free(resultado_pms);
    free(resultado_pls);
    
    free(orders);
    return 0;
}

