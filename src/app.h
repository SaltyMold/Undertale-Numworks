#ifndef CORE_H
#define CORE_H

#define SAVE_FILE "undertale.dat"

typedef struct {
    int var1;
    int var2;
    int var3;
    int var4;
    int var5;
} SaveData;

extern SaveData game_save;

void first_menu();
void load_data();
void save_data();

#endif