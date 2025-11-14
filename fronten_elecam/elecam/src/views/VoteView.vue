<template>
  <div class="page">
    

    <h1 class="title">🗳️ Espace de Vote</h1>

    <!-- Sélection de l’électeur -->
    <div class="form">
      <input v-model.number="vote.id_electeur" type="number" placeholder="ID de l’électeur" required />
      <select v-model.number="vote.id_candidat">
        <option disabled value="">-- Choisir un candidat --</option>
        <option v-for="c in candidates" :key="c.id" :value="c.id">
          {{ c.nom }} ({{ c.parti }})
        </option>
      </select>
      <button @click="submitVote">Voter</button>
    </div>

    <hr />

    <!-- Résultats -->
    <h2 class="subtitle">Résultats du vote</h2>
    <button @click="loadResults">🔄 Actualiser</button>

    <div class="results">
      <div v-for="r in results" :key="r.id" class="result-card">
        <h3>{{ r.nom }} — {{ r.parti }}</h3>
        <p><strong>Votes :</strong> {{ r.votes }}</p>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import Navbar from '../components/Navbar.vue'
import { api } from '../services/api'

const candidates = ref([])
const results = ref([])
const vote = ref({ id_electeur: '', id_candidat: '' })

const loadCandidates = async () => {
  const res = await api.getCandidates()
  candidates.value = res.data
}

const loadResults = async () => {
  const res = await api.getResults()
  results.value = res.data
}

const submitVote = async () => {
  try {
    await api.vote(vote.value)
    alert('✅ Vote enregistré avec succès !')
    vote.value = { id_electeur: '', id_candidat: '' }
    loadResults()
  } catch (err) {
    alert('❌ Erreur lors du vote.')
    console.error(err)
  }
}

onMounted(() => {
  loadCandidates()
  loadResults()
})
</script>

<style scoped>
.page {
  padding: 2rem;
  font-family: 'Poppins', sans-serif;
}
.title {
  text-align: center;
  color: #222;
  margin-bottom: 1.5rem;
}
.form {
  display: flex;
  justify-content: center;
  align-items: center;
  gap: 1rem;
  margin-bottom: 2rem;
}
input, select, button {
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
  background-color: #007777;
}
.results {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 1rem;
}
.result-card {
  background: #f5f5f5;
  border-radius: 10px;
  padding: 1rem;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
}
.subtitle {
  text-align: center;
  margin: 1rem 0;
  color: #333;
}
</style>
