<template>
  <div class="card">
    <div class="ima">
      <img :src="candidate.url_image" alt="Photo du candidat" />
    </div>
    <h3>{{ candidate.nom }}</h3>
    <p><strong>Parti :</strong> {{ candidate.parti }}</p>
    <p><strong>Âge :</strong> {{ candidate.age }}</p>
    <p><strong>Votes :</strong> {{ candidate.votes }}</p>
    <p><strong>Nationalité :</strong> {{ candidate.nationalite }}</p>

  </div>
</template>

<script setup>
defineProps({
  candidate: Object
})
</script>

<style scoped>
.ima {
  display: flex;
  justify-content: center;
  align-items: center;
  margin-bottom: 1rem;
}
img{
  border-radius: 50%;
  width: 120px;
  height: 120px;
  border-radius: 50%;
  object-fit: cover;
  border: 3px solid #008080;
  box-shadow: 0 0 10px rgba(0,0,0,0.2);
  transition: transform 0.3s ease;

}
img:hover {
  transform: scale(1.05);
}
.card {
  background: #f5f5f5;
  border-radius: 10px;
  padding: 1rem;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
  transition: transform 0.2s ease;
}
.card:hover {
  transform: translateY(-5px);
}
</style>
