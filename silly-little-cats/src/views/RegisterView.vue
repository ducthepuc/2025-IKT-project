<template>
    <div class="register-container">
      <header class="register-header">
        <h1>Join Us!</h1>
        <p>Create your new account</p>
      </header>
      <main class="register-main">
        <form class="register-form">
          <div class="form-group">
            <label for="username">Username:</label>
            <input type="text" id="username" placeholder="Choose a username">
          </div>
          <div class="form-group">
            <label for="email">Email:</label>
            <input type="email" id="email" placeholder="Enter your email address">
          </div>
          <div class="form-group">
            <label for="password">Password:</label>
            <input type="password" id="password" placeholder="Create a password">
          </div>
          <div class="form-group">
            <label for="confirm-password">Confirm Password:</label>
            <input type="password" id="confirm-password" placeholder="Confirm your password">
          </div>
          <button type="submit" class="register-button">Sign Up</button>
          <p class="login-link">Already have an account? <router-link to="/login">Log in</router-link></p>
        </form>
      </main>
      <footer class="register-footer">
        <p>&copy; 2025 My Awesome App</p>
      </footer>
    </div>
  </template>
  
  <script>
  export default {
    name: 'RegisterView',

  }
  </script>
  
  <style scoped>
  .register-container {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    min-height: 100vh;
    background-color: #f8f9fa;
    padding: 2rem;
  }
  
  .register-header {
    text-align: center;
    margin-bottom: 2rem;
  }
  
  .register-header h1 {
    font-size: 2.5rem;
    color: var(--primary-color, #007bff);
    margin-bottom: 0.5rem;
  }
  
  .register-header p {
    color: #6c757d;
  }
  
  .register-main {
    background-color: white;
    padding: 2rem;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
    width: 90%;
    max-width: 400px;
  }
  
  .register-form {
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
  .form-group input[type="email"],
  .form-group input[type="password"] {
    width: 100%;
    padding: 0.75rem;
    border: 1px solid #ced4da;
    border-radius: 4px;
    font-size: 1rem;
  }
  
  .register-button {
    background-color: var(--accent-color, #28a745);
    color: white;
    padding: 0.75rem 1.5rem;
    border: none;
    border-radius: 4px;
    cursor: pointer;
    font-size: 1.1rem;
    transition: background-color 0.2s ease;
  }
  
  .register-button:hover {
    background-color: darken(#28a745, 10%);
  }
  
  .login-link {
    text-align: center;
    margin-top: 1rem;
    color: #6c757d;
    font-size: 0.9rem;
  }
  
  .login-link a {
    color: var(--primary-color, #007bff);
    text-decoration: none;
  }
  
  .login-link a:hover {
    text-decoration: underline;
  }
  
  .register-footer {
    margin-top: 2rem;
    text-align: center;
    color: #6c757d;
    font-size: 0.8rem;
  }
  
  /* Responsive adjustments */
  @media (max-width: 768px) {
    .register-main {
      width: 95%;
      padding: 1.5rem;
    }
  
    .register-header h1 {
      font-size: 2rem;
    }
  }
  
  @media (max-width: 480px) {
    .register-main {
      padding: 1rem;
    }
  
    .register-header h1 {
      font-size: 1.8rem;
    }
  }
  </style>