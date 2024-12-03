#include "LoadDump.h"

void loadDump(const char* pathToFile)
{
    FILE *file = fopen(pathToFile, "rb");
    
    if (!file) 
    {
        perror("Ошибка открытия файла");
        return;
    }

    int arrayCount;
    
    if (fread(&arrayCount, sizeof(int), 1, file) != 1) 
    {
        perror("Ошибка чтения количества массивов");
        fclose(file);
        return;
    }

    for (int i = 0; i < arrayCount; i++) 
    {
        int arraySize;

        if (fread(&arraySize, sizeof(int), 1, file) != 1) 
        {
            perror("Ошибка чтения размера массива");
            break;
        }

        StatData *array = malloc(arraySize * sizeof(StatData));
        if (!array) 
        {
            perror("Ошибка выделения памяти");
            break;
        }

        if (fread(array, sizeof(StatData), arraySize, file) != (size_t)arraySize) 
        {
            perror("Ошибка чтения массива структур");
            free(array);
            break;
        }

        printf("Массив %d (размер: %d):\n", i + 1, arraySize);
        
        for (int j = 0; j < arraySize; j++) 
        {
            printf("  ID: %ld, \tCount: %d, \tCost: %.2f, \tPrimary: %u, \tMode: %u\n",
                   array[j].id, array[j].count, array[j].cost,
                   array[j].primary, array[j].mode);
        }

        printf("\n");

        free(array);
    }

    fclose(file);
}
