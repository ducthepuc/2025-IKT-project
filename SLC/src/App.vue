<script setup>
import HomepageRoot from './components/HomepageRoot.vue'
import Navbar from './components/Navbar.vue'
</script>

<template>
  <Navbar title1="Learn more" destination1="#docs" title2="Little game" destination2="#game"/>
  <div>
    <img src="./assets/development.png" class="logo" alt="construction_image">
  </div>
  <HomepageRoot msg="SLC Homepage is currently under development" />
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
