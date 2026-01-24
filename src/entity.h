#ifndef ENTITY_H
#define ENTITY_H

#include "libs/eadk.h"

#define MAX_ENTITY 64

typedef struct {
    int entity_id;
    int x, y;
    int w, h;
    bool is_active;
    bool is_visible;
    const eadk_color_t *sprite;
} entity_t;

extern entity_t entity_list[MAX_ENTITY];

void init_entities();
int create_entity(bool is_visible, int x, int y, int w, int h, const eadk_color_t *sprite);
void remove_entity(int entity_id);
void update_entity(int entity_id, const bool *is_visible, const int *x, const int *y, const int *w, const int *h, const eadk_color_t *sprite);

#endif