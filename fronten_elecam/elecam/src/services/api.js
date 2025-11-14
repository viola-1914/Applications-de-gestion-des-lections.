import axios from 'axios'
import { BASE_URL } from './config'

export const api = {
  getCandidates: () => axios.get(`${BASE_URL}/candidates`),
  addCandidate: (data) => axios.post(`${BASE_URL}/candidates`, data),
  
  getVoters: () => axios.get(`${BASE_URL}/voters`),
  addVoter: (data) => axios.post(`${BASE_URL}/voters`, data),
  
  vote: (data) => axios.post(`${BASE_URL}/vote`, data),
  getResults: () => axios.get(`${BASE_URL}/results`),
  getRefunded: () => axios.get(`${BASE_URL}/refunded`),
  getRejected: () => axios.get(`${BASE_URL}/rejected`)  
}
