import { createRouter, createWebHistory } from 'vue-router'
import HomeView from '../views/HomeView.vue'
import CandidatesView from '../views/CandidatesView.vue'
import VotersView from '../views/VotersView.vue'
import VoteView from '../views/VoteView.vue'
import ResultsView from '../views/ResultsView.vue'

const routes = [
  { path: '/',
     component: HomeView },
  { path: '/candidates',
     component: CandidatesView },
  { path: '/voters',
     component: VotersView },
  { path: '/vote',
     component: VoteView },
  { path: '/results',
     component: ResultsView },
]

export default createRouter({
  history: createWebHistory(),
  routes
})
