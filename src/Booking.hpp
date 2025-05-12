#pragma once
#include <iostream>
#include <vector>
#include "DateTime.hpp"
#include "Guest.hpp"

class Room;

class Reservation
{
private:
    u_int64_t id;
    Guest *guest;
    Room *room;
    DateTimeRange date_range;

public:
    Reservation(u_int64_t id, Room *room, Guest *guest, DateTimeRange date_range) : id(id), guest(guest), room(room), date_range(date_range) {};
    u_int64_t getId() const
    {
        return id;
    }
    Room *getRoom()
    {
        return room;
    }
    Guest *getGuest()
    {
        return guest;
    }
    DateTimeRange getDateRange()
    {
        return date_range;
    }
};

class Room
{
private:
    struct TimeSlot
    {
        DateTimeRange range;
        u_int64_t reservation_id;
    };

    u_int64_t id;
    double price;
    std::vector<TimeSlot> reservation_time;
    unsigned int max_guest;

public:
    Room(u_int64_t id, double price, unsigned int max_guest) : id(id), price(price), max_guest(max_guest) {};

    double calculate_cost(int nights)
    {
        return price * nights;
    }

    bool isAvailable(DateTimeRange date_range)
    {
        for (auto slot : reservation_time)
        {
            if (slot.range.overlaps(date_range))
            {
                return false;
            }
        }
        return true;
    }

    Reservation book(u_int64_t id, Guest *guest, DateTimeRange range)
    {
        TimeSlot slot = {range, id};
        reservation_time.push_back(slot);
        return Reservation(id, this, guest, range);
    }

    u_int64_t getId() const
    {
        return id;
    }

    double getPrice() const { return price; }
    unsigned int getMaxGuests() const { return max_guest; }

    void removeReservation(u_int64_t res_id)
    {
        std::vector<TimeSlot> new_slots;
        for (auto &slot : reservation_time)
        {
            if (slot.reservation_id != res_id)
            {
                new_slots.push_back(slot);
            }
        }
        reservation_time = new_slots;
    }
};


class BookingEntity {
private:
    struct TimeSlot {
        DateTimeRange range;
        u_int64_t reservation_id;
    };

    double price;
    std::vector<TimeSlot> reservation_time;
public:
    BookingEntity(u_int64_t id, double price) : id(id), price(price) {};
    
    ~BookingEntity() {
        for (auto slot : reservation_time) {
            delete slot.range;
        }
    }

    

};