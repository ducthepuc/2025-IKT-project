<template>
    <div class="product-card" >
        <BImg thumbnail fluid rounded :src="resolvedImagePath" alt="product_image" class="product-image" />
        <h3>{{ item_name }}</h3>
        <p>{{ item_price }}</p>
        <BButton variant="primary" @click="$emit('view-button-clicked', id)">View</BButton>
    </div>
</template>

<script>
import { BImg, BButton } from 'bootstrap-vue-next';

export default {
    components: {
        BImg,
        BButton,
    },
    props: {
        id: Number,
        item_name: String,
        item_price: String,
        image_path: String,
    },
    computed: {
      resolvedImagePath() {
        return new URL(`../assets/${this.image_path}`, import.meta.url).href;
      }
    }
};
</script>

<style scoped>
.product-card {
  border: 1px solid #ddd;
  padding: 1em;
  width: 100%;
  text-align: center;
  border-radius: 8px;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  transition: all 0.1s ease-in-out;
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.product-image {
  width: 100%;
  height: 200px;
  object-fit: cover;
  border-radius: 8px;
}

.product-card h3 {
  font-size: clamp(1rem, 2vw, 1.25rem);
  margin: 0.5rem 0;
}

.product-card p {
  font-size: clamp(0.9rem, 1.5vw, 1rem);
}

@media (hover: hover) {
  .product-card:hover {
    transform: scale(1.05);
  }
}

@media (max-width: 768px) {
  .product-image {
    height: 180px;
  }
}

@media (max-width: 480px) {
  .product-card {
    max-width: 300px;
    margin: 0 auto;
  }
}
</style>