<script setup>
import HelloWorld from './components/HelloWorld.vue'
</script>

<template>
  <div>
    <img src="./assets/placeholder_catto.jpeg" class="logo" alt="goobert_image">
  </div>
  <HelloWorld msg="SLC = silly little cats" />
</template>

<style scoped>
.logo {
  height: 20rem;
  padding: 1.5em;
  will-change: filter;
  transition: filter 300ms;
}
.logo:hover {
  filter: drop-shadow(0 0 2em #888888);
}
</style>
