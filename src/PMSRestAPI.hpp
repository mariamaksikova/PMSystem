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
            .methods("GET"_method, "POST"_method, "PUT"_method, "DELETE"_method)
            .origin("*");
    }

    void setupRoutes()
    {
        // Получить список всех объектов бронирования (бывшие комнаты)
        CROW_ROUTE(app, "/api/entities")
        ([this]
         {
            auto entities = this->manager.getAllRooms();
            crow::json::wvalue result;
            result["entities"] = crow::json::wvalue::list();

            for (size_t i = 0; i < entities.size(); ++i) {
                result["entities"][i]["id"] = entities[i]->getId();
                result["entities"][i]["price"] = entities[i]->getPrice();
                result["entities"][i]["status"] = entities[i]->isAvailable(DateTimeRange{DateTime(), DateTime()}) ? "available" : "occupied";
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
                result["guests"][i]["loyalty_points"] = 0; // Добавляем поле для бонусных баллов
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
                result["reservations"][i]["entity_id"] = reservations[i]->getEntity()->getId();
                result["reservations"][i]["checkin"] = reservations[i]->getDateRange().getStart().toString();
                result["reservations"][i]["checkout"] = reservations[i]->getDateRange().getEnd().toString();
                result["reservations"][i]["status"] = "active";
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

                u_int64_t guest_id = body["guest_id"].u();
                u_int64_t entity_id = body["entity_id"].u();
                std::string checkin = body["checkin"].s();
                std::string checkout = body["checkout"].s();
                
                DateTime checkin_date(checkin);
                DateTime checkout_date(checkout);
                DateTimeRange range{checkin_date, checkout_date};

                auto reservation = manager.createReservation(guest_id, entity_id, range);

                crow::json::wvalue response;
                response["reservation_id"] = reservation->getId();
                return crow::response(response);
            } catch (const std::exception& e) {
                return crow::response(400, e.what());
            } });

        // Отменить бронирование
        CROW_ROUTE(app, "/api/reservations/<uint>").methods("DELETE"_method)([this](u_int64_t reservation_id)
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