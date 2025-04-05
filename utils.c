#include "utils.h"
#include <ctype.h>
#include <string.h>

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
            i++; // Saltar comilla inicial
            while (i < len && line[i] != '"') {
                field[field_index++] = line[i++];
            }
            field[field_index] = '\0';
            i++; // Saltar comilla de cierre
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
