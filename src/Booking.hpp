#pragma once
#include <iostream>
#include <vector>
#include "DateTime.hpp"
#include "Guest.hpp"

class BookingEntity;

class Reservation
{
private:
    u_int64_t id;
    Guest *guest;
    BookingEntity *entity;
    DateTimeRange date_range;

public:
    Reservation(u_int64_t id, BookingEntity *entity, Guest *guest, DateTimeRange date_range) : id(id), guest(guest), entity(entity), date_range(date_range) {};
    u_int64_t getId() const
    {
        return id;
    }
    BookingEntity *getEntity()
    {
        return entity;
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

class BookingEntity
{
protected:
    struct TimeSlot
    {
        DateTimeRange range;
        u_int64_t reservation_id;
    };
    u_int64_t id;
    double price;
    std::vector<TimeSlot> reservation_time;

public:
    BookingEntity(u_int64_t id, double price) : id(id), price(price) {};

    virtual double calculate_cost(int duration) = 0;

    bool isAvailable(DateTimeRange range)
    {
        for (auto slot : reservation_time)
        {
            if (slot.range.overlaps(range))
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

    u_int64_t getId() const { return id; }
    double getPrice() const { return price; }
};

class Room : public BookingEntity
{
private:
    unsigned int max_guest;

public:
    Room(u_int64_t id, double price, unsigned int max_guest) : BookingEntity(id, price), max_guest(max_guest) {};

    double calculate_cost(int nights) override
    {
        return price * nights;
    }
};
