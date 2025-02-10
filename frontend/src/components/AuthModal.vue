<template>
  <div>
    <!-- Buttons to Open Modals -->
    <button @click="openLoginModal">Login</button>
    <button @click="openRegisterModal">Register</button>

    <!-- Login Modal -->
    <div v-if="isLoginModalOpen" class="modal">
      <div class="modal-content">
        <h2>Login</h2>
        <form @submit.prevent="login">
          <input v-model="username" type="text" placeholder="Username" required />
          <br />
          <input v-model="password" type="password" placeholder="Password" required />
          <br />
          <button type="submit">Login</button>
        </form>
        <p v-if="errorMessage" class="error">{{ errorMessage }}</p>
        <button @click="emit('close-login')">Close</button>
      </div>
    </div>

    <!-- Register Modal -->
    <div v-if="isRegisterModalOpen" class="modal">
      <div class="modal-content">
        <h2>Register</h2>
        <form @submit.prevent="register">
          <input v-model="newUser.username" type="text" placeholder="Username" required />
          <br />
          <input v-model="newUser.password" type="password" placeholder="Password" required />
          <br />
          <input v-model="newUser.email" type="email" placeholder="Email" required />
          <br />
          <button type="submit">Register</button>
        </form>
        <p v-if="errorMessage" class="error">{{ errorMessage }}</p>
        <button @click="emit('close-register')">Close</button>
      </div>
    </div>
  </div>
</template>

<script>
import axios from "axios";

const props = defineProps({
    isLoginModalOpen: Boolean,
    isRegisterModalOpen: Boolean,
});
const emit = defineEmits(['close-login', 'close-register']);
export default {
  data() {
    return {
      username: "",
      password: "",
      errorMessage: "",
      newUser: {
        username: "",
        password: "",
        email: "",
      },
    };
  },
  methods: {

    async login() {
      try {
        const response = await axios.post("http://localhost:5001/api/auth/login", {
          UserName: this.username,
          Password: this.password,
        });
        localStorage.setItem("authToken", response.data.token);
        emit('close-login');
      } catch (error) {
        if (error.response && error.response.data) {
          this.errorMessage = error.response.data.message;
        } else {
          this.errorMessage = "An unexpected error occurred during login.";
        }
      }
    },

    async register() {
      try {
        const response = await axios.post("http://localhost:5001/api/auth/register", {
          UserName: this.newUser.username,
          Password: this.newUser.password,
          Email: this.newUser.email,
        });
        console.log(response.data);
        emit('close-register');
      } catch (error) {
        if (error.response && error.response.data) {
          this.errorMessage = error.response.data.message;
        } else {
          this.errorMessage = "An unexpected error occurred during registration.";
        }
      }
    },

    async logout() {
      try {
        const token = localStorage.getItem("authToken");
        if (!token) {
          console.log("No token found, user is already logged out.");
          return;
        }
        await axios.post(
          "http://localhost:5001/api/auth/logout",
          {},
          {
            headers: {
              Authorization: token,
            },
          }
        );
        localStorage.removeItem("authToken");
        console.log("Logged out successfully");
      } catch (error) {
        console.error("Logout failed:", error);
      }
    },
  },
};
</script>

<style scoped>
.modal {
  position: fixed;
  top: 0;
  left: 0;
  width: 100%;
  height: 100%;
  background-color: rgba(0, 0, 0, 0.5);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

.modal-content {
  background-color: white;
  padding: 2rem;
  border-radius: 8px;
  width: 300px;
}

.error {
  color: red;
}
</style>