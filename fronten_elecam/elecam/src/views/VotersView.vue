<template>
  <div class="page">
    

    <h1 class="title">Liste des Électeurs</h1>

    <!-- ✅ Formulaire d'ajout -->
    <form @submit.prevent="addVoter" class="form">
      <input v-model="newVoter.nom" type="text" placeholder="Nom de l’électeur" required />
      <input v-model="newVoter.quartier" type="text" placeholder="Quartier" required />
      <input v-model.number="newVoter.age" type="number" placeholder="Âge" required />
      <button type="submit">Ajouter</button>
    </form>

    <!-- ✅ Liste des électeurs -->
    <div class="voters-list">
      <div
        v-for="v in voters"
        :key="v.id"
        class="voter-card"
      >
        <h3>{{ v.nom }}</h3>
        <p><strong>Quartier :</strong> {{ v.quartier }}</p>
        <p><strong>Âge :</strong> {{ v.age }}</p>
        <p>
          <strong>Statut :</strong>
          <span :class="v.has_voted ? 'voted' : 'not-voted'">
            {{ v.has_voted ? 'A voté ✅' : 'N’a pas voté ❌' }}
          </span>
        </p>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import Navbar from '../components/Navbar.vue'
import { api } from '../services/api'

const voters = ref([])
const newVoter = ref({ nom: '', quartier: '', age: '' })

const loadVoters = async () => {
  try {
    const res = await api.getVoters()
    voters.value = res.data
  } catch (err) {
    console.error('Erreur lors du chargement des électeurs', err)
  }
}

const addVoter = async () => {
  try {
    await api.addVoter(newVoter.value)
    alert('✅ Électeur ajouté avec succès !')
    newVoter.value = { nom: '', quartier: '', age: '' }
    loadVoters()
  } catch (err) {
    alert('❌ Erreur lors de l’ajout de l’électeur')
    console.error(err)
  }
}

onMounted(loadVoters)
</script>

<style scoped>
.page {
  padding: 2rem;
  font-family: 'Poppins', sans-serif;
}
.title {
  text-align: center;
  margin-bottom: 1.5rem;
  color: #333;
}
.form {
  display: flex;
  justify-content: center;
  gap: 1rem;
  margin-bottom: 2rem;
}
input, button {
  padding: 0.6rem 1rem;
  border-radius: 8px;
  border: 1px solid #ccc;
}
button {
  background-color: #004080;
  color: white;
  cursor: pointer;
}
button:hover {
  background-color: #003366;
}
.voters-list {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 1rem;
}
.voter-card {
  background: #f5f5f5;
  padding: 1rem;
  border-radius: 10px;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
}
.voted {
  color: green;
  font-weight: bold;
}
.not-voted {
  color: red;
  font-weight: bold;
}
</style>
