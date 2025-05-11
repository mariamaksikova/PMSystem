// API Configuration
const API_BASE_URL = 'http://localhost:8080/api';

// DOM Elements
const sections = document.querySelectorAll('.section');
const navItems = document.querySelectorAll('.sidebar nav li');
const newReservationBtn = document.getElementById('new-reservation-btn');
const reservationForm = document.getElementById('reservation-form');
const createReservationForm = document.getElementById('create-reservation-form');
const roomList = document.getElementById('room-list');
const guestList = document.getElementById('guest-list');
const reservationList = document.getElementById('reservation-list');
const totalRooms = document.getElementById('total-rooms');
const availableRooms = document.getElementById('available-rooms');
const activeReservations = document.getElementById('active-reservations');

// Navigation
navItems.forEach(item => {
    item.addEventListener('click', () => {
        navItems.forEach(i => i.classList.remove('active'));
        item.classList.add('active');

        const sectionId = item.getAttribute('data-section');
        sections.forEach(section => {
            section.classList.remove('active');
            if (section.id === sectionId) {
                section.classList.add('active');
                loadSection(sectionId);
            }
        });
    });
});

// Toggle reservation form
newReservationBtn.addEventListener('click', () => {
    reservationForm.style.display = reservationForm.style.display === 'none' ? 'block' : 'none';
});

// Form submission
createReservationForm.addEventListener('submit', async (e) => {
    e.preventDefault();

    const reservationData = {
        guest_id: parseInt(document.getElementById('guest-id').value),
        room_id: parseInt(document.getElementById('room-id').value),
        checkin: document.getElementById('checkin').value,
        checkout: document.getElementById('checkout').value
    };

    try {
        const response = await fetch(`${API_BASE_URL}/reservations`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(reservationData)
        });

        if (!response.ok) {
            throw new Error(await response.text());
        }

        alert('Reservation created successfully!');
        createReservationForm.reset();
        reservationForm.style.display = 'none';
        loadReservations();
        loadRooms();
        loadDashboard();
    } catch (error) {
        console.error('Error:', error);
        alert(error.message);
    }
});

// Load section content
function loadSection(sectionId) {
    switch (sectionId) {
        case 'dashboard':
            loadDashboard();
            break;
        case 'rooms':
            loadRooms();
            break;
        case 'reservations':
            loadReservations();
            break;
        case 'guests':
            loadGuests();
            break;
    }
}

// API Functions
async function loadDashboard() {
    try {
        const [roomsRes, reservationsRes] = await Promise.all([
            fetch(`${API_BASE_URL}/rooms`),
            fetch(`${API_BASE_URL}/reservations`)
        ]);

        if (!roomsRes.ok || !reservationsRes.ok) {
            throw new Error('Failed to fetch data');
        }

        const rooms = await roomsRes.json();
        const reservations = await reservationsRes.json();

        totalRooms.textContent = rooms.rooms?.length || 0;
        activeReservations.textContent = reservations.reservations?.length || 0;

        // Calculate available rooms
        const availableCount = rooms.rooms?.filter(room => room.status === 'available').length || 0;
        availableRooms.textContent = availableCount;
    } catch (error) {
        console.error('Error loading dashboard:', error);
    }
}

async function loadRooms() {
    try {
        const response = await fetch(`${API_BASE_URL}/rooms`);
        if (!response.ok) throw new Error('Failed to load rooms');

        const data = await response.json();
        console.log('Rooms data:', data);

        roomList.innerHTML = data.rooms?.map(room => `
            <div class="card">
                <h3>Room #${room.id}</h3>
                <p>Price: $${room.price} per night</p>
                <p>Max guests: ${room.max_guests}</p>
                <p>Status: ${room.status === 'available' ? '✅ Available' : '❌ Occupied'}</p>
            </div>
        `).join('') || '<p>No rooms found</p>';
    } catch (error) {
        console.error('Error loading rooms:', error);
        roomList.innerHTML = '<p>Error loading rooms. Please try again.</p>';
    }
}

async function loadGuests() {
    try {
        const response = await fetch(`${API_BASE_URL}/guests`);
        if (!response.ok) throw new Error('Failed to load guests');

        const data = await response.json();
        console.log('Guests data:', data);

        guestList.innerHTML = data.guests?.map(guest => `
            <div class="card">
                <h3>${guest.first_name} ${guest.last_name}</h3>
                <p>ID: ${guest.id}</p>
                <p>Email: ${guest.email}</p>
                <p>Phone: ${guest.phone}</p>
                <p>Loyalty Points: ${guest.loyalty_points || 0}</p>
            </div>
        `).join('') || '<p>No guests found</p>';
    } catch (error) {
        console.error('Error loading guests:', error);
        guestList.innerHTML = '<p>Error loading guests. Please try again.</p>';
    }
}

async function loadReservations() {
    try {
        const response = await fetch(`${API_BASE_URL}/reservations`);
        if (!response.ok) throw new Error('Failed to load reservations');

        const data = await response.json();
        console.log('Reservations data:', data);

        reservationList.innerHTML = data.reservations?.map(res => `
            <div class="card">
                <h3>Reservation #${res.id}</h3>
                <p><strong>Room:</strong> #${res.room_id}</p>
                <p><strong>Guest:</strong> #${res.guest_id}</p>
                <p><strong>Dates:</strong> ${res.checkin} to ${res.checkout}</p>
                <p><strong>Status:</strong> ${res.status}</p>
                <button class="btn-danger" onclick="cancelReservation(${res.id})">
                    <i class="fas fa-trash"></i> Cancel
                </button>
            </div>
        `).join('') || '<p>No reservations found</p>';
    } catch (error) {
        console.error('Error loading reservations:', error);
        reservationList.innerHTML = '<p>Error loading reservations. Please try again.</p>';
    }
}

// Cancel reservation function
window.cancelReservation = async function (reservationId) {
    if (!confirm('Are you sure you want to cancel this reservation?')) return;

    try {
        const response = await fetch(`${API_BASE_URL}/reservations/${reservationId}`, {
            method: 'DELETE'
        });

        if (!response.ok) {
            throw new Error('Failed to cancel reservation');
        }

        alert('Reservation cancelled successfully!');
        loadReservations();
        loadRooms();
        loadDashboard();
    } catch (error) {
        console.error('Error cancelling reservation:', error);
        alert(error.message);
    }
}

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    loadDashboard();

    // Load current active section
    const activeSection = document.querySelector('.sidebar nav li.active');
    if (activeSection) {
        loadSection(activeSection.getAttribute('data-section'));
    }
});