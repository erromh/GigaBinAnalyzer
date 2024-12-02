#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/stat.h>  

typedef struct StatData
{
    long id;
    int count;
    float cost;

    unsigned int primary : 1;
    unsigned int mode : 3;

} StatData;

void storeDump(char* pathToFile) {
    int fd = open(pathToFile, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Ошибка открытия файла");
        return;
    }

    int arrayCount;
    printf("Введите количество массивов структур: ");
    if (scanf("%i", &arrayCount) != 1 || arrayCount <= 0) {
        printf("Некорректное количество массивов.\n");
        close(fd);
        return;
    }

    for (size_t i = 0; i < arrayCount; i++) {
        int arraySize;
        printf("Введите количество структур в массиве %zu: ", i + 1);
        if (scanf("%i", &arraySize) != 1 || arraySize <= 0 || arraySize > 10000) {
            printf("Некорректное количество структур. Пропускаем массив.\n");
            continue;
        }

        StatData* currentArray = malloc(arraySize * sizeof(StatData));
        if (!currentArray) {
            perror("Ошибка выделения памяти");
            break;
        }

        for (size_t j = 0; j < arraySize; j++) {
            printf("Enter data %zu:\n", i + 1);
            
            printf("  ID: ");
            scanf("%ld", &currentArray[i].id);
            
            printf("  Count: ");
            scanf("%d", &currentArray[i].count);
            
            printf("  Cost: ");
            scanf("%f", &currentArray[i].cost);
            
            printf("  Primary (0/1): ");
            unsigned int primary_tmp;
            scanf("%u", &primary_tmp);
            currentArray[i].primary = primary_tmp;

            printf("  Mode (0-7): ");
            unsigned int mode_tmp;
            scanf("%u", &mode_tmp);
            currentArray[i].mode = mode_tmp;

            printf("\n");
        }

        // Запись размера массива
        if (write(fd, &arraySize, sizeof(int)) != sizeof(int)) {
            perror("Ошибка записи размера массива");
            free(currentArray);
            break;
        }

        // Запись массива структур
        if (write(fd, currentArray, arraySize * sizeof(StatData)) != arraySize * sizeof(StatData)) {
            perror("Ошибка записи массива структур");
            free(currentArray);
            break;
        }

        free(currentArray);
    }

    if (close(fd) == -1) {
        perror("Ошибка закрытия файла");
    }
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
