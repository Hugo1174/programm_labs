#include "GraphicPrimitive.h"
#include "DocController.h"

// Представление GUI
void printMenu()
{
    std::cout << "\nВыберитe действие:\n";
    std::cout << "1. Создать документ\n";
    std::cout << "2. Открыть документ по пути\n";
    std::cout << "3. Импорт документа\n";
    std::cout << "4. Экспорт документа\n";
    std::cout << "5. Добавить примитив\n";
    std::cout << "6. Удалить примитив\n";
    std::cout << "7. Показать информацию о текущем документе\n";
    std::cout << "8. Выход\n";
    std::cout << "Введите номер действия: ";
}

int main()
{
    Controller controller;

    while (true)
    {
        printMenu();
        int choice;

        while (!(std::cin >> choice))
        {
            std::cin.clear();                                                   // Сбрасываем состояние потока
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорируем некорректный ввод
            std::cout << "Неккоректный ввод. \n";
            printMenu();
        }

        switch (choice)
        {
        case 1:
            controller.createDocument();
            break;
        case 2:
        {
            std::cout << "Введите путь к документу: ";
            std::string path;
            std::cin >> path;
            controller.openDocumentByPath(path);
            break;
        }
        case 3:
        {
            std::cout << "Введите путь до импортируемого документа: ";
            std::string path;
            std::cin >> path;
            std::cout << "Импорт документа из файла: " << std::endl;
            controller.importDocument(path);
            std::cout << std::endl;
            break;
        }
        case 4:
        {
            std::cout << "Введите путь до экспортируемого документа: " << std::endl;
            std::string path;
            std::cin >> path;
            controller.exportDocument(path);
            std::cout << std::endl;
            break;
        }
        case 5:
        {
            std::cout << "Введите тип примитива(Circle, Square, Rectangle): " << std::endl;
            std::string type;
            std::cin >> type;
            controller.addGraphicPrimitive(type);
            break;
        }
        case 6:
        {
            std::cout << "Введите ID графического примитива для удаления: " << std::endl;
            int id;
            std::cin >> id;
            controller.deleteGraphicPrimitive(id);
            std::cout << std::endl;
            break;
        }
        case 7:
        {
            std::cout << "Показать информацию о текущем документе: " << std::endl;
            controller.showCurrentDocumentInfo();
            std::cout << std::endl;
            break;
        }
        case 8:
        {
            std::cout << "Выход.\n";
            return 0;
        }
        default:
            std::cout << "Неккоректный выбор";
        }
    }
}