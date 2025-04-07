<template>
  <div class="product-card">
    <div class="image-container">
      <img :src="image_path" :alt="item_name" class="product-image" />
      <div class="celebrity-badge">
        <i class="fas fa-star"></i> Internet Celebrity
      </div>
    </div>
    <div class="card-content">
      <h3>{{ item_name }}</h3>
      <p class="price">{{ item_price }}</p>
      <p class="followers">
        <i class="fas fa-users"></i> {{ formatFollowers(totalFollowers) }}
      </p>
      <button class="view-button" @click="$emit('view-button-clicked', id)">
        <i class="fas fa-paw"></i> Meet Me
      </button>
    </div>
  </div>
</template>

<script>
export default {
  props: {
    id: String,
    item_name: String,
    item_price: String,
    image_path: String,
    followers: Number,
    tiktok: {
      type: Number,
      default: 0
    },
    youtube: {
      type: Number,
      default: 0
    }
  },
  computed: {
    totalFollowers() {
      return (this.followers || 0) + (this.tiktok || 0) + (this.youtube || 0)
    }
  },
  methods: {
    formatFollowers(count) {
      if (!count) return '0'
      if (count >= 1000000) {
        return (count / 1000000).toFixed(1) + 'M'
      } else if (count >= 1000) {
        return (count / 1000).toFixed(1) + 'K'
      }
      return count.toString()
    }
  }
}
</script>

<style scoped>
.product-card {
  background-color: var(--card-bg);
  border-radius: 16px;
  overflow: hidden;
  box-shadow: var(--card-shadow);
  transition: transform 0.2s, box-shadow 0.2s;
}

.image-container {
  position: relative;
  overflow: hidden;
}

.product-image {
  height: 280px;
  object-fit: cover;
  transition: transform 0.3s;
}

.celebrity-badge {
  position: absolute;
  top: 12px;
  right: 12px;
  background: var(--accent-color);
  color: white;
  padding: 8px 12px;
  border-radius: 20px;
  font-size: 0.9rem;
}

.card-content {
  padding: 1.5rem;
}

.price {
  color: var(--accent-color);
  font-weight: bold;
  font-size: 1.2rem;
}

.followers {
  color: var(--text-secondary);
  font-size: 0.9rem;
  margin: 8px 0;
}

.view-button {
  width: 100%;
  border-radius: 25px;
  background: var(--accent-color);
  border: none;
  padding: 12px;
}

@media (hover: hover) {
  .product-card:hover {
    transform: translateY(-5px);
    box-shadow: var(--card-shadow), 0 10px 20px rgba(0, 0, 0, 0.1);
  }
  
  .product-card:hover .product-image {
    transform: scale(1.05);
  }
}

@media (max-width: 768px) {
  .product-card {
    max-width: 100%;
  }
  
  .product-image {
    height: 220px;
  }
  
  .card-content {
    padding: 1rem;
  }
  
  .price {
    font-size: 1rem;
  }
}

@media (max-width: 480px) {
  .product-image {
    height: 200px;
  }
  
  .celebrity-badge {
    padding: 6px 10px;
    font-size: 0.8rem;
  }
  
  .card-content h3 {
    font-size: 1.1rem;
  }
  
  .view-button {
    padding: 10px;
    font-size: 0.9rem;
  }
}

/* Disable hover effects on touch devices */
@media (hover: none) {
  .product-card:hover {
    transform: none;
    box-shadow: var(--card-shadow);
  }
  
  .product-card:hover .product-image {
    transform: none;
  }
}
</style>