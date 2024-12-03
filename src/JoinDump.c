#include "JoinDump.h"

// Дополнительная функция для обновления полей записи с одинаковым id
void mergeStatData(StatData* target, const StatData* source) 
{
    target->count += source->count;
    target->cost += source->cost;
    target->primary = target->primary && source->primary;
    
    if (source->mode > target->mode) 
    {
        target->mode = source->mode;
    }
}

// Дополнительная функция для поиска записи с заданным id в массиве
int findById(long id, StatData* array, int size) 
{
    for (int i = 0; i < size; i++) 
    {
        if (array[i].id == id) 
        {
            // Возвращаем индекс найденной записи 
            return i; 
        }
    }
    return -1; // Если запись не найдена
}

void joinDump(const char* pathToFile) 
{
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

    // Выбор двух массивов для объединения
    int firstArray, secondArray;
    printf("\nВведите индекс первого массива для объединения (1-%d): ", arrayCount);
    scanf("%d", &firstArray);
    
    printf("Введите индекс второго массива для объединения (1-%d): ", arrayCount);
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

    // Объединение массивов с учётом логики
    StatData* resultArray = malloc((arraySizes[firstArray - 1] + arraySizes[secondArray - 1]) * sizeof(StatData));
    int resultSize = 0;

    // Добавляем элементы из первого массива
    for (int i = 0; i < arraySizes[firstArray - 1]; i++) 
    {
        resultArray[resultSize++] = arrays[firstArray - 1][i];
    }

    // Добавляем элементы из второго массива с обработкой повторяющихся id
    for (int i = 0; i < arraySizes[secondArray - 1]; i++) 
    {
        int index = findById(arrays[secondArray - 1][i].id, resultArray, resultSize);
    
        if (index != -1) 
        {
            // Если id уже есть, обновляем существующую запись
            mergeStatData(&resultArray[index], &arrays[secondArray - 1][i]);
        } 
        else 
        {
            // Если id нет, добавляем новую запись
            resultArray[resultSize++] = arrays[secondArray - 1][i];
        }
    }

    // Перезапись файла с новым массивом
    rewind(file);
    
    // Количество массивов
    fwrite(&arrayCount, sizeof(int), 1, file);
    
    for (int i = 0; i < arrayCount; i++) 
    {
        if (i == firstArray - 1) 
        {
            // Новый размер объединённого массива
            fwrite(&resultSize, sizeof(int), 1, file); 
            fwrite(resultArray, sizeof(StatData), resultSize, file);
        } 
        else if (i != secondArray - 1) 
        {
            fwrite(&arraySizes[i], sizeof(int), 1, file);
            fwrite(arrays[i], sizeof(StatData), arraySizes[i], file);
        }
    }

    printf("\nОбъединение завершено. Данные обновлены в файле.\n");

    // Очистка памяти
    for (int i = 0; i < arrayCount; i++) 
    {
        free(arrays[i]);
    }
    
    free(arrays);
    free(arraySizes);
    free(resultArray);

    fclose(file);
}
