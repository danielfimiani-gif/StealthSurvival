# Parcial — Motores (Unreal Engine + C++)

## Definición

El examen consiste en desarrollar un videojuego en Unreal Engine utilizando C++. El proyecto deberá incluir mecánicas distintas y contar con un flujo de juego completo y funcional, desde el inicio hasta la finalización de la partida. Se evaluará la correcta implementación de las mecánicas, así como la aplicación de los conceptos y técnicas vistos durante la cursada.

### Flujo de juego obligatorio

1. **Menú Principal** en un nivel separado del Gameplay.
   - Permite iniciar la partida y acceder a opciones básicas.
   - La transición al nivel de juego debe estar implementada en C++, asegurando carga de mapa e inicialización de sistemas.

2. **Gameplay**: ciclo completo de partida.
   - Objetivo claro y condiciones explícitas de victoria y derrota, definidas por el alumno.
   - Mecánicas distintas, integradas entre sí, coherentes con la propuesta.
   - Las mecánicas deben estar implementadas en **C++** y ser parte activa del núcleo (no decorativas ni aisladas).
   - Mundo de juego con **al menos 3 enemigos y/o powerups funcionales**, que interactúen significativamente con las mecánicas principales.

3. **Personaje principal**
   - Sistema de control con **Enhanced Input** correctamente aplicado (estructura ordenada y escalable).
   - Sistema de animaciones con **mínimo 3 estados** claramente diferenciables (ej.: Idle / Walk / Run).
   - Si el proyecto es en primera persona, estos estados deben evidenciarse al menos en otro personaje o entidad visible.

4. **HUD funcional e integrado** durante toda la partida.
   - Información clara y actualizada del estado y progreso (vida, temporizador, puntaje, etc.).
   - La UI debe formar parte activa del gameplay y actualizarse según los eventos.

5. **Pantalla final** al alcanzar victoria o derrota.
   - Condición claramente identificable por el jugador.
   - Permite **reiniciar la partida** o **regresar al menú principal**.
   - Habilita rejugabilidad sin reiniciar la aplicación.

### Lo que se evalúa

No solo funcionamiento técnico, también:
- Coherencia del diseño.
- Claridad del objetivo.
- Correcta estructuración del flujo de juego.
- Integración consistente de las herramientas vistas durante la cursada en C++.

---

## Entregables

### Fecha de entrega
**8 de junio de 2026, 18:00 hs**.

### Archivos
- **`TP_NombreCompleto.pdf`**: documento actualizado siguiendo la definición del juego mediante descomposición de elementos:
  - Título
  - Descripción (contexto de Game Design, con referencias)
  - Actores
  - Mecánicas
  - Metas y obstáculos
  - Reglas
  - Espacio
  - Git / Diversión
- **El PDF es obligatorio.** Sin él, instancia recuperatorio.
- **ZIP del proyecto** o enlace al repositorio donde se vean los archivos creados.

---

## Criterios de Evaluación

### Mínimo para aprobar (nota 4)
- El juego se ejecuta correctamente, sin errores críticos ni cierres inesperados.
- Implementación de **al menos 5 mecánicas funcionales**.
- Implementación correcta de los conceptos del **Gameplay Framework**.
- Comunicación clara y optimizada entre clases C++ (referencias, interfaces o eventos).
- Desarrollar **algún Patrón de Diseño**.
- Entrega de documentación detallada del minijuego.

### Puntos adicionales (hasta nota 10)
- Implementación de **GAS** (Gameplay Ability System).
- Validación de punteros, manejo de errores, control de estados.
- Uso correcto de **Unreal Coding Standards**, buena legibilidad del código, evitando lógica y elementos no utilizados.

---

## Notas finales

- Cada estudiante desarrolla un minijuego libre.
- Se aprueba implementando los requisitos mínimos para el 4. Los puntos extra permiten llegar hasta 10.
- Se valorará bastante la implementación en C++, uso correcto y legibilidad del código, y originalidad del juego.
