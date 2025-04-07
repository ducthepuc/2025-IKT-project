<template>
  <div id="app" :class="{ 'dark-mode': isDark }">
    <nav class="navbar">
      <div class="nav-content">
        <router-link to="/" class="nav-brand">
          <i class="fas fa-cat"></i>
          <span>Silly Little Cats</span>
        </router-link>
        <div class="nav-links">
          <router-link to="/">
            <i class="fas fa-home"></i> Home
          </router-link>
          <router-link to="/about">
            <i class="fas fa-info-circle"></i> About
          </router-link>
          <router-link to="/admin">
            <i class="fas fa-user-shield"></i> Admin
          </router-link>
          <router-link to="/login">
            <i class="fas fa-user-shield"></i> Login
          </router-link>
          <router-link to="/register">
            <i class="fas fa-user-shield"></i> Register
          </router-link>
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
  position: sticky;
  top: 0;
  z-index: 1000;
}

.nav-content {
  max-width: 1200px;
  margin: 0 auto;
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 0 1rem;
}

.nav-brand {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  text-decoration: none;
  font-size: 1.5rem;
  font-weight: bold;
  color: var(--accent-color);
}

.nav-brand i {
  font-size: 1.8rem;
}

.nav-links {
  display: flex;
  gap: 2rem;
}

.nav-links a {
  display: flex;
  align-items: center;
  gap: 0.5rem;
  text-decoration: none;
  color: var(--text-primary);
  font-weight: 500;
  padding: 0.5rem 1rem;
  border-radius: 8px;
  transition: all 0.2s;
}

.nav-links a:hover {
  background-color: var(--bg-secondary);
  color: var(--accent-color);
}

.nav-links a.router-link-active {
  background-color: var(--accent-color);
  color: white;
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
