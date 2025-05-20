import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'
import ProductView from '../views/ProductView.vue'
import AdminView from '../views/AdminView.vue'
import LoginView from '../views/LoginView.vue'
import RegisterView from '../views/RegisterView.vue'

const routes = [
  {
    path: '/',
    name: 'home',
    component: HomeView
  },
  {
    path: '/product/:id',
    name: 'product',
    component: ProductView
  },
  {
    path: '/about',
    name: 'about',
    component: () => import(/* webpackChunkName: "about" */ '../views/AboutView.vue')
  },
  {
    path: '/admin',
    name: 'admin',
    component: AdminView
  },
  {
    path: "/login",
    name: "login",
    component: LoginView
  },
  {
    path: "/register",
    name: "register",
    component: RegisterView
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

router.beforeEach(async (to, from, next) => {
  const protectedRoutes = ['/admin'];
  const token = localStorage.getItem('authToken');
  console.log(token)

  if (protectedRoutes.includes(to.path)) {
    console.log(token)
    if (!token) return next('/login');

    try {
      /*const res = await fetch('http://localhost:3000/api/validate-token', {
        headers: {
          Authorization: `Bearer ${token}`
        }
      });
      if (res.status !== 200) {
        localStorage.removeItem('authToken');
        return next('/login');
      }
      */

      return next();
    } catch {
      localStorage.removeItem('authToken');
      return next('/login');
    }
  }

  next();
});


export default router
