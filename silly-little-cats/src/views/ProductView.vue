<template>
  <div class="product-detail">
    <button class="btn back-button" @click="goBack">
      <i class="fas fa-arrow-left"></i> Back to Celebrities
    </button>
    
    <div class="product-container" v-if="product">
      <div class="product-image-section">
        <img :src="'/images/' + product.image" :alt="product.name" />
        <div class="social-stats">
          <div class="stat">
            <i class="fab fa-instagram"></i>
            <span>{{ formatFollowers(product.instagram) }}</span>
          </div>
          <div class="stat">
            <i class="fab fa-tiktok"></i>
            <span>{{ formatFollowers(product.tiktok) }}</span>
          </div>
          <div class="stat">
            <i class="fab fa-youtube"></i>
            <span>{{ formatFollowers(product.youtube) }}</span>
          </div>
        </div>
      </div>
      
      <div class="product-info">
        <div class="celebrity-tag">
          <i class="fas fa-star"></i> Internet Celebrity Cat
        </div>
        <h1>{{ product.name }}</h1>
        <p class="description">{{ product.description }}</p>
        <div class="features">
          <div class="feature">
            <i class="fas fa-heart"></i>
            <span>Loving Personality</span>
          </div>
          <div class="feature">
            <i class="fas fa-medal"></i>
            <span>Award Winner</span>
          </div>
          <div class="feature">
            <i class="fas fa-check-circle"></i>
            <span>Verified Celebrity</span>
          </div>
        </div>
        <div class="adoption-section">
          <h2 class="price">{{ formatPrice(product.price) }}</h2>
          <button class="btn adopt-button">
            <i class="fas fa-home"></i> Adopt Now
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'ProductView',
  data() {
    return {
      product: null
    }
  },
  methods: {
    goBack() {
      this.$router.push('/')
    },
    formatPrice(price) {
      if (!price) return ''
      return `$${price.toLocaleString()}`
    },
    async fetchProduct() {
      try {
        const res = await fetch(`http://localhost:3000/api/get-products`)
        const data = await res.json()
        const productId = parseInt(this.$route.params.id)
        return data.find(p => p.id === productId)
      } catch (error) {
        console.error('Error fetching product:', error)
        return null
      }
    },
    formatFollowers(count) {
      if (!count) return '0'
      if (count >= 1000000) {
        return (count / 1000000).toFixed(1) + 'M'
      } else if (count >= 1000) {
        return (count / 1000).toFixed(1) + 'K'
      }
      return count.toString()
    }
  },
  async created() {
    try {
      this.product = await this.fetchProduct()
      if (!this.product) {
        console.log('Product not found')
        this.$router.push('/')
      }
    } catch (error) {
      console.error('Error loading product:', error)
      this.$router.push('/')
    }
  }
}
</script>

<style scoped>
.product-detail {
  max-width: 1200px;
  margin: 40px auto;
  padding: 20px;
}

.product-container {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 40px;
  background: var(--card-bg);
  border-radius: 20px;
  padding: 40px;
  box-shadow: var(--card-shadow);
}

.product-image-section {
  aspect-ratio: 4/3;
  overflow: hidden;
  border-radius: 12px;
}

.product-image-section img {
  width: 100%;
  height: 100%;
  object-fit: cover;
  object-position: center;
}

.social-stats {
  display: flex;
  justify-content: space-around;
  margin-top: 20px;
}

.stat {
  text-align: center;
  color: var(--text-secondary);
}

.celebrity-tag {
  display: inline-block;
  background: var(--accent-color);
  color: white;
  padding: 8px 16px;
  border-radius: 20px;
  margin-bottom: 20px;
}

.features {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 20px;
  margin: 30px 0;
}

.feature {
  text-align: center;
  padding: 15px;
  background: var(--bg-secondary);
  border-radius: 12px;
}

.adopt-button {
  background: var(--accent-color);
  color: white;
  padding: 15px 30px;
  border-radius: 30px;
  border: none;
  font-size: 1.2rem;
  width: 100%;
}

@media (max-width: 1024px) {
  .product-container {
    padding: 30px;
    gap: 30px;
  }
  
  .features {
    gap: 15px;
  }
}

@media (max-width: 768px) {
  .product-container {
    grid-template-columns: 1fr;
  }
  
  .product-image-section img {
    max-height: 400px;
    width: 100%;
    object-fit: cover;
  }
  
  .features {
    grid-template-columns: repeat(2, 1fr);
  }
}

@media (max-width: 480px) {
  .product-detail {
    padding: 10px;
  }
  
  .product-container {
    padding: 20px;
  }
  
  .features {
    grid-template-columns: 1fr;
  }
  
  .social-stats {
    flex-direction: column;
    gap: 15px;
  }
  
  .stat {
    font-size: 0.9rem;
  }
  
  h1 {
    font-size: 1.5rem;
  }
}
</style>