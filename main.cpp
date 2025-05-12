#include "src/HotelManager.hpp"
#include "src/PMSRestAPI.hpp"
#include <memory>

int main()
{
    // Инициализация менеджера
    HotelManager manager;

    // Добавляем тестовые данные
    auto guest1 = new Guest(
        1,
        "Иван",
        "Петров",
        "+79161234567",
        "ivan@example.com",
        1234567890,
        DateTime(1, 1, 1990),
        100);
    manager.addGuest(guest1);

    auto guest2 = new Guest(
        2,
        "Петр",
        "Иванов",
        "+79107654321",
        "petr@example.com",
        9987654321,
        DateTime(2, 3, 1988),
        100);
    manager.addGuest(guest2);

    auto guest3 = new Guest(
        3,
        "Сергей",
        "Сергеев",
        "+79113214567",
        "sergey@example.com",
        4321567890,
        DateTime(12, 5, 1999),
        100);
    manager.addGuest(guest3);

    auto guest4 = new Guest(
        4,
        "Алексей",
        "Алексеев",
        "+79101237654",
        "alexei@example.com",
        1234765890,
        DateTime(23, 7, 1979),
        100);
    manager.addGuest(guest4);

    auto room1 = new Room(101, 5000.0, 2);
    manager.addEntity(room1);

    auto room2 = new Room(102, 5500.0, 3);
    manager.addEntity(room2);

    auto room3 = new Room(103, 6000.0, 4);
    manager.addEntity(room3);

    auto room4 = new Room(104, 4500.0, 1);
    manager.addEntity(room4);

    // Запускаем REST API
    PMSRestAPI api(manager);
    api.setupRoutes();

    std::cout << "Server running on http://localhost:8080\n";
    api.run();

    return 0;
}