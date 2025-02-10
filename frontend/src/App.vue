<script setup>
import Navbar from './components/Navbar.vue';
import ProductCard from './components/ProductCard.vue';
import AuthModal from './components/AuthModal.vue';
import { ref } from 'vue';

const title1 = "Shop";
const destination1 = "#shop";
const title2 = "About Us";
const destination2 = "#about";

const isLoginModalOpen = ref(false);
const isRegisterModalOpen = ref(false);

const showLoginModal = () => {
    isLoginModalOpen.value = true;
};

const closeLoginModal = () => {
    isLoginModalOpen.value = false;
};

const showRegisterModal = () => {
    isRegisterModalOpen.value = true;
};

const closeRegisterModal = () => {
    isRegisterModalOpen.value = false;
};
</script>

<template>
    <div id="app">
        <!-- Navbar -->
        <Navbar
            :title1="title1"
            :destination1="destination1"
            :title2="title2"
            :destination2="destination2"
            @show-login="showLoginModal"
            @show-register="showRegisterModal"
        />

        <!-- Main Content -->
        <main>
            <section id="shop">
                <h1>Shop Section</h1>
                <div class="product-grid">
                    <ProductCard
                        v-for="i in 3"
                        :key="i"
                        item_name="Doobert"
                        item_price="$100"
                        image_path="placeholder_catto.jpeg"
                    />
                </div>
            </section>
            <section id="about">
                <h1>About Us</h1>
                <p>This is the about us section.</p>
            </section>
        </main>

        <!-- Authentication Modals -->
        <AuthModal
            :isLoginModalOpen="isLoginModalOpen"
            :isRegisterModalOpen="isRegisterModalOpen"
            @close-login="closeLoginModal"
            @close-register="closeRegisterModal"
        />
    </div>
</template>

<style scoped>
.product-grid {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
    gap: 1rem;
    padding: 1rem;
}
</style>