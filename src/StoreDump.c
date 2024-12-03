#include "StoreDump.h"

void storeDump(const char* pathToFile)
{
    int arrayCount;

    printf("Введите количество массивов структур: ");
    
    if (scanf("%d", &arrayCount) != 1 || arrayCount <= 0) 
    {
        printf("Некорректное количество массивов.\n");
        return;
    }

    FILE *file = fopen(pathToFile, "wb");
    
    if (!file) 
    {
        perror("Ошибка открытия файла для записи");
        return;
    }

    fwrite(&arrayCount, sizeof(int), 1, file); // Сохраняем количество массивов

    for (int i = 0; i < arrayCount; i++) 
    {
        int arraySize;

        printf("Введите количество структур в массиве %d: ", i + 1);
        
        if (scanf("%d", &arraySize) != 1 || arraySize <= 0) 
        {
            printf("Некорректное количество структур. Пропускаем массив.\n");
            continue;
        }

        fwrite(&arraySize, sizeof(int), 1, file); // Сохраняем размер массива

        StatData *array = malloc(arraySize * sizeof(StatData));
        
        if (!array) 
        {
            perror("Ошибка выделения памяти");
            fclose(file);
            return;
        }

        for (int j = 0; j < arraySize; j++) 
        {
            printf("Введите данные для структуры %d:\n", j + 1);
            printf("  ID: ");
            scanf("%ld", &array[j].id);
            printf("  Count: ");
            scanf("%d", &array[j].count);
            printf("  Cost: ");
            scanf("%f", &array[j].cost);
            
            printf("  Primary (0/1): ");
            unsigned int primary_tmp;
            scanf("%u", &primary_tmp);
            array[i].primary = primary_tmp;

            printf("  Mode (0-7): ");
            unsigned int mode_tmp;
            scanf("%u", &mode_tmp);
            array[i].mode = mode_tmp;

            printf("\n");
        }

        fwrite(array, sizeof(StatData), arraySize, file); // Сохраняем массив структур
        free(array);
    }

    fclose(file);
    printf("Данные успешно записаны в файл.\n");
}
