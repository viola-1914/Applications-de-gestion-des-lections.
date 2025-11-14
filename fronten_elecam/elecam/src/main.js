import { createApp } from 'vue'
import './style.css'
import App from './App.vue'

//import du routeur
import router from './router'

const app = createApp(App)

// On relie le routeur à l'application
app.use(router)

//On monte l'app sur la page
app.mount('#app')

