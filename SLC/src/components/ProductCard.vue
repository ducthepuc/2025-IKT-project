<script setup>
import { ref, computed } from 'vue';

const props = defineProps({
    item_name: String,
    item_id: String,
    item_price: String,
    image_path: String
});

const resolvedImagePath = computed(() => {
    return new URL(`../assets/${props.image_path}`, import.meta.url).href;
});
</script>

<template>
    <div>
        <img :src="resolvedImagePath" alt="product_image">
        <h3>{{item_name}}</h3>
        <p>{{item_price}}</p>
        <button>View</button>
    </div>
</template>

<style>
div {
    
}
</style>