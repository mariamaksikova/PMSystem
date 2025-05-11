#pragma once
#include <vector>
#include <stdexcept>
#include "Guest.hpp"
#include "Booking.hpp"

class HotelManager
{
private:
    std::vector<Guest *> guests;
    std::vector<Room *> rooms;
    std::vector<Reservation *> reservations;
    uint64_t next_reservation_id = 1;

public:
    ~HotelManager()
    {
        for (auto g : guests)
            delete g;
        for (auto r : rooms)
            delete r;
        for (auto res : reservations)
            delete res;
    }

    void addGuest(Guest *guest)
    {
        guests.push_back(guest);
    }

    void addRoom(Room *room)
    {
        rooms.push_back(room);
    }

    // Основные операции
    Reservation *createReservation(
        uint64_t guest_id,
        uint64_t room_id,
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

        Room *room = nullptr;
        for (size_t i = 0; i < rooms.size(); ++i)
        {
            Room *current_room = rooms[i];
            if (current_room->getId() == room_id)
            {
                room = current_room;
                break;
            }
        }
        if (room == nullptr)
        {
            throw std::runtime_error("Room not found");
        }

        if (!room->isAvailable(period))
        {
            throw std::runtime_error("Room is occupied");
        }

        Reservation *reservation = new Reservation(
            next_reservation_id++,
            room,
            guest,
            period);

        reservations.push_back(reservation);
        room->book(reservation->getId(), guest, period);

        return reservation;
    }

    void cancelReservation(uint64_t reservation_id)
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

        reservations[index]->getRoom()->removeReservation(reservation_id);
        delete reservations[index];
        reservations.erase(reservations.begin() + index);
    }

    std::vector<Room *> getAllRooms() const { return rooms; }
    std::vector<Guest *> getAllGuests() const
    {
        return guests;
    }

    std::vector<Reservation *> getAllReservations() const
    {
        return reservations;
    }
};