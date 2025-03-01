<template>
  <div class="product-detail">
    <BButton variant="outline-primary" class="back-button" @click="goBack">
      <BIcon icon="arrow-left"></BIcon> Back to Shop
    </BButton>
    
    <div class="product-container">
      <div class="product-image">
        <img :src="product.image_path" :alt="product.item_name" />
      </div>
      
      <div class="product-info">
        <h1>{{ product.item_name }}</h1>
        <p class="description">
          Meet {{ product.item_name }}, your potential new feline friend! This adorable cat has a unique personality 
          and is looking for a loving home. Each of our cats comes with a health check, vaccinations, 
          and a starter kit for their new home.
        </p>
        <div class="price-section">
          <h2 class="price">{{ product.item_price }}</h2>
          <BButton variant="success" size="lg">Adopt Now</BButton>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { BButton, BIcon } from 'bootstrap-vue-next'
import doobert from '@/assets/images/doobert.jpeg'
import xiaoje from '@/assets/images/xiaoje.jpeg'
import puff from '@/assets/images/puff.jpeg'
import nala from '@/assets/images/nala.jpeg'
import stryker from '@/assets/images/stryker.jpeg'
import luna from '@/assets/images/luna_cat.jpeg'

export default {
  name: 'ProductView',
  components: {
    BButton,
    BIcon
  },
  props: {
    id: {
      type: String,
      required: true
    }
  },
  data() {
    return {
      product: null
    }
  },
  methods: {
    goBack() {
      this.$router.push('/')
    }
  },
  created() {
    const products = [
        { id: 1, item_name: "Doobert", item_price: "$500", image_path: doobert },
        { id: 2, item_name: "Xiaoje Cat", item_price: "$1000", image_path: xiaoje },
        { id: 3, item_name: "Little Puff", item_price: "$2500", image_path: puff },
        { id: 4, item_name: "Nala Cat", item_price: "$700", image_path: nala },
        { id: 5, item_name: "Stryker", item_price: "$10000", image_path: stryker },
        { id: 6, item_name: "Luna", item_price: "$690", image_path: luna },
    ];
    this.product = products.find(p => p.id === parseInt(this.id));
  }
}
</script>

<style scoped>
.product-detail {
  padding: 2rem;
  max-width: 1200px;
  margin: 0 auto;
  width: 90%;
}

.back-button {
  margin-bottom: 2rem;
}

.product-container {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 3rem;
  align-items: start;
}

.product-image img {
  width: 100%;
  height: auto;
  border-radius: 8px;
  box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
  object-fit: cover;
}

.product-info {
  padding: 1rem;
}

.product-info h1 {
  font-size: clamp(1.5rem, 4vw, 2.5rem);
  margin-bottom: 1.5rem;
  color: #2c3e50;
}

.description {
  font-size: clamp(0.9rem, 2vw, 1.1rem);
  line-height: 1.6;
  color: #666;
  margin-bottom: 2rem;
}

.price-section {
  margin-top: 2rem;
}

.price {
  font-size: clamp(1.5rem, 3vw, 2rem);
  color: #2c3e50;
  margin-bottom: 1rem;
}

/* Tablet */
@media (max-width: 992px) {
  .product-container {
    gap: 2rem;
  }
  
  .product-info {
    padding: 0.5rem;
  }
}

/* Mobile */
@media (max-width: 768px) {
  .product-detail {
    padding: 1rem;
    width: 95%;
  }

  .product-container {
    grid-template-columns: 1fr;
  }

  .product-image {
    max-height: 50vh;
    overflow: hidden;
  }

  .product-image img {
    height: 100%;
  }

  .price-section {
    display: flex;
    flex-direction: column;
    align-items: center;
    gap: 1rem;
  }
}

/* Small Mobile */
@media (max-width: 480px) {
  .product-detail {
    padding: 0.5rem;
  }

  .back-button {
    margin-bottom: 1rem;
  }
}
</style>