<script setup>
import { ref } from 'vue';
import axios from 'axios';

defineProps({
    title1: String,
    title2: String,
    destination1: String,
    destination2: String,
});

const isLoggedIn = ref(!!localStorage.getItem('authToken'));
const userProfile = ref(null);
const isDropdownOpen = ref(false);

const toggleDropdown = () => {
    isDropdownOpen.value = !isDropdownOpen.value;
};

const fetchUserProfile = async () => {
    try {
        const token = localStorage.getItem('authToken');
        const response = await axios.get('/api/auth/profile', {
            headers: {
                Authorization: token,
            },
        });
        if (response.data) {
            userProfile.value = response.data;
        }
    } catch (error) {
        console.error('Error fetching user profile:', error);
    }
};

if (isLoggedIn.value) {
    fetchUserProfile();
}
</script>

<template>
    <nav class="navbar">
        <ul class="menu">
            <li><a :href="destination1">{{ title1 }}</a></li>
            <li><a :href="destination2">{{ title2 }}</a></li>
        </ul>
        <!-- Account Icon -->
        <div class="account-icon" @click="toggleDropdown">
            <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 24 24" width="24" height="24" fill="white">
                <path d="M12 12c2.21 0 4-1.79 4-4s-1.79-4-4-4-4 1.79-4 4 1.79 4 4 4zm0 2c-2.67 0-8 1.34-8 4v2h16v-2c0-2.66-5.33-4-8-4z"/>
            </svg>
        </div>
        <!-- Dropdown Menu -->
        <div v-if="isDropdownOpen" class="dropdown-menu">
            <div v-if="isLoggedIn">
                <p>Welcome, {{ userProfile?.UserName }}</p>
                <button>My Orders</button>
                <button>Favorites</button>
                <button @click="logout">Logout</button>
            </div>
            <div v-else>
                <button @click="$emit('show-login')">Login</button>
                <button @click="$emit('show-register')">Register</button>
            </div>
        </div>
    </nav>
</template>

<style scoped>
.navbar {
    background-color: #219ebc;
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 0 1rem;
}

.menu {
    list-style-type: none;
    margin: 0;
    padding: 0;
    display: flex;
}

.menu li {
    margin-right: 1rem;
}

.menu li a {
    color: white;
    text-decoration: none;
    font-size: 1rem;
}

.account-icon {
    cursor: pointer;
}

.dropdown-menu {
    position: absolute;
    top: 4rem;
    right: 1rem;
    background-color: white;
    border: 1px solid #ccc;
    padding: 1rem;
    box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
    z-index: 1000;
}

.dropdown-menu button {
    display: block;
    width: 100%;
    margin-bottom: 0.5rem;
    padding: 0.5rem;
    background-color: #219ebc;
    color: white;
    border: none;
    cursor: pointer;
}

.dropdown-menu button:hover {
    background-color: #111111;
}
</style>