# Parcial — Inteligencia Artificial en Videojuegos

## Introducción

El parcial consiste en el desarrollo de un minijuego libre en donde el alumno deberá desarrollarlo utilizando las herramientas de IA. El objetivo es aplicar los conocimientos adquiridos en **Behavior Trees**, **Navigation**, **Pathfinding**, **AI Perception System**, **EQS** y otras herramientas de IA en videojuegos.

Hacer el minijuego requerirá un mínimo de puntos necesarios para aprobar (4). Los puntos adicionales permitirán obtener una mejor calificación hasta alcanzar la nota máxima (10).

---

## Formato de entrega

Cada alumno deberá entregar un archivo **ZIP con el proyecto** o un **enlace a un repositorio** donde se puedan ver los archivos creados. No es necesario entregar una build del juego.

---

## Minijuego Libre

### Descripción

El minijuego deberá desarrollarse en un **único nivel** en el que el jugador controla un personaje dentro de un entorno cerrado. Debe haber también una **zona segura**.

El objetivo podrá ser:
- **Infiltrarse**, o
- **Sobrevivir**, o
- **Alcanzar un punto específico del mapa**,

mientras interactúa con múltiples agentes controlados por IA.

### Requisitos de los agentes

- Comportamiento **autónomo desde el inicio** de la partida.
- Comienzan **patrullando** el escenario usando el sistema de navegación.
- Se desplazan correctamente por el **NavMesh** y adaptan rutas ante obstáculos.
- **No pueden permanecer estáticos** ni reaccionar únicamente cuando el jugador se acerque.
- Al detectar al jugador por **visión** o **sonido**, deben cambiar de comportamiento de manera **clara y progresiva**:
  - Alerta → persecución → combate (o equivalentes).
- El sistema debe estar organizado en **estados bien definidos y acciones específicas**, demostrando estructura clara y coherente en la toma de decisiones.

### Otros requerimientos

- Condición de **victoria** y **derrota** claramente establecidas.
- **HUD básico** con info relevante: vida del jugador, estado general de la situación.

### Lo que se verifica durante la evaluación

- Los agentes navegan correctamente.
- Reaccionan ante estímulos.
- Cambian de comportamiento de forma coherente.
- Utilizan objetos del entorno.
- Funcionan correctamente con **múltiples instancias activas** simultáneamente.

> Objetivo del examen: demostrar la construcción de un sistema de **IA estructurado**, capaz de tomar decisiones en función del entorno y estímulos recibidos.

---

## Criterios de Evaluación

### Mínimo para aprobar (nota 4)
- Al menos **2 NPCs** con mecánicas diferentes.
- Implementación de **NavMesh** y **Pathfinding**.
- Al menos **un Behavior Tree o State Tree**.
- **AI Perception System** (vista/oído) para la percepción del minijuego.

### Puntos adicionales (hasta nota 10)
- Implementar **ambos** Behavior Tree **y** State Tree.
- Al menos una decisión de selección de objetivo/punto resuelta con **EQS**.
  - Ejemplos: elegir mejor cobertura, posición de disparo, punto de inspección.
- Al menos un **Smart Object interactuable** que los AIs usen correctamente.
  - Ejemplos: cobertura reutilizable, puerta que abre/cierra, zona de curación.

---

## Notas finales

- Cada estudiante desarrolla un minijuego libre.
- Se aprueba con los requisitos mínimos para el 4. Los extras permiten llegar a 10.
- La evaluación se basa en **funcionalidad**, **implementación de IA** y **originalidad**.
