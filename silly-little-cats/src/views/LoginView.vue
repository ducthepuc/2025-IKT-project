<template>
    <div class="login-container">
      <header class="login-header">
        <h1>Welcome!</h1>
        <p>Log in to your account</p>
      </header>
      <main class="login-main">
        <form class="login-form">
          <div class="form-group">
            <label for="username">Username:</label>
            <input type="text" id="username" placeholder="Enter your username">
          </div>
          <div class="form-group">
            <label for="password">Password:</label>
            <input type="password" id="password" placeholder="Enter your password">
          </div>
          <button type="submit" class="login-button">Log In</button>
          <p class="signup-link">Don't have an account? <router-link to="/register">Register</router-link></p>
        </form>
      </main>
      <footer class="login-footer">
        <p>&copy; 2025 SLC</p>
      </footer>
    </div>
  </template>
  
  <script>
  export default {
    name: 'LoginView',

  }
  </script>
  
  <style scoped>
  .login-container {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    min-height: 100vh;
    background-color: #f8f9fa;
    padding: 2rem;
  }
  
  .login-header {
    text-align: center;
    margin-bottom: 2rem;
  }
  
  .login-header h1 {
    font-size: 2.5rem;
    color: var(--primary-color, #007bff);
    margin-bottom: 0.5rem;
  }
  
  .login-header p {
    color: #6c757d;
  }
  
  .login-main {
    background-color: white;
    padding: 2rem;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    width: 90%;
    max-width: 400px;
  }
  
  .login-form {
    display: flex;
    flex-direction: column;
  }
  
  .form-group {
    margin-bottom: 1.5rem;
  }
  
  .form-group label {
    display: block;
    margin-bottom: 0.5rem;
    color: #495057;
    font-weight: bold;
  }
  
  .form-group input[type="text"],
  .form-group input[type="password"] {
    width: 100%;
    padding: 0.75rem;
    border: 1px solid #ced4da;
    border-radius: 4px;
    font-size: 1rem;
  }
  
  .login-button {
    background-color: var(--primary-color, #007bff);
    color: white;
    padding: 0.75rem 1.5rem;
    border: none;
    border-radius: 4px;
    cursor: pointer;
    font-size: 1.1rem;
    transition: background-color 0.2s ease;
  }
  
  .login-button:hover {
    background-color: darken(#007bff, 10%);
  }
  
  .signup-link {
    text-align: center;
    margin-top: 1rem;
    color: #6c757d;
    font-size: 0.9rem;
  }
  
  .signup-link a {
    color: var(--accent-color, #28a745);
    text-decoration: none;
  }
  
  .signup-link a:hover {
    text-decoration: underline;
  }
  
  .login-footer {
    margin-top: 2rem;
    text-align: center;
    color: #6c757d;
    font-size: 0.8rem;
  }
  
  @media (max-width: 768px) {
    .login-main {
      width: 95%;
      padding: 1.5rem;
    }
  
    .login-header h1 {
      font-size: 2rem;
    }
  }
  
  @media (max-width: 480px) {
    .login-main {
      padding: 1rem;
    }
  
    .login-header h1 {
      font-size: 1.8rem;
    }
  }
  </style>