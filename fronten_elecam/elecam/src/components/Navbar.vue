<template>
  <nav class="navbar">
    <div class="logo">
      🗳️ <span>ELECAM</span>
    </div>

    <ul class="nav-links" :class="{ open: menuOpen }">
      <li><router-link to="/candidates" class="link" active-class="active">Candidats</router-link></li>
      <li><router-link to="/voters" class="link" active-class="active">Électeurs</router-link></li>
      <li><router-link to="/vote" class="link" active-class="active">Vote</router-link></li>
      <li><router-link to="/" class="link" active-class="active">Home</router-link></li>
      <li><router-link to="/results" class="link" active-class="active">Result</router-link></li>

    </ul>

    <div class="burger" @click="menuOpen = !menuOpen">
      <div></div>
      <div></div>
      <div></div>
    </div>
  </nav>
</template>

<script setup>
import { ref } from 'vue'
const menuOpen = ref(false)
</script>

<style scoped>
.navbar {
  display: flex;
  justify-content: space-between;
  align-items: center;
  background: linear-gradient(90deg, #003366, #0055aa);
  padding: 1rem 2rem;
  color: white;
  position: sticky;
  top: 0;
  z-index: 10;
}

.logo {
  font-size: 1.4rem;
  font-weight: bold;
  display: flex;
  align-items: center;
  gap: 0.5rem;
}

.logo span {
  color: #00ccff;
  letter-spacing: 1px;
}

.nav-links {
  list-style: none;
  display: flex;
  gap: 1.5rem;
}

.link {
  color: white;
  text-decoration: none;
  font-weight: 500;
  transition: 0.3s;
}

.link:hover {
  color: #00ccff;
}

.active {
  border-bottom: 2px solid #00ccff;
  padding-bottom: 4px;
}

.burger {
  display: none;
  flex-direction: column;
  gap: 6px;
  cursor: pointer;
}

.burger div {
  width: 25px;
  height: 3px;
  background: white;
  border-radius: 2px;
}

/* --- Responsive --- */
@media (max-width: 768px) {
  .nav-links {
    position: absolute;
    right: 0;
    top: 65px;
    background: #002244;
    flex-direction: column;
    align-items: center;
    width: 100%;
    height: 0;
    overflow: hidden;
    transition: height 0.3s ease;
  }

  .nav-links.open {
    height: 150px;
    padding: 1rem 0;
  }

  .burger {
    display: flex;
  }
}
</style>
