<template>
  <div>
    <!-- Login Form -->
    <form @submit.prevent="login">
      <input v-model="username" type="text" placeholder="Username" required />
      <br />
      <input v-model="password" type="password" placeholder="Password" required />
      <br />
      <button type="submit">Login</button>
      <br />
      <button type="button" @click="logout">Logout</button>
    </form>

    <!-- Register Form -->
    <form @submit.prevent="register">
      <input v-model="newUser.username" type="text" placeholder="Username" required />
      <br />
      <input v-model="newUser.password" type="password" placeholder="Password" required />
      <br />
      <input v-model="newUser.email" type="email" placeholder="Email" required />
      <br />
      <button type="submit">Register</button>
    </form>

    <button @click="deleteAllUsers">Delete All Users</button>
    <br>
    <button @click="fetchUsers">Log All Users</button>

    <p v-if="errorMessage" class="error">{{ errorMessage }}</p>
  </div>
</template>

<script>
import axios from "axios";

export default {
  data() {
    return {
      username: "",
      password: "",
      errorMessage: "",
      newUser: {
        username: "",
        password: "",
        email: ""
      },
    };
  },
  methods: {
    async login() {
      try {
        const response = await axios.post("http://localhost:5194/api/auth/login", {
          UserName: this.username,
          Password: this.password,
        });

        console.log(response.data);
        localStorage.setItem("authToken", response.data.token);

      } catch (error) {
        if (error.response && error.response.data) {
          this.errorMessage = error.response.data.message;
        } else {
          this.errorMessage = "";
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
          "http://localhost:5194/api/auth/logout",
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

    async register() {
      try {
        const response = await axios.post("http://localhost:5194/api/auth/register", {
          UserName: this.newUser.username,
          Password: this.newUser.password,
          Email: this.newUser.email,
        });


        console.log(response.data);
      } catch (error) {
        if (error.response && error.response.data) {
          this.errorMessage = error.response.data.message;
        } else {
          this.errorMessage = "An unexpected error occurred during registration.";
        }
      }
    },
    async deleteAllUsers() {
      try {
        const response = await axios.delete('http://localhost:5194/api/auth/delete/-1');
        alert(response.data.message); // Show success message
      } catch (error) {
        console.error("Error deleting users:", error.response?.data || error.message);
      }
    },
    async fetchUsers() {
      try {
        const response = await axios.get("http://localhost:5194/api/auth/GetAll");
        console.log(response.data); // This will log the list of users
      } catch (error) {
        console.error("Error fetching users:", error);
      }
    }
  },
};
</script>

<style scoped>
.error {
  color: red;
}
</style>
