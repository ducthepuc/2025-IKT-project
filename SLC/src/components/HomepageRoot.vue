<script setup>
import { ref } from 'vue'

defineProps({
  msg: String,
})

const count = ref(0)
</script>

<template>
  <h1>{{ msg }}</h1>

  <div class="card" id="game">
    <button type="button" @click="count++">your jail sentence is: {{ count }} years</button>
    <p>
      We use Vue and Go for this webshop project
    </p>
    <p>Its really cool. Also, use Fleet IDE, cause its beautiful:)</p>
  </div>

  <p id="docs">
    Check out
    <a href="https://github.com/ducthepuc/2025-IKT-project" target="_blank"
      >this project's github</a
    >, to read docs and learn more!
  </p>
</template>