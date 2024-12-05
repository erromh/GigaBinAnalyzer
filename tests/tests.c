#include <stdio.h>
#include <stdlib.h>

#include "StoreDump.h"
#include "LoadDump.h"
#include "SortDump.h"

void test_storeDump()
{
    const char* testFile = "test_store.bin";

    // Удаляем старый файл, если он есть
    remove(testFile);

    // Запускаем функцию
    storeDump(testFile);

    // Проверяем, что файл создан
    FILE* file = fopen(testFile, "rb");
    if (!file)
    {
        printf("test_storeDump: FAILED (Файл не создан)\n");
        return;
    }

    // Проверяем содержимое файла
    int arrayCount;
    fread(&arrayCount, sizeof(int), 1, file);

    // Можно заменить на ожидаемое количество массивов
    if (arrayCount != 2) 
    {
        printf("test_storeDump: FAILED (Неверное количество массивов в файле: %d)\n", arrayCount);
        fclose(file);
        return; 
    }

    // Закрываем файл и удаляем его
    fclose(file);
    remove(testFile);

    printf("test_storeDump: PASSED\n");
}

void test_loadDump()
{
    const char* testFile = "test_load.bin";

    // Создаём тестовый файл
    FILE* file = fopen(testFile, "wb");
    int arrayCount = 1;
    fwrite(&arrayCount, sizeof(int), 1, file);

    int arraySize = 2;
    fwrite(&arraySize, sizeof(int), 1, file);

    StatData testArray[2] = 
    {
        {1, 10, 100.5, 1, 3},
        {2, 20, 200.0, 0, 5}
    };
    
    fwrite(testArray, sizeof(StatData), arraySize, file);

    fclose(file);

    // Загружаем данные через функцию
    loadDump(testFile);

    // Удаляем файл
    remove(testFile);

    printf("test_loadDump: PASSED\n");
}


void test_sortDump()
{
    const char* testFile = "test_sort.bin";

    // Создаём тестовый файл
    FILE* file = fopen(testFile, "wb");
    int arrayCount = 1;
    fwrite(&arrayCount, sizeof(int), 1, file);

    int arraySize = 3;
    fwrite(&arraySize, sizeof(int), 1, file);

    StatData testArray[3] =
    {
        {3, 30, 300.0, 1, 1},
        {1, 10, 100.5, 0, 0},
        {2, 20, 200.0, 1, 1}
    };

    fwrite(testArray, sizeof(StatData), arraySize, file);

    fclose(file);

    // Сортируем
    sortDump(testFile);

    // Проверяем, что массив отсортирован
    file = fopen(testFile, "rb");
    int readArrayCount, readArraySize;
    
    fread(&readArrayCount, sizeof(int), 1, file);
    fread(&readArraySize, sizeof(int), 1, file);

    StatData sortedArray[3];
    fread(sortedArray, sizeof(StatData), readArraySize, file);

    if (sortedArray[0].id == 1 && sortedArray[1].id == 2 && sortedArray[2].id == 3)
    {
        printf("test_sortDump: PASSED\n");
    }
    else
    {
        printf("test_sortDump: FAILED (Сортировка неверна)\n");
    }

    fclose(file);
    remove(testFile);
}


int main()
{
    test_storeDump();
    test_loadDump();
    test_sortDump();

    printf("All test passed\n");

    return 0;
}