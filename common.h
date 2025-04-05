#ifndef COMMON_H
#define COMMON_H

#define MAX_LINE 1024
#define MAX_TOKENS 20

typedef struct {
    int pizza_id;
    int order_id;
    char pizza_name_id[128];
    int quantity;
    char order_date[32];    // formato DD/MM/YYYY
    float unit_price;
    float total_price;
    char pizza_size[32];    // Ej: L, M, S
    char pizza_category[64];
    char pizza_ingredients[256];
    char pizza_name[128];
} Order;

#endif // COMMON_H
