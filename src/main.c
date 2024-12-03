#include "StatData.h"
#include "StoreDump.h"
#include "LoadDump.h"
#include "SortDump.h"
#include "JoinDump.h"

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
    
    int option = -1;

    while (1) 
    {
        showMenu();
        scanf("%d", &option);

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
