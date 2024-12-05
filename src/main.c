#include "StatData.h"
#include "StoreDump.h"
#include "LoadDump.h"
#include "SortDump.h"
#include "JoinDump.h"

int safeMenuInput()
{
    int value;
    char buffer[64];
    while (1)
    {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        {
            printf("Ошибка чтения ввода. Попробуйте снова.\n");
            continue;
        }

        if (sscanf(buffer, "%d", &value) == 1)
        {
            return value;
        }
        printf("\nНекорректный ввод. Введите целое число.\n-> ");
    }
}


void showMenu() 
{
    printf("\n=== Меню ===\n");
    printf("\n1. Создать и заполнить массивы структур\n");
    printf("2. Вывести данные из файла\n");
    printf("3. Сортировать массивы по полю cost \n");
    printf("4. Объединить массивы\n");
    printf("0. Выход\n");
    printf("\nВыберите опцию: ");
}

int main()
{
    char fileName[] = "data.bin";
    
    while (1) 
    {
        showMenu();
        
        int option = safeMenuInput();
        // scanf("%d", &option);

        switch (option) 
        {
            case 1:
                printf("\n--- Создание и заполнение массивов структур ---\n\n");
                storeDump(fileName);
                break;
        
            case 2:
                printf("\n--- Загрузка данных из файла ---\n\n");
                loadDump(fileName);
                break;
        
            case 3:
                printf("\n--- Сортировка массивов ---\n\n");
                sortDump(fileName);
                printf("\n--- Загрузка данных после сортировки ---\n");
                loadDump(fileName);
                break;
        
            case 4:
                printf("\n--- Объединение массивов ---\n\n");
                joinDump(fileName);
                printf("\n--- Загрузка данных после объединения ---\n");
                loadDump(fileName);
                break;
        
            case 0:
                printf("\nДо свидания!\n");
                return 0;
        
            default:
                printf("\nНекорректный выбор. Попробуйте снова.\n");
                break;
        }
    }

    return 0;
}
