#include "SortDump.h"

int compareByCost(const void *a, const void *b)
{
    const StatData *statA = (const StatData *)a;
    const StatData *statB = (const StatData *)b;
    return (statA->cost > statB->cost) - (statA->cost < statB->cost);
}

void sortDump(const char* pathToFile) 
{
    // Чтение и запись в одном режиме
    FILE *file = fopen(pathToFile, "rb+"); 
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

    // Выделяем память для массивов
    StatData **arrays = malloc(arrayCount * sizeof(StatData*));
    int *arraySizes = malloc(arrayCount * sizeof(int));
    if (!arrays || !arraySizes) 
    {
        perror("Ошибка выделения памяти");
        fclose(file);
        free(arrays);
        free(arraySizes);
        return;
    }

    // Чтение всех массивов из файла
    for (int i = 0; i < arrayCount; i++) 
    {
        if (fread(&arraySizes[i], sizeof(int), 1, file) != 1) 
        {
            perror("Ошибка чтения размера массива");
            fclose(file);
            free(arrays);
            free(arraySizes);
            return;
        }

        arrays[i] = malloc(arraySizes[i] * sizeof(StatData));
        
        if (!arrays[i]) 
        {
            perror("Ошибка выделения памяти для массива структур");
            fclose(file);
            for (int j = 0; j < i; j++) 
            {
                free(arrays[j]);
            }
            free(arrays);
            free(arraySizes);
            return;
        }

        if (fread(arrays[i], sizeof(StatData), arraySizes[i], file) != (size_t)arraySizes[i]) 
        {
            perror("Ошибка чтения массива структур");
            fclose(file);
            for (int j = 0; j <= i; j++) 
            {
                free(arrays[j]);
            }
            free(arrays);
            free(arraySizes);
            return;
        }
    }

    // Выбор двух массивов для сортировки
    int firstArray, secondArray;
    
    printf("Введите индекс первого массива для сортировки (1-%d): ", arrayCount);
    scanf("%d", &firstArray);

    printf("Введите индекс второго массива для сортировки (1-%d): ", arrayCount);
    scanf("%d", &secondArray);

    if (firstArray < 1 || firstArray > arrayCount || secondArray < 1 || secondArray > arrayCount) 
    {
        printf("Некорректный индекс массива.\n");
        fclose(file);
        for (int i = 0; i < arrayCount; i++) 
        {
            free(arrays[i]);
        }
        free(arrays);
        free(arraySizes);
        return;
    }

    // Сортировка выбранных массивов
    qsort(arrays[firstArray - 1], arraySizes[firstArray - 1], sizeof(StatData), compareByCost);
    qsort(arrays[secondArray - 1], arraySizes[secondArray - 1], sizeof(StatData), compareByCost);

    // Перезапись отсортированных данных в файл
    rewind(file);
    
    // Перезапись количества массивов
    fwrite(&arrayCount, sizeof(int), 1, file); 
    
    for (int i = 0; i < arrayCount; i++) 
    {
        fwrite(&arraySizes[i], sizeof(int), 1, file);
        fwrite(arrays[i], sizeof(StatData), arraySizes[i], file);
        free(arrays[i]);
    }

    printf("\nСортировка завершена. Данные обновлены в файле.\n\n");

    // Очистка памяти и закрытие файла
    free(arrays);
    free(arraySizes);
    fclose(file);
}
