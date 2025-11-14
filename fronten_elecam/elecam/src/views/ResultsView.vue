<template>
  <div class="results-page">
    <h1>Résultats de l’élection</h1>

    <div class="actions">
      <button @click="loadResults">🔄 Rafraîchir les résultats</button>
      <button @click="loadRefunded">💰 Candidats remboursés</button>
      <button @click="loadRejected">❌ Candidatures rejetées</button>
    </div>

    <div v-if="loading" class="loading">Chargement...</div>

    <!-- ✅ Section des résultats -->
    <div v-else class="results-list">
      <div
        v-for="(c, i) in results"
        :key="c.id"
        class="result-card"
        :class="{ top: i === 0 }"
      >
        <div class="rank">#{{ i + 1 }}</div>
        <div class="info">
          <h3>{{ c.nom }}</h3>
          <p><strong>Parti :</strong> {{ c.parti }}</p>
          <p><strong>Votes :</strong> {{ c.votes }}</p>
          <p v-if="c.pourcentage"><strong>Pourcentage :</strong> {{ c.pourcentage }}%</p>
        </div>
      </div>
    </div>

    <!-- ✅ Section spéciale rejetés -->
    <div v-if="rejectedCount !== null" class="rejected-box">
      <h2>❌ Candidatures rejetées : {{ rejectedCount }}</h2>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { api } from '../services/api'

const results = ref([])
const loading = ref(true)
const rejectedCount = ref(null)

// 🔹 Charger les résultats triés
const loadResults = async () => {
  loading.value = true
  rejectedCount.value = null
  try {
    const res = await api.getResults()
    results.value = res.data
  } catch (err) {
    alert('Erreur lors du chargement des résultats 😢')
  } finally {
    loading.value = false
  }
}

// 🔹 Candidats remboursés
const loadRefunded = async () => {
  loading.value = true
  rejectedCount.value = null
  try {
    const res = await api.getRefunded()
    results.value = res.data
  } catch (err) {
    alert('Erreur lors du chargement des remboursés 😢')
  } finally {
    loading.value = false
  }
}

// 🔹 Candidats rejetés (accepted = 0)
const loadRejected = async () => {
  loading.value = true
  try {
    const res = await api.getRejected()
    rejectedCount.value = res.data.rejected
    results.value = []
  } catch (err) {
    alert('Erreur lors du comptage des rejetés 😢')
  } finally {
    loading.value = false
  }
}

onMounted(loadResults)
</script>

<style scoped>
.results-page {
  padding: 2rem;
  font-family: 'Poppins', sans-serif;
  background: linear-gradient(135deg, #e0f7fa, #f1f8e9);
  min-height: 100vh;
  text-align: center;
}

.actions {
  display: flex;
  justify-content: center;
  gap: 1rem;
  margin-bottom: 2rem;
}

button {
  background-color: #00897b;
  color: white;
  border: none;
  border-radius: 8px;
  padding: 0.7rem 1.5rem;
  cursor: pointer;
  font-weight: 600;
  transition: background 0.3s ease;
}
button:hover {
  background-color: #00695c;
}

.rejected-box {
  margin-top: 2rem;
  background-color: #ffebee;
  border-left: 5px solid #c62828;
  padding: 1rem;
  display: inline-block;
  border-radius: 10px;
  box-shadow: 0 2px 6px rgba(0,0,0,0.1);
}
.rejected-box h2 {
  color: #c62828;
  margin: 0;
}
</style>
