<script setup>
import Card from './components/ProductCard.vue'
import Navbar from './components/Navbar.vue'
</script>

<template>
  <Navbar title1="Shop" destination1="#docs" title2="About us" destination2="#game"/>
  <Card item_name="doobert" item_price="$800" image_path="placeholder_catto.jpeg"></Card>
</template>

<style scoped>
.logo {
  height: 20rem;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}
.logo:hover {
  filter: drop-shadow(0 0 2em #ffffff);
}
</style>
