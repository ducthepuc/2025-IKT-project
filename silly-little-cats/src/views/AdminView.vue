<template>
  <div class="admin-panel">
    <header class="header">
      <h1>Admin Dashboard</h1>
      <div class="header-controls">
        <button class="theme-toggle" @click="toggleTheme">
          <i :class="isDark ? 'fas fa-sun' : 'fas fa-moon'"></i>
        </button>
      </div>
    </header>

    <div class="admin-sections">
      <!-- Upload Section -->
      <section class="upload-section">
        <div class="section-header">
          <h2><i class="fas fa-upload"></i> Upload New Celebrity Cat</h2>
        </div>
        <form @submit.prevent="handleSubmit" enctype="multipart/form-data">
          <div class="form-grid">
            <div class="form-group">
              <label>Name</label>
              <input v-model="form.name" type="text" placeholder="Cat's name" required>
            </div>
            <div class="form-group">
              <label>Price</label>
              <input v-model="form.price" type="number" placeholder="Adoption price" required>
            </div>
          </div>

          <div class="form-group">
            <label>Description</label>
            <textarea v-model="form.description" placeholder="Tell us about this celebrity cat" required></textarea>
          </div>

          <div class="drop-zone" 
               @click="triggerFileInput"
               @dragover.prevent="handleDragOver"
               @dragleave="handleDragLeave"
               @drop.prevent="handleDrop"
               ref="dropZone">
            <i class="fas fa-cloud-upload-alt"></i>
            <p>{{ dropZoneText }}</p>
            <input type="file" ref="fileInput" accept="image/*" @change="handleFileChange" hidden>
          </div>

          <div class="social-stats-input">
            <h3><i class="fas fa-hashtag"></i> Social Media Stats</h3>
            <div class="stats-grid">
              <div class="form-group">
                <label><i class="fab fa-instagram"></i> Instagram</label>
                <input v-model="form.instagram" type="number" placeholder="Followers">
              </div>
              <div class="form-group">
                <label><i class="fab fa-tiktok"></i> TikTok</label>
                <input v-model="form.tiktok" type="number" placeholder="Followers">
              </div>
              <div class="form-group">
                <label><i class="fab fa-youtube"></i> YouTube</label>
                <input v-model="form.youtube" type="number" placeholder="Subscribers">
              </div>
            </div>
          </div>

          <button type="submit" class="submit-btn">
            <i class="fas fa-plus-circle"></i> Add Celebrity Cat
          </button>
        </form>
      </section>

      <!-- Products Section -->
      <section class="products-section">
        <div class="section-header">
          <h2><i class="fas fa-list"></i> Manage Celebrity Cats</h2>
        </div>
        <div v-if="loading" class="loading">
          <i class="fas fa-spinner fa-spin"></i> Loading...
        </div>
        <div v-else-if="error" class="error">
          <i class="fas fa-exclamation-circle"></i> {{ error }}
        </div>
        <div v-else-if="!products.length" class="no-products">
          <i class="fas fa-inbox"></i>
          <p>No celebrity cats available yet.</p>
        </div>
        <div v-else class="products-grid">
          <div v-for="product in products" :key="product.id" class="product-card">
            <img :src="`http://localhost:3000${product.image}`" :alt="product.name">
            <div class="product-content">
              <h3>{{ product.name }}</h3>
              <p class="price">${{ product.price }}</p>
              <p class="description">{{ product.description }}</p>
              <div class="social-stats">
                <span><i class="fab fa-instagram"></i> {{ formatNumber(product.instagram) }}</span>
                <span><i class="fab fa-tiktok"></i> {{ formatNumber(product.tiktok) }}</span>
                <span><i class="fab fa-youtube"></i> {{ formatNumber(product.youtube) }}</span>
              </div>
              <button @click="deleteProduct(product.id)">
                <i class="fas fa-trash"></i> Delete
              </button>
            </div>
          </div>
        </div>
        <button v-if="products.length" class="delete-all-btn" @click="deleteAllProducts">
          <i class="fas fa-trash-alt"></i> Delete All
        </button>
      </section>
    </div>
  </div>
</template>

<script>
import { useThemeStore } from '../stores/themeStore'
import { storeToRefs } from 'pinia'

export default {
  name: 'AdminView',
  setup() {
    const themeStore = useThemeStore()
    const { isDark } = storeToRefs(themeStore)
    const { toggleTheme } = themeStore
    return { isDark, toggleTheme }
  },
  data() {
    return {
      form: {
        name: '',
        price: '',
        description: '',
        instagram: '',
        tiktok: '',
        youtube: ''
      },
      selectedFile: null,
      dropZoneText: 'Drop an image here or click to upload',
      products: [],
      loading: false,
      error: null
    }
  },
  methods: {
    async deleteProduct(productId) {
      try {
        const response = await fetch(`http://localhost:3000/api/delete-product?id=${productId}`, {
          method: 'DELETE',
        });
        console.log(response);
        if (response.ok) {
          const result = await response.json();
          if (result.success) {
            alert('Product deleted successfully');
            this.products = this.products.filter(product => product.id !== productId);
          } else {
            alert('Failed to delete product: ' + result.message);
          }
        } else {
          throw new Error('Network error or invalid response from server');
        }
      } catch (error) {
        console.error('Error deleting product:', error);
        alert('Error deleting product. Please try again.');
      }
    },
    async deleteAllProducts() {
      try {
        const response = await fetch('http://localhost:3000/api/delete-all-products', {
          method: 'DELETE',
        });

        const result = await response.json();
        if (result.success) {
          alert('All products deleted successfully');
          this.loadProducts();
        } else {
          alert('Failed to delete all products: ' + result.message);
        }
      } catch (error) {
        console.error('Error deleting all products:', error);
        alert('Error deleting all products. Please try again.');
      }
    },
    triggerFileInput() {
      this.$refs.fileInput.click()
    },
    handleDragOver() {
      this.$refs.dropZone.style.borderColor = '#007bff'
    },
    handleDragLeave() {
      this.$refs.dropZone.style.borderColor = '#ccc'
    },
    handleDrop(e) {
      const file = e.dataTransfer.files[0]
      this.handleFile(file)
    },
    handleFileChange(e) {
      const file = e.target.files[0]
      this.handleFile(file)
    },
    handleFile(file) {
      this.selectedFile = file
      this.dropZoneText = file.name
    },
    async handleSubmit() {
      try {
        const formData = new FormData()
        Object.entries(this.form).forEach(([key, value]) => {
          formData.append(key, value || '0')
        })
        if (this.selectedFile) {
          formData.append('image', this.selectedFile)
        }

        const response = await fetch('http://localhost:3000/api/upload', {
          method: 'POST',
          body: formData
        })
        
        const data = await response.json()
        
        if (data.success) {
          console.log('Upload successful!') 
          this.resetForm()
          this.loadProducts()
        } else {
          console.log('Upload failed: ' + data.message) 
        }
      } catch (err) {
        console.error('Error:', err)
      }
    },
    resetForm() {
      this.form = {
        name: '',
        price: '',
        description: '',
        instagram: '',
        tiktok: '',
        youtube: ''
      }
      this.selectedFile = null
      this.dropZoneText = 'Drop an image here or click to upload'
    },
    async loadProducts() {
      this.loading = true
      this.error = null
      try {
        const res = await fetch('http://localhost:3000/api/get-products')
        if (!res.ok) throw new Error(`HTTP error! status: ${res.status}`)
        this.products = await res.json()
      } catch (err) {
        console.error('Error loading products:', err)
        this.error = 'Error loading products. Please try again later.'
      } finally {
        this.loading = false
      }
    },
    formatNumber(num) {
      if (!num) return '0'
      if (num >= 1000000) return (num / 1000000).toFixed(1) + 'M'
      if (num >= 1000) return (num / 1000).toFixed(1) + 'K'
      return num.toString()
    }
  },
  created() {
    this.loadProducts()
  }
}
</script>

<style scoped>
.admin-panel {
  width: 90%;
  max-width: 1200px;
  margin: 0 auto;
  padding: 2rem 1rem;
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 2rem;
}

.admin-sections {
  display: grid;
  gap: 2rem;
}

.section-header {
  margin-bottom: 1.5rem;
}

.section-header h2 {
  color: var(--accent-color);
  font-size: 1.5rem;
}

.upload-section, .products-section {
  background: var(--card-bg);
  border-radius: 12px;
  padding: 2rem;
  box-shadow: var(--card-shadow);
}

.form-grid {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 1rem;
  margin-bottom: 1rem;
}

.form-group {
  display: flex;
  flex-direction: column;
  gap: 0.5rem;
}

.form-group label {
  color: var(--text-secondary);
  font-size: 0.9rem;
}

input, textarea {
  padding: 0.75rem;
  border: 1px solid var(--border-color);
  border-radius: 8px;
  background: var(--bg-secondary);
  color: var(--text-primary);
}

textarea {
  min-height: 100px;
  resize: vertical;
}

.drop-zone {
  border: 2px dashed var(--border-color);
  border-radius: 8px;
  padding: 2rem;
  text-align: center;
  cursor: pointer;
  transition: all 0.2s;
}

.drop-zone:hover {
  border-color: var(--accent-color);
  background: var(--bg-secondary);
}

.stats-grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  gap: 1rem;
  margin: 1rem 0;
}

.submit-btn {
  background: var(--accent-color);
  color: black;
  padding: 1rem;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  transition: opacity 0.2s;
  font-weight: bold;
  margin-top: 1rem;
}

.products-grid {
  display: grid;
  grid-template-columns: repeat(auto-fill, minmax(280px, 1fr));
  gap: 1.5rem;
}

.product-card {
  background: var(--bg-secondary);
  border-radius: 8px;
  overflow: hidden;
  transition: transform 0.2s;
}

.product-card:hover {
  transform: translateY(-5px);
}

.product-card img {
  width: 100%;
  height: 200px;
  object-fit: cover;
}

.product-content {
  padding: 1rem;
}

.social-stats {
  display: flex;
  gap: 1rem;
  margin-top: 0.5rem;
  color: var(--text-secondary);
  font-size: 0.9rem;
}

.loading, .error, .no-products {
  text-align: center;
  padding: 2rem;
  color: var(--text-secondary);
}

@media (max-width: 768px) {
  .admin-panel {
    width: 95%;
    padding: 1rem;
  }

  .form-grid, .stats-grid {
    grid-template-columns: 1fr;
  }

  .header {
    flex-direction: column;
    gap: 1rem;
    text-align: center;
  }

  .upload-section, .products-section {
    padding: 1rem;
  }
}

@media (max-width: 480px) {
  .admin-panel {
    width: 100%;
  }

  .products-grid {
    grid-template-columns: 1fr;
  }

  .section-header h2 {
    font-size: 1.2rem;
  }
}
</style>