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

    int arrayCount;
    printf("Введите колличество массивов структур: ");
    scanf("%i", arrayCount);

    if (arrayCount == 0)
    {
        printf("Невозможно создать массивы с нулевым количеством.\n");
        return;
    }
    
    // Выделяем память для хранения указателей на массивы
    StatData** arraysList = malloc(arrayCount * sizeof(StatData*));
    int* arraysSize = malloc(arrayCount * sizeof(StatData));

    if (!arraysList || !arraysSize)
    {
        perror("Ошибка выделения памяти");

        free(arraysList);
        free(arraysSize);

        return;
    }
    
    for (size_t i = 0; i < arrayCount; i++)
    {
        printf("Введите колличество структур в масииве %zu: ", i + 1);
        scanf("%i", arraysSize[i]);

        if (arraysSize[i] == 0)
        {
            printf("Пропускаем создание массива с нулевым размером\n");
            arraysList[i] = NULL;
            continue;
        }
        
        arraysList[i] = malloc(arraysSize[i] * sizeof(StatData));
        
        if (!arraysList[i])
        {
            perror("Ошибка выделения памяти для массива структур\n");

            for (size_t j = 0; j < i; j++)
            {
                free(arraysList[i]);
            }

            free(arraysList);
            free(arraysSize);

            return;
        }
        
        for (size_t j = 0; j < arraysSize[i]; j++)
        {
            printf("Enter data %zu:\n", i + 1);
            
            printf("  ID: ");
            scanf("%ld", &arraysList[i][j].id);
            
            printf("  Count: ");
            scanf("%d", &arraysList[i][j].count);
            
            printf("  Cost: ");
            scanf("%f", &arraysList[i][j].cost);
            
            printf("  Primary (0/1): ");
            unsigned int primary_tmp;
            scanf("%u", &primary_tmp);
            arraysList[i][j].primary = primary_tmp;

            printf("  Mode (0-7): ");
            unsigned int mode_tmp;
            scanf("%u", &mode_tmp);
            arraysList[i][j].mode = mode_tmp;

            printf("\n");
        }
    }
        
    // Открытие файла в режиме "wb" для записи
    file = fopen(pathToFile, "wb");
    if (file == NULL)
    {
        perror("Ошибка открытия файла для записи");

        for (size_t j = 0; j < arrayCount; j++)
        {
            free(arraysList[j]); 
        }
        
        free(arraysList);
        free(arraysSize);

        return;
    }

    // Запись массива в файл
    for (size_t j = 0; j < arrayCount; j++)
    {
        if (arraysList[j] && arraysSize[j] > 0)
        {
            // Сначала записываем размер массива
            if (fwrite(&arraysSize[j], sizeof(size_t), 1, file) != 1 ||  
                fwrite(arraysList[j], sizeof(StatData), arraysSize[j], file) != arraysList[j]) 
            {
                perror("Ошибка записи в файл");
                break;
            }   
        }
    }
    
    printf("Данные успешно записаны в файл\n");

    for (size_t j = 0; j < arrayCount; j++)
    {
        free(arraysList[j]);
    }
    free(arraysList);
    free(arraysSize);
    
    // Закрытие файла
    fclose(file);
}

void loadDump(char* pathToFile)
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

void sortDump()
{

}

int main()
{
    storeDump("test.txt");
    loadDump("test.txt");

    return 0;
}
