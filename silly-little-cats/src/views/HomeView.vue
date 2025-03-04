<template>
  <div class="home">
        <BFormInput aria-label="Input" class="mr-1" placeholder="Search for cattos..." v-model="searchQuery"/>
        <main>
            <section id="shop">
                <h1>Shop Section</h1>
                <div class="product-grid">
                  <ProductCard
                    :id="product.id"
                    v-for="product in filteredProducts"
                    :key="product.name"
                    :item_name="product.name"
                    :item_price="product.price"
                    :image_path="product.image"
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

export default {
  name: 'HomeView',
  components: {
    ProductCard,
    BFormInput,
  },
  setup() {
    const router = useRouter();
    return { router };
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
    async fetchProducts() {
      const res = await fetch(`http://localhost:3000/api/get-products`)

      const data = await res.json();

      return data;
    }
  },
  computed: {
    filteredProducts() {
      if (!this.searchQuery) return this.products;
      return this.products.filter((product) => {
        return product.item_name.toLowerCase().includes(this.searchQuery.toLowerCase());
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
</style>
