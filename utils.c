#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_TOKENS 20  // Permitimos más tokens para manejar campos con comas

// Definición de la estructura para cada venta (Order)
typedef struct {
    int pizza_id;           // campo agregado
    int order_id;
    char pizza_name_id[128]; // campo agregado
    int quantity;
    char order_date[32];    // formato DD/MM/YYYY
    float unit_price;       // campo agregado
    float total_price;
    char pizza_size[32];    // campo agregado (L, M, S)
    char pizza_category[64];
    char pizza_ingredients[256];
    char pizza_name[128];
} Order;

// Función para eliminar espacios al inicio y final de una cadena
void trim(char *str) {
    while (isspace((unsigned char)*str)) str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end+1) = '\0';
}

// Función para parsear una línea CSV, separándola en tokens respetando comillas
int parse_csv_line(char *line, char tokens[][256]) {
    int token_index = 0;
    int i = 0, len = strlen(line);
    while (i < len && token_index < MAX_TOKENS) {
        char field[256] = {0};
        int field_index = 0;
        if (line[i] == '"') {
            i++; // saltar comilla inicial
            while (i < len && line[i] != '"') {
                field[field_index++] = line[i++];
            }
            field[field_index] = '\0';
            i++; // saltar comilla de cierre
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
