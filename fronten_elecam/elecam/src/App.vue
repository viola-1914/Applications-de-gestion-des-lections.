<template>
  <div id="app">
    <Navbar/>
     <!-- 👈 Intégration ici -->
    <router-view />
  </div>
</template>

<script setup>

     import Navbar from './components/Navbar.vue';                                        // 👈 import de ton fichier


</script>

<style scoped>
#app {
  text-align: center;
  padding-top: 50px;
}
</style>
