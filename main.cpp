#include "src/HotelManager.hpp"
#include "src/PMSRestAPI.hpp"
#include <memory>

int main()
{
    // Инициализация менеджера
    HotelManager manager;

    // Добавляем тестовые данные
    auto guest = new Guest(
        1,
        "Иван",
        "Петров",
        "+79161234567",
        "ivan@example.com",
        1234567890,
        DateTime(1, 1, 1990),
        100);
    manager.addGuest(guest);

    auto room = new Room(101, 5000.0, 2);
    manager.addRoom(room);

    // Запускаем REST API
    PMSRestAPI api(manager);
    api.setupRoutes();

    std::cout << "Server running on http://localhost:8080\n";
    api.run();

    return 0;
}