#include "entity.h"

entity_t entity_list[MAX_ENTITY];

void init_entities() {
    for (int idx = 0; idx < MAX_ENTITY; ++idx) {
        entity_list[idx].entity_id = idx;
        entity_list[idx].is_active = false;
        entity_list[idx].is_visible = false;
        entity_list[idx].damage = 0;
        entity_list[idx].pos.x = 0;
        entity_list[idx].pos.y = 0;
        entity_list[idx].size.w = 0;
        entity_list[idx].size.h = 0;
        entity_list[idx].sprite = NULL;
    }
}

int find_free_entity() {
    for (int idx = 0; idx < MAX_ENTITY; ++idx) {
        if (!entity_list[idx].is_active) return idx;
    }
    return -1; // -1 if error or no free entity
}

// returns entity id
int create_entity(bool is_visible, int x, int y, int w, int h, int damage, const eadk_color_t *sprite) {
    int idx = find_free_entity();
    if (idx < 0) return -1; // -1 if error or no free entity
    entity_list[idx].entity_id = idx;
    entity_list[idx].is_active = true;
    entity_list[idx].damage = damage;
    entity_list[idx].is_visible = is_visible;
    entity_list[idx].pos.x = x;
    entity_list[idx].pos.y = y;
    entity_list[idx].size.w = w;
    entity_list[idx].size.h = h;
    entity_list[idx].sprite = sprite;
    return idx; 
}

// sets is_active to false
void remove_entity(int entity_id) {
    if (entity_id < 0 || entity_id >= MAX_ENTITY) return; // bounds check

    entity_list[entity_id].is_active = false;
}

void update_entity(int entity_id, const bool *is_visible, const int *x, const int *y, const int *w, const int *h, const int *damage, const eadk_color_t *sprite) { 
    if (entity_id < 0 || entity_id >= MAX_ENTITY) return; // bounds check

    entity_list[entity_id].is_visible = *is_visible;
    entity_list[entity_id].pos.x = *x;
    entity_list[entity_id].pos.y = *y;
    entity_list[entity_id].size.w = *w;
    entity_list[entity_id].size.h = *h;
    entity_list[entity_id].damage = *damage;
    entity_list[entity_id].sprite = sprite;
}