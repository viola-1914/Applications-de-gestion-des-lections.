<template>
  
  <div class="home">
    <section class="hero">
      <div class="hero-content">
        <h1>🇨🇲 Système Électoral du Cameroun</h1>
        <p>
          Bienvenue sur la plateforme numérique <strong>ELECAM</strong> —
          un système transparent et moderne pour la gestion des élections présidentielles.
          Ici, chaque électeur compte, chaque voix est tracée, et chaque candidat est visible.
        </p>

        <div class="actions">
          <router-link to="/candidates" class="btn">👤 Voir les candidats</router-link>
          <router-link to="/voters" class="btn">🧾 Liste des électeurs</router-link>
          <router-link to="/vote" class="btn">🗳️ Voter maintenant</router-link>
        </div>
      </div>
    </section>

    <section class="about">
      <h2>Notre mission</h2>
      <p>
        ELECAM vise à <strong>moderniser la gestion électorale</strong> au Cameroun grâce à la technologie.
        Cette application relie un <em>backend en C</em> robuste et sécurisé
        à une interface web dynamique construite avec <strong>Vue.js</strong>.
      </p>

      <div class="features">
        <div class="card">
          <h3>🧑‍💼 Gestion des candidats</h3>
          <p>Ajoute, consulte et classe les candidats selon les résultats en temps réel.</p>
        </div>
        <div class="card">
          <h3>🧑‍🤝‍🧑 Gestion des électeurs</h3>
          <p>Enregistre les électeurs et vérifie en un clic s’ils ont déjà voté.</p>
        </div>
        <div class="card">
          <h3>📊 Comptabilisation automatique</h3>
          <p>Les votes sont comptés automatiquement et les résultats générés instantanément.</p>
        </div>
      </div>
    </section>
  </div>
</template>

<script setup>
</script>

<style scoped>
.home {
  font-family: "Poppins", sans-serif;
  color: #222;
  line-height: 1.6;
}

/* --- Section Héro --- */
.hero {
  background: linear-gradient(120deg, #003366, #0055aa);
  color: white;
  text-align: center;
  padding: 5rem 2rem;
  display: flex;
  justify-content: center;
  align-items: center;
}

.hero-content {
  max-width: 800px;
}

.hero h1 {
  font-size: 2.5rem;
  margin-bottom: 1rem;
}

.hero p {
  font-size: 1.1rem;
  margin-bottom: 2rem;
}

.actions {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 1rem;
}

.btn {
  background: #00ccff;
  color: #003366;
  padding: 0.8rem 1.5rem;
  border-radius: 25px;
  text-decoration: none;
  font-weight: bold;
  transition: 0.3s;
}

.btn:hover {
  background: #00aadd;
  transform: scale(1.05);
}

/* --- Section À propos --- */
.about {
  text-align: center;
  padding: 4rem 2rem;
  background: #f8f9fa;
}

.about h2 {
  font-size: 2rem;
  color: #003366;
  margin-bottom: 1rem;
}

.features {
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  gap: 2rem;
  margin-top: 2rem;
}

.card {
  background: white;
  border-radius: 12px;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.1);
  width: 280px;
  padding: 1.5rem;
  transition: 0.3s;
}

.card:hover {
  transform: translateY(-5px);
}

.card h3 {
  color: #0055aa;
  margin-bottom: 0.5rem;
}
</style>
