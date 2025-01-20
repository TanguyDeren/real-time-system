# -*- coding: utf-8 -*-
"""
Created on Mon Jan 20 16:53:02 2025

@author: tangu
"""

from itertools import permutations as perm

# Liste des tâches
tasks_data = [
    [11, 2, 10],
    [21, 3, 10],
    [31, 2, 20],
    [41, 2, 20],
    [51, 2, 40],
    [61, 2, 40],
    [71, 3, 80]]

# Paramètres de la configuration
task_count = 3
hyperperiod_value = 20
print('Début de la création des jobs')

# Génération des jobs à partir des données
job_collection = []

for i in range(task_count):
    print(f"Traitement de la tâche {i + 1}")
    job_frequency = hyperperiod_value / tasks_data[i][2]
    print(f"Fréquence des jobs : {job_frequency}")

    for j in range(int(job_frequency)):
        job_identifier = 10 * (i + 1) + (j + 1)
        print(f"Job identifier : {job_identifier}")

        job_arrival_time = j * tasks_data[i][2]
        job_deadline_time = (j + 1) * tasks_data[i][2]

        task_details = [job_identifier, tasks_data[i][1], job_arrival_time, job_deadline_time]
        job_collection.append(task_details)
        print(f"Ajout du job : {task_details}")

def is_schedulable(jobs_sequence, allowed_task_id=None, max_deadline_misses=1):
    """
    Vérifie si une séquence donnée de jobs est schedulable.
    Renvoie True si tous les jobs respectent les deadlines, sinon False.
    """
    print('Vérification de la planification')
    current_time = 0
    total_waiting_time = 0
    missed_deadlines = 0

    for job in jobs_sequence:
        job_id, exec_time, arrival_time, deadline = job

        if current_time < arrival_time:
            current_time = arrival_time

        total_waiting_time += current_time - arrival_time

        if current_time + exec_time > deadline:
            if allowed_task_id and job_id // 10 == allowed_task_id:
                missed_deadlines += 1
                if missed_deadlines > max_deadline_misses:
                    return False, float('inf')
            else:
                return False, float('inf')

        current_time += exec_time
        print(f"Tâche exécutée : {job}")

    return True, total_waiting_time

# Calculer toutes les permutations des jobs
all_job_permutations = perm(job_collection)

# Analyser les permutations sans autoriser les deadlines manquées
valid_schedules = []
minimum_waiting_time = float('inf')
best_schedule = None

for permutation in all_job_permutations:
    schedulable, waiting_time = is_schedulable(permutation)
    if schedulable:
        valid_schedules.append((permutation, waiting_time))
        if waiting_time < minimum_waiting_time:
            minimum_waiting_time = waiting_time
            best_schedule = permutation

# Analyser les permutations avec tolérance pour une deadline manquée de tau5
tau5_allowed_miss = []

for permutation in all_job_permutations:
    schedulable, waiting_time = is_schedulable(permutation, allowed_task_id=5, max_deadline_misses=1)
    if schedulable:
        tau5_allowed_miss.append((permutation, waiting_time))

# Affichage des résultats
if valid_schedules:
    print("Planifications valides avec temps d'attente total :")
    for schedule, waiting_time in valid_schedules:
        print(f"Ordre des jobs : {[job[0] for job in schedule]} | Temps d'attente : {waiting_time}")

    print("\nPlanification optimale :")
    print(f"Ordre des jobs : {[job[0] for job in best_schedule]} | Temps d'attente minimal : {minimum_waiting_time}")
else:
    print("Aucune planification valide.")

# Affichage de la meilleure planification avec tolérance pour tau5
if tau5_allowed_miss:
    best_tau5_schedule = min(tau5_allowed_miss, key=lambda x: x[1])
    print("\nPlanification optimale (tau5 peut manquer une deadline) :")
    print(f"Ordre des jobs : {[job[0] for job in best_tau5_schedule[0]]} | Temps d'attente minimal : {best_tau5_schedule[1]}")
else:
    print("\nAucune planification valide pour tau5 avec une deadline manquée autorisée.")
