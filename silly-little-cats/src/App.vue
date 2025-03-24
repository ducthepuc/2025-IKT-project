<template>
  <div id="app" :class="{ 'dark-mode': isDark }">
    <nav class="navbar">
      <div class="nav-content">
        <div class="nav-brand">
          <i class="fas fa-cat"></i> SLC
        </div>
        <div class="nav-links">
          <router-link to="/">Home</router-link>
          <router-link to="/about">About</router-link>
          <router-link to="/admin">Admin</router-link>
        </div>
      </div>
    </nav>
    <router-view/>
  </div>
</template>

<script>
import { useThemeStore } from './stores/themeStore'
import { storeToRefs } from 'pinia'

export default {
  name: 'App',
  setup() {
    const themeStore = useThemeStore()
    const { isDark } = storeToRefs(themeStore)
    return { isDark }
  }
}
</script>

<style>
#app {
  font-family: 'Inter', sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  color: var(--text-primary);
  background-color: var(--bg-primary);
  min-height: 100vh;
}

.navbar {
  background-color: var(--card-bg);
  box-shadow: var(--card-shadow);
  padding: 1rem;
}

.nav-content {
  max-width: 1200px;
  margin: 0 auto;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.nav-brand {
  font-size: 1.5rem;
  font-weight: bold;
  color: var(--accent-color);
}

.nav-links {
  display: flex;
  gap: 2rem;
}

.nav-links a {
  text-decoration: none;
  color: var(--text-primary);
  font-weight: 500;
  transition: color 0.2s;
}

.nav-links a:hover {
  color: var(--accent-color);
}

.nav-links a.router-link-active {
  color: var(--accent-color);
}

@media (max-width: 768px) {
  .nav-content {
    padding: 0 1rem;
  }
  
  .nav-brand {
    font-size: 1.2rem;
  }
  
  .nav-links {
    gap: 1rem;
  }
}

@media (max-width: 480px) {
  .nav-content {
    flex-direction: column;
    gap: 1rem;
    padding: 1rem;
  }
}
</style>
