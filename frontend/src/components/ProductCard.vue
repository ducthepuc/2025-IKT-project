<script setup>
import { defineProps, computed } from 'vue';
import { BImg, BButton } from 'bootstrap-vue-next';

const props = defineProps({
    item_name: String,
    item_price: String,
    image_path: String,
    key: String,
    searchQuery: String
});

const resolvedImagePath = computed(() => {
    return new URL(`../assets/${props.image_path}`, import.meta.url).href;
});
</script>

<template>
    <div class="product-card" >
        <BImg thumbnail fluid rounded :src="resolvedImagePath" alt="product_image" class="product-image" />
        <h3 name="Name">{{ item_name }}</h3>
        <p>{{ item_price }}</p>
        <BButton variant="primary" @click="$emit('view-button-clicked')">View</BButton>
    </div>
</template>

<style scoped>
.product-card {
    border: 1px solid #ddd;
    padding: 1em;
    width: 200px;
    text-align: center;
    border-radius: 8px;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    transition: all 0.1s ease-in-out;
}
.product-image {
    width: 100%;
    height: 150px;
    object-fit: cover;
    border-radius: 8px;
}
.product-card:hover {
    scale: 1.1;
}
</style>