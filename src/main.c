#include <stdio.h>
#include <stdlib.h>

typedef struct StatData
{
    long id;
    int count;
    float cost;

    unsigned int primary : 1;
    unsigned int mode : 3;

} StatData;

void storeDump(char* pathToFile)
{
    FILE *file = fopen(pathToFile, "rb");

    if (file)
    {
        printf("File %s already exists.\n", pathToFile);
        fclose(file);
    }
    else
    {
        printf("No such file here '%s', creating new.\n", pathToFile);
    }

    // Массив данных
    StatData statData[2];

    statData[0].id = 4;
    statData[0].count = 13;
    statData[0].cost = 44;
    statData[0].primary = 0;
    statData[0].mode = 3;

    statData[1].id = 65;
    statData[1].count = 90;
    statData[1].cost = 45;
    statData[1].primary = 1;
    statData[1].mode = 4;

    int size = sizeof(statData) / sizeof(statData[0]);

    // Открытие файла в режиме "wb" для записи
    file = fopen(pathToFile, "wb");
    if (file == NULL)
    {
        perror("Ошибка открытия файла для записи");
        return;
    }

    // Запись данных в файл
    if (fwrite(statData, sizeof(StatData), size, file) != size)
    {
        perror("Ошибка записи в файл");
    }
    else
    {
        printf("Success\n");
    }

    // Закрытие файла
    fclose(file);
}

void LoadDump(char* pathToFile)
{
    FILE *file = fopen(pathToFile, "rb");

    if (file == NULL)
    {
        perror("Ошибка открытия файла");
        return;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    size_t count = file_size / sizeof(StatData);

    StatData* statData = malloc(count * sizeof(StatData));

    if (statData == NULL)
    {
        perror("Ошибка выделения памяти");
        fclose(file);
        return;
    }

    // Чтение данных из файла
    if (fread(statData, sizeof(StatData), count, file) != count)
    {
        perror("Ошибка чтения из файла");
    }
    else
    {
        // Вывод прочитанных данных
        for (size_t i = 0; i < count; i++)
        {
            printf("ID: %ld, count: %d, cost: %.2f, primary: %u, mode: %u\n",
                   statData[i].id, statData[i].count, statData[i].cost,
                   statData[i].primary, statData[i].mode);
        }
    }

    // Закрытие файла и освобождение памяти
    fclose(file);
    free(statData);
}

int main()
{
    storeDump("test.txt");
    LoadDump("test.txt");

    return 0;
}
