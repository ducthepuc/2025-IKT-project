<template>
  <div class="home">
    <header class="header">
      <h1>Celebrity Cats</h1>
      <div class="header-controls">
        <BFormInput
          aria-label="Search"
          class="search-input"
          placeholder="Search for celebrity cats..."
          v-model="searchQuery"
        />
        <button class="theme-toggle" @click="toggleTheme">
          <i :class="isDark ? 'fas fa-sun' : 'fas fa-moon'"></i>
        </button>
      </div>
    </header>
    <main>
      <section id="shop">
        <h1>Shop Section</h1>
        <div class="product-grid">
          <ProductCard
            :id=parseInt(product.id)
            v-for="product in filteredProducts"
            :key="product.name"
            :item_name="product.name"
            :item_price="formatPrice(product.price)"
            :image_path="'/images/'+product.image"
            :followers="product.instagram"
            :tiktok="product.tiktok"
            :youtube="product.youtube"
            @view-button-clicked="handleViewButtonClicked"
          />
        </div>
      </section>
    </main>
    <footer>
      <p>&copy; 2025 SLC. All rights reserved.</p>
    </footer>
  </div>
</template>

<script>
import ProductCard from '../components/ProductCard.vue';
import { BFormInput } from 'bootstrap-vue-next';
import { useRouter } from 'vue-router';
import { useThemeStore } from '../stores/themeStore'
import { storeToRefs } from 'pinia'

export default {
  name: 'HomeView',
  components: {
    ProductCard,
    BFormInput,
  },
  setup() {
    const router = useRouter();
    const themeStore = useThemeStore()
    const { isDark } = storeToRefs(themeStore)
    const { toggleTheme } = themeStore
    
    return { router, isDark, toggleTheme };
  },
  data() {
    return {
      products: [],
      searchQuery: String(""),
    }
  },
  async created() {
    this.products = await this.fetchProducts()
  },
  methods: {
    updateSearchList(query) {
      this.searchQuery = String(query);
    },
    handleViewButtonClicked(id) {
      this.router.push(`/product/${id}`);
    },
    formatPrice(price) {
      if (!price) return ''
      return `$${price.toLocaleString()}`
    },
    async fetchProducts() {
      const res = await fetch(`http://localhost:3000/api/get-products`)

      const data = await res.json();
      console.log(data)
      return data;
    }
  },
  computed: {
    filteredProducts() {
      if (!this.searchQuery) return this.products;
      return this.products.filter((product) => {
        return product.name.toLowerCase().includes(this.searchQuery.toLowerCase());
      });
    },
  },
}
</script>

<style scoped>
.home {
  width: 90%;
  margin: 0 auto;
  max-width: 1200px;
  padding: 1rem;
}

.product-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(250px, 1fr));
  gap: 2rem;
  padding: 1rem;
}

@media (max-width: 768px) {
  .home {
    width: 95%;
    padding: 0.5rem;
  }

  .product-grid {
    grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
    gap: 1rem;
    padding: 0.5rem;
  }
}

@media (max-width: 480px) {
  .product-grid {
    grid-template-columns: 1fr;
  }
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 40px;
}

.header-controls {
  display: flex;
  gap: 20px;
  align-items: center;
}

.theme-toggle {
  background: var(--accent-color);
  border: none;
  color: white;
  width: 40px;
  height: 40px;
  border-radius: 50%;
  cursor: pointer;
  transition: transform 0.2s;
}

.theme-toggle:hover {
  transform: rotate(15deg);
}

@media (max-width: 768px) {
  .header {
    flex-direction: column;
    gap: 20px;
    text-align: center;
  }
  
  .header h1 {
    font-size: 1.8rem;
  }
  
  .header-controls {
    width: 100%;
  }
  
  .search-input {
    flex: 1;
  }
}

@media (max-width: 480px) {
  .home {
    width: 100%;
    padding: 0.5rem;
  }
  
  .header h1 {
    font-size: 1.5rem;
  }
  
  .header-controls {
    flex-direction: column;
    gap: 10px;
  }
  
  .theme-toggle {
    width: 100%;
    border-radius: 12px;
  }
}
</style>