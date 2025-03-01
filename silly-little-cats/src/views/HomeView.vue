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
                    :key="product.item_name"
                    :item_name="product.item_name"
                    :item_price="product.item_price"
                    :image_path="product.image_path"
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
import doobert from '@/assets/images/doobert.jpeg';
import xiaoje from '@/assets/images/xiaoje.jpeg';
import puff from '@/assets/images/puff.jpeg';
import nala from '@/assets/images/nala.jpeg';
import stryker from '@/assets/images/stryker.jpeg';
import luna from '@/assets/images/luna_cat.jpeg';

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
      products: [
        { id: 1, item_name: "Doobert", item_price: "$500", image_path: doobert },
        { id: 2, item_name: "Xiaoje Cat", item_price: "$1000", image_path: xiaoje },
        { id: 3, item_name: "Little Puff", item_price: "$2500", image_path: puff },
        { id: 4, item_name: "Nala Cat", item_price: "$700", image_path: nala },
        { id: 5, item_name: "Stryker", item_price: "$10000", image_path: stryker },
        { id: 6, item_name: "Luna", item_price: "$690", image_path: luna },
      ],
      searchQuery: String(""),
    }
  },
  methods: {
    updateSearchList(query) {
      this.searchQuery = String(query);
    },
    handleViewButtonClicked(id) {
      this.router.push(`/product/${id}`);
    },
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
