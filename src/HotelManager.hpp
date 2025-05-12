#pragma once
#include <vector>
#include <stdexcept>
#include "Guest.hpp"
#include "Booking.hpp"

class HotelManager
{
private:
    std::vector<Guest *> guests;
    std::vector<BookingEntity *> entities;
    std::vector<Reservation *> reservations;
    u_int64_t next_reservation_id = 1;

public:
    ~HotelManager()
    {
        for (auto g : guests)
            delete g;
        for (auto e : entities)
            delete e;
        for (auto res : reservations)
            delete res;
    }

    void addGuest(Guest *guest)
    {
        guests.push_back(guest);
    }

    void addEntity(BookingEntity *entity)
    {
        entities.push_back(entity);
    }

    // Основные операции
    Reservation *createReservation(
        u_int64_t guest_id,
        u_int64_t entity_id,
        const DateTimeRange &period)
    {

        Guest *guest = nullptr;
        for (size_t i = 0; i < guests.size(); ++i)
        {
            Guest *current_guest = guests[i];
            if (current_guest->getId() == guest_id)
            {
                guest = current_guest;
                break;
            }
        }
        if (guest == nullptr)
        {
            throw std::runtime_error("Guest not found");
        }

        BookingEntity *entity = nullptr;
        for (size_t i = 0; i < entities.size(); ++i)
        {
            BookingEntity *current_entity = entities[i];
            if (current_entity->getId() == entity_id)
            {
                entity = current_entity;
                break;
            }
        }
        if (entity == nullptr)
        {
            throw std::runtime_error("Room not found");
        }

        if (!entity->isAvailable(period))
        {
            throw std::runtime_error("Room is occupied");
        }

        Reservation *reservation = new Reservation(
            next_reservation_id++,
            entity,
            guest,
            period);

        reservations.push_back(reservation);
        entity->book(reservation->getId(), guest, period);

        return reservation;
    }

    void cancelReservation(u_int64_t reservation_id)
    {
        size_t index = 0;
        for (; index < reservations.size(); ++index)
        {
            if (reservations[index]->getId() == reservation_id)
            {
                break;
            }
        }

        if (index == reservations.size())
        {
            throw std::runtime_error("Reservation not found");
        }

        reservations[index]->getEntity()->removeReservation(reservation_id);
        delete reservations[index];
        reservations.erase(reservations.begin() + index);
    }

    std::vector<BookingEntity *> getAllRooms() const { return entities; }
    std::vector<Guest *> getAllGuests() const
    {
        return guests;
    }

    std::vector<Reservation *> getAllReservations() const
    {
        return reservations;
    }
};