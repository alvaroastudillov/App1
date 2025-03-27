#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    while(token != NULL && token_index < MAX_TOKENS) {
        // Se copia el token al arreglo (se podría agregar un trim si fuera necesario)
        strcpy(tokens[token_index++], token);
        token = strtok(NULL, ",");
    }
    return token_index;
}

int main(int argc, char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Uso: %s archivo.csv\n", argv[0]);
        return 1;
    }

    // Abrir el archivo CSV
    FILE *fp = fopen(argv[1], "r");
    if(fp == NULL) {
        perror("Error al abrir el archivo");
        return 1;
    }

    char line[MAX_LINE];
    
    // Leer e imprimir la cabecera (se ignora para procesamiento)
    if(fgets(line, MAX_LINE, fp) == NULL) {
        fprintf(stderr, "Archivo vacío o error al leer\n");
        fclose(fp);
        return 1;
    }
    printf("Cabecera: %s", line);

    // Arreglo de órdenes (de tamaño fijo para este ejemplo)
    Order orders[100];
    int num_orders = 0;

    // Procesar cada línea del CSV
    while(fgets(line, MAX_LINE, fp)) {
        // Eliminar salto de línea
        line[strcspn(line, "\r\n")] = 0;
        if(strlen(line) == 0)
            continue;
        
        char tokens[MAX_TOKENS][256] = {{0}};
        int token_count = parse_csv_line(line, tokens);
        // Verificamos que se hayan leído los 12 campos
        if(token_count < MAX_TOKENS)
            continue;

        // Asignar los campos a la estructura Order
        // Orden de campos según el formato:
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

    // Imprimir las órdenes leídas para verificar el parsing
    for (int i = 0; i < num_orders; i++){
        printf("Order %d: %s, %d, %.2f, %s, %s, %s\n", i+1,
            orders[i].order_date, orders[i].quantity, orders[i].total_price,
            orders[i].pizza_category, orders[i].pizza_ingredients, orders[i].pizza_name);
    }

    return 0;
}
