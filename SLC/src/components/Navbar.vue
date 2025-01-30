<script setup>
defineProps({
    title1: String,
    title2: String,
    destination1: String,
    destination2: String
})
</script>

<template>
    <nav class="navbar">
        <ul class="menu">
            <li><a :href="destination1">{{ title1 }}</a></li>
            <li><a :href="destination2">{{ title2 }}</a></li>
        </ul>
    </nav>
</template>

<style scoped>
.navbar {
  background-color: #333333;
}

.menu {
  list-style-type: none;
  margin: 0;
  padding: 0;
  overflow: hidden;
}

.menu li {
  float: left;
}

.menu li a {
  display: block;
  color: white;
  text-align: center;
  padding: 16px;
  text-decoration: none;
}

.menu li a:hover {
  background-color: #111111;
}
</style>