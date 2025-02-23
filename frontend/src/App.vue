<script setup>
import ProductCard from './components/ProductCard.vue';
import { computed,ref } from 'vue';
import { BNav, BNavItem, BFormInput, BAlert } from 'bootstrap-vue-next';

const products = ref([
  { item_name: "Doobert", item_price: "$500", image_path: "doobert.jpeg" },
  { item_name: "Xiaoje Cat", item_price: "$1000", image_path: "xiaoje.jpeg" },
  { item_name: "Little Puff", item_price: "$2500", image_path: "puff.jpeg" },
  { item_name: "Nala Cat", item_price: "$700", image_path: "nala.jpeg" },
  { item_name: "Stryker", item_price: "$10000", image_path: "stryker.jpeg" },
]);

const searchQuery = ref("");

function updateSearchList(query) {
  searchQuery.value = query;
}

const filteredProducts = computed(() => {
  return products.value.filter((product) => {
    return product.item_name.toLowerCase().includes(searchQuery.value.toLowerCase());
  });
});

const showAlert = ref(false);

function handleViewButtonClicked() {
    showAlert.value = true;
}

</script>

<template>
    <div id="app">
        <BAlert variant="warning" v-model="showAlert" dismissible>Thanks for visiting SLC! Currently our shop is under construction...</BAlert>
        <BNav pills class="mb-1">
            <BNavItem href="#shop" active>Shop</BNavItem>
            <BNavItem href="#about">About us</BNavItem>
            <BFormInput aria-label="Input" class="mr-1" placeholder="Search for cattos..." v-model="searchQuery" @input="updateSearchList"/>
        </BNav>
        <!-- Main Content -->
        <main>
            <section id="shop">
                <h1>Shop Section</h1>
                <div class="product-grid">
                  <ProductCard
                    v-for="product in filteredProducts"
                    :key="product.item_name"
                    :item_name="product.item_name"
                    :item_price="product.item_price"
                    :image_path="product.image_path"
                    @view-button-clicked="handleViewButtonClicked"
                  />
                </div>
            </section>
            <section id="about">
                <h1>About Us</h1>
                <p>This is the about us section.</p>
            </section>
        </main>
    </div>
</template>

<style scoped>
.product-grid {
    display: grid;
    grid-template-columns: repeat(auto-fill, minmax(200px, 1fr));
    gap: 1rem;
    padding: 1rem;
}
</style>