// PMSRestAPI.hpp
#pragma once
#include "HotelManager.hpp"
#include <crow.h>
#include <crow/middlewares/cors.h>

class PMSRestAPI
{
private:
    HotelManager &manager;
    crow::App<crow::CORSHandler> app;

public:
    PMSRestAPI(HotelManager &mgr) : manager(mgr)
    {
        // Настройка CORS
        auto &cors = app.get_middleware<crow::CORSHandler>();
        cors
            .global()
            .headers("Content-Type")
            .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method);
    }

    void setupRoutes()
    {
        // Получить список всех комнат
        CROW_ROUTE(app, "/api/rooms")
        ([this]
         {
            auto rooms = this->manager.getAllRooms();
            crow::json::wvalue result;
            result["rooms"] = crow::json::wvalue::list();

            for (size_t i = 0; i < rooms.size(); ++i) {
                result["rooms"][i]["id"] = rooms[i]->getId();
                result["rooms"][i]["price"] = rooms[i]->getPrice();
                result["rooms"][i]["max_guests"] = rooms[i]->getMaxGuests();
                result["rooms"][i]["status"] = rooms[i]->isAvailable(DateTimeRange{DateTime(), DateTime()}) ? "available" : "occupied";
            }

            return result; });

        // Получить список всех гостей
        CROW_ROUTE(app, "/api/guests")
        ([this]
         {
            auto guests = this->manager.getAllGuests();
            crow::json::wvalue result;
            result["guests"] = crow::json::wvalue::list();

            for (size_t i = 0; i < guests.size(); ++i) {
                result["guests"][i]["id"] = guests[i]->getId();
                result["guests"][i]["first_name"] = guests[i]->getFirstName();
                result["guests"][i]["last_name"] = guests[i]->getSecondName();
                result["guests"][i]["email"] = guests[i]->getEmail();
                result["guests"][i]["phone"] = guests[i]->getPhone();
            }

            return result; });

        // Получить список всех бронирований
        CROW_ROUTE(app, "/api/reservations")
        ([this]
         {
            auto reservations = this->manager.getAllReservations();
            crow::json::wvalue result;
            result["reservations"] = crow::json::wvalue::list();

            for (size_t i = 0; i < reservations.size(); ++i) {
                result["reservations"][i]["id"] = reservations[i]->getId();
                result["reservations"][i]["guest_id"] = reservations[i]->getGuest()->getId();
                result["reservations"][i]["room_id"] = reservations[i]->getRoom()->getId();
                result["reservations"][i]["checkin"] = reservations[i]->getDateRange().getStart().toString();
                result["reservations"][i]["checkout"] = reservations[i]->getDateRange().getEnd().toString();
                result["reservations"][i]["status"] = "active"; // Можно добавить логику определения статуса
            }

            return result; });

        // Создать бронирование
        CROW_ROUTE(app, "/api/reservations").methods("POST"_method)([this](const crow::request &req)
                                                                    {
            try {
                auto body = crow::json::load(req.body);
                if (!body) {
                    return crow::response(400, "Invalid JSON");
                }

                uint64_t guest_id = body["guest_id"].u();
                uint64_t room_id = body["room_id"].u();
                std::string checkin = body["checkin"].s();
                std::string checkout = body["checkout"].s();
                std::cout << checkin << " " << checkout << std::endl;
                DateTime checkin_date(checkin);
                DateTime checkout_date(checkout);
                DateTimeRange range{checkin_date, checkout_date};

                auto reservation = manager.createReservation(guest_id, room_id, range);

                crow::json::wvalue response;
                response["reservation_id"] = reservation->getId();
                return crow::response(response);
            } catch (const std::exception& e) {
                return crow::response(400, e.what());
            } });

        // Отменить бронирование
        CROW_ROUTE(app, "/api/reservations/<uint>").methods("DELETE"_method)([this](uint64_t reservation_id)
                                                                             {
            try {
                manager.cancelReservation(reservation_id);
                return crow::response(200, "Reservation cancelled");
            } catch (const std::exception& e) {
                return crow::response(404, e.what());
            } });
    }

    void run(int port = 8080)
    {
        app.port(port).multithreaded().run();
    }
};