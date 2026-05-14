#pragma once
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <limits> 
#include "Document.h"

class Controller {
private:
    std::vector<std::shared_ptr<Document>> documents; // Хранение всех документов
    int currentDocumentIndex; // Индекс текущего документа
public:
    
    Controller() : currentDocumentIndex(-1) {}
    
    void createDocument() {
        std::cout << "Введите имя нового документа: ";
        std::string filename;
        std::cin >> filename;

        std::cout << "Введите путь для нового документа: ";
        std::string path;
        std::cin >> path;

        auto newDocument = std::make_shared<Document>(filename, path);
        documents.push_back(newDocument);
        currentDocumentIndex = documents.size() - 1; // Установить новый документ текущим
        std::cout << "Документ успешно создан и установлен как текущий.";
    }

    void openDocumentByPath(const std::string& path) {
        for (size_t i = 0; i < documents.size(); ++i) {
            if (documents[i]->getPathDocument() == path) {
                currentDocumentIndex = i;
                std::cout << "Документ успешно открыт: " << documents[i]->getNameDocument() << "";
                return;
            }
        }
        std::cout << "Документ с указанным путем не найден.";
    }
  
    void importDocument(const std::string& path) {
        if (currentDocumentIndex >= 0) {
            for (const auto& doc : documents) {
                if (doc->getPathDocument() == path) {
                    for (const auto& primitive : doc->getPrimitiveList()) {
                        documents[currentDocumentIndex]->addGraphicPrimitive(primitive);
                    }
                    std::cout << "Примитивы импортированы из документа: " << path << "\n";
                    return;
                }
            }
            std::cout << "Документ с указанным путем не найден для импорта.\n";
        } else {
            std::cout << "Нет текущего документа для импорта.\n";
        }
    }

    void exportDocument(const std::string& path) {
        if (currentDocumentIndex >= 0) {
            for (auto& doc : documents) {
                if (doc->getPathDocument() == path) {
                    for (const auto& primitive : documents[currentDocumentIndex]->getPrimitiveList()) {
                        doc->addGraphicPrimitive(primitive);
                    }
                    std::cout << "Примитивы экспортированы в документ: " << path << "\n";
                    return;
                }
            }
            std::cout << "Документ с указанным путем не найден для экспорта.\n";
        } else {
            std::cout << "Нет текущего документа для экспорта.\n";
        }
    }
    
    void addGraphicPrimitive(const std::string& type) {
        if (!documents[currentDocumentIndex]) {
            std::cout << "Нет текущего документа. Сначала создайте или выберите документ." << std::endl;
            return;
        }
        std::shared_ptr<GraphicPrimitive> primitive;
        if (type == "Circle") {
            double radius;
            while (true) {
                std::cout << "Введите радиус: ";
                std::cin >> radius;

                if (std::cin.fail()) {
                    std::cin.clear(); // Очистка состояния потока
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорирование некорректного ввода
                    std::cout << "Ошибка: введите число." << std::endl;
                } else {
                    break; // Ввод корректен, выходим из цикла
                }
            }
            primitive = std::make_shared<Circle>(radius);
        } else if (type == "Rectangle") {
            double length;
            double width;

            while (true){
                std::cout << "Введите длину: ";
                std::cin >> length;
                std::cout << "Введите ширину: ";
                std::cin >> width;

                if (std::cin.fail()) {
                    std::cin.clear(); // Очистка состояния потока
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорирование некорректного ввода
                    std::cout << "Ошибка: введите число." << std::endl;
                } else {
                    break; // Ввод корректен, выходим из цикла
                }
            }
            primitive = std::make_shared<Rectangle>(width, length);
        } else if (type == "Square") {
            double length;

            while (true){
                std::cout << "Введите длину: ";
                std::cin >> length;

                if (std::cin.fail()) {
                    std::cin.clear(); // Очистка состояния потока
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Игнорирование некорректного ввода
                    std::cout << "Ошибка: введите число." << std::endl;
                } else {
                    break; // Ввод корректен, выходим из цикла
                }
            }
            primitive = std::make_shared<Square>(length);
        } else {
            std::cout << "Неизвестный тип графического примитива: " << type << std::endl;
            return;
        }
        documents[currentDocumentIndex]->addGraphicPrimitive(primitive);
    }
    
    void deleteGraphicPrimitive(int id) {
        if (!documents[currentDocumentIndex]) {
            std::cout << "Нет текущего документа. Сначала создайте или выберите документ" << std::endl;
            return;
        }

        documents[currentDocumentIndex]->deleteGraphicPrimitive(id);
    }
    
    void showCurrentDocumentInfo() const {
        if (documents[currentDocumentIndex]) {
            std::cout << "Информация о текущем документе:" << std::endl;
            documents[currentDocumentIndex]->getNameDocument();
            documents[currentDocumentIndex]->getId();
            documents[currentDocumentIndex]->getPathDocument();
            
            // Вывод информации о графических примитивах
            documents[currentDocumentIndex]->showGraphicPrimitivesInfo();
        } else {
            std::cout << "Нет текущего документа" << std::endl;
        }
    }
};