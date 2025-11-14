<template>
  <div class="page">
    

    <h1 class="title">Liste des Candidats</h1>

    <!-- ✅ Formulaire d'ajout -->
    <form @submit.prevent="addCandidate" class="form">
      <input v-model="newCandidate.nom" type="text" placeholder="Nom du candidat" required />
      <input v-model="newCandidate.parti" type="text" placeholder="Parti politique" required />
      <input v-model.number="newCandidate.age" type="number" placeholder="Âge" required />
      <input v-model="newCandidate.nationalite" type="text" placeholder="Nationalité" required />
      <input v-model="newCandidate.url_image" type="text" placeholder="URL de l’image" required />

      <button type="submit">Ajouter</button>
    </form>

    <!-- ✅ Liste des candidats -->
    <div class="candidates-list">
      <CandidateCard
        v-for="c in candidates"
        :key="c.id"
        :candidate="c"
      />
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'

import CandidateCard from '../components/CandidateCard.vue'
import { api } from '../services/api'

// 🔹 Liste des candidats
const candidates = ref([])

// 🔹 Candidat à ajouter
const newCandidate = ref({
  nom: '',
  parti: '',
  age: ''
})

// 🔹 Charger les candidats au montage
const loadCandidates = async () => {
  try {
    const res = await api.getCandidates()
    candidates.value = res.data
  } catch (err) {
    console.error('Erreur de chargement des candidats', err)
  }
}

// 🔹 Ajouter un candidat
const addCandidate = async () => {
  try {
    await api.addCandidate(newCandidate.value)
    alert('✅ Candidat ajouté avec succès !')
    newCandidate.value = { nom: '', parti: '', age: '' }
    loadCandidates() // recharge la liste
  } catch (err) {
    alert('❌ Erreur lors de l’ajout du candidat')
    console.error(err)
  }
}

onMounted(loadCandidates)
</script>

<style scoped>
.page {
  padding: 2rem;
  font-family: 'Poppins', sans-serif;
}
.title {
  text-align: center;
  color: #333;
  margin-bottom: 1.5rem;
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
  background-color: teal;
  color: white;
  border: none;
  cursor: pointer;
}
button:hover {
  background-color: #008080;
}
.candidates-list {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 1.5rem;
}
</style>
