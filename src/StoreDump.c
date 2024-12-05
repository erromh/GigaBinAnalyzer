#include "StoreDump.h"

// Функция безопасного ввода целого числа
int safeInputInt(const char* prompt)
{
    int value;
    char buffer[64];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Ошибка чтения ввода. Попробуйте снова.\n");
            continue;
        }
        if (sscanf(buffer, "%d", &value) == 1)
        {
            return value;
        }
        printf("Некорректный ввод. Введите целое число.\n");
    }
}

// Функция безопасного ввода числа с плавающей точкой
float safeInputFloat(const char* prompt)
{
    float value;
    char buffer[64];
    while (1)
    {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Ошибка чтения ввода. Попробуйте снова.\n");
            continue;
        }
        if (sscanf(buffer, "%f", &value) == 1)
        {
            return value;
        }
        printf("Некорректный ввод. Введите число с плавающей точкой.\n");
    }
}

void storeDump(const char* pathToFile)
{
    int arrayCount = safeInputInt("Введите количество массивов структур: ");
    if (arrayCount <= 0)
    {
        printf("Количество массивов должно быть положительным.\n");
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
        int arraySize = safeInputInt("Введите количество структур в массиве: ");
        if (arraySize <= 0)
        {
            printf("Количество структур в массиве должно быть положительным. Пропускаем массив.\n");
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
            array[j].id = safeInputInt("  ID: ");
            array[j].count = safeInputInt("  Count: ");
            array[j].cost = safeInputFloat("  Cost: ");

            unsigned int primary_tmp = safeInputInt("  Primary (0/1): ");
            array[j].primary = primary_tmp ? 1 : 0;

            unsigned int mode_tmp = safeInputInt("  Mode (0-7): ");
            array[j].mode = (mode_tmp <= 7) ? mode_tmp : 7;

            printf("\n");
        }

        fwrite(array, sizeof(StatData), arraySize, file); // Сохраняем массив структур
        free(array);
    }

    fclose(file);
    printf("Данные успешно записаны в файл: %s\n", pathToFile);
}