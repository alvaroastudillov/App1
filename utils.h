#ifndef UTILS_H
#define UTILS_H

#define MAX_LINE 1024
#define MAX_TOKENS 20  // Permitimos más tokens para manejar campos con comas

// Definición de la estructura para cada venta (Order)
// Esta definición se usará en todos los módulos (main, utils y metrics)
typedef struct {
    int pizza_id;           // Campo agregado
    int order_id;
    char pizza_name_id[128]; // Campo agregado
    int quantity;
    char order_date[32];    // Formato DD/MM/YYYY
    float unit_price;       // Campo agregado
    float total_price;
    char pizza_size[32];    // Campo agregado (por ejemplo, L, M, S)
    char pizza_category[64];
    char pizza_ingredients[256];
    char pizza_name[128];
} Order;

// Declaración de las funciones de utilidad
void trim(char *str);
int parse_csv_line(char *line, char tokens[][256]);

#endif // UTILS_H
