<script setup>
import { ref } from 'vue'

defineProps({
  msg: String,
})

const count = ref(0)
</script>

<template>
  <h1>{{ msg }}</h1>

  <div class="card">
    <button type="button" @click="count++">your loaf rank is: {{ count }}</button>
    <p>
      Edit
      <code>components/HomepageRoot.vue</code> to test HMR
    </p>
    <p>Its really cool. Also, use Fleet IDE, cause its beautiful:)</p>
  </div>

  <p>
    Check out
    <a href="https://github.com/ducthepuc/2025-IKT-project" target="_blank"
      >this project's github</a
    >, to read docs and learn more!
  </p>
</template>