<script setup>
import { computed } from 'vue';

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
    <div class="product-card">
        <img :src="resolvedImagePath" alt="product_image" class="product-image"/>
        <h3>{{item_name}}</h3>
        <p>{{item_price}}</p>
        <button>View</button>
    </div>
</template>

<style scoped>
.product-card {
  border: 1px solid #ddd;
  padding: 1em;
  width: 200px;
  text-align: center;
  border-radius: 8px;
}

.product-image {
  width: 100%;
  height: 150px;
  object-fit: cover;
  border-radius: 8px;
}

h3 {
  font-size: 1.2em;
  margin-top: 10px;
}

p {
  color: #555;
  font-size: 1em;
}

button {
  background-color: #219ebc;
  color: white;
  border: none;
  padding: 10px;
  width: 100%;
  cursor: pointer;
  border-radius: 5px;
}

button:hover {
  background-color: #197a8b;
}
</style>
