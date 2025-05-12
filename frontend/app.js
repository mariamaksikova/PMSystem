// API Configuration
const API_BASE_URL = 'http://localhost:8080/api';

// DOM Elements
const sections = document.querySelectorAll('.section');
const navItems = document.querySelectorAll('.sidebar nav li');
const newReservationBtn = document.getElementById('new-reservation-btn');
const reservationForm = document.getElementById('reservation-form');
const createReservationForm = document.getElementById('create-reservation-form');
const entityList = document.getElementById('entity-list');
const guestList = document.getElementById('guest-list');
const reservationList = document.getElementById('reservation-list');
const totalEntities = document.getElementById('total-entities');
const availableEntities = document.getElementById('available-entities');
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
        entity_id: parseInt(document.getElementById('entity-id').value),
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
        loadEntities();
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
        case 'entities':
            loadEntities();
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
        const [entitiesRes, reservationsRes] = await Promise.all([
            fetch(`${API_BASE_URL}/entities`),
            fetch(`${API_BASE_URL}/reservations`)
        ]);

        if (!entitiesRes.ok || !reservationsRes.ok) {
            throw new Error('Failed to fetch data');
        }

        const entities = await entitiesRes.json();
        const reservations = await reservationsRes.json();

        totalEntities.textContent = entities.entities?.length || 0;
        activeReservations.textContent = reservations.reservations?.length || 0;

        // Calculate available entities
        const availableCount = entities.entities?.filter(entity => entity.status === 'available').length || 0;
        availableEntities.textContent = availableCount;
    } catch (error) {
        console.error('Error loading dashboard:', error);
    }
}

async function loadEntities() {
    try {
        const response = await fetch(`${API_BASE_URL}/entities`);
        if (!response.ok) throw new Error('Failed to load entities');

        const data = await response.json();
        console.log('Entities data:', data); // Проверьте структуру данных в консоли

        entityList.innerHTML = data.entities?.map(entity => `
            <div class="card">
                <h3>Объект #${entity.id}</h3>
                <p>Цена: ${entity.price} руб за ночь</p>
                <p>Статус: ${entity.status === 'available' ? '✅ Доступен' : '❌ Занят'}</p>
            </div>
        `).join('') || '<p>Объекты не найдены</p>';
    } catch (error) {
        console.error('Error loading entities:', error);
        entityList.innerHTML = '<p>Ошибка загрузки объектов. Пожалуйста, попробуйте снова.</p>';
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
                <p>Телефон: ${guest.phone}</p>
                <p>Бонусные баллы: ${guest.loyalty_points || 0}</p>
            </div>
        `).join('') || '<p>Гости не найдены</p>';
    } catch (error) {
        console.error('Error loading guests:', error);
        guestList.innerHTML = '<p>Ошибка загрузки гостей. Пожалуйста, попробуйте снова.</p>';
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
                <h3>Бронирование #${res.id}</h3>
                <p><strong>Объект:</strong> #${res.entity_id}</p>
                <p><strong>Гость:</strong> #${res.guest_id}</p>
                <p><strong>Даты:</strong> ${res.checkin} до ${res.checkout}</p>
                <p><strong>Статус:</strong> ${res.status}</p>
                <button class="btn-danger" onclick="cancelReservation(${res.id})">
                    <i class="fas fa-trash"></i> Отменить
                </button>
            </div>
        `).join('') || '<p>Бронирования не найдены</p>';
    } catch (error) {
        console.error('Error loading reservations:', error);
        reservationList.innerHTML = '<p>Ошибка загрузки бронирований. Пожалуйста, попробуйте снова.</p>';
    }
}

// Cancel reservation function
window.cancelReservation = async function (reservationId) {
    if (!confirm('Вы уверены, что хотите отменить это бронирование?')) return;

    try {
        const response = await fetch(`${API_BASE_URL}/reservations/${reservationId}`, {
            method: 'DELETE'
        });

        if (!response.ok) {
            throw new Error('Не удалось отменить бронирование');
        }

        alert('Бронирование успешно отменено!');
        loadReservations();
        loadEntities();
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