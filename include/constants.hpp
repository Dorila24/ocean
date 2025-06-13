#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

/* ── отображение ── */
constexpr float CELL     = 8.f;
constexpr int   FIELD_W  = 120;
constexpr int   FIELD_H  = 80;
constexpr int   SAND_ROWS = 5;

/* водоросли: максимум 20 клеток над песком */
constexpr int   MAX_ALGAE = 20;

/* ── симуляция ── */
constexpr int   TICK_MS           = 300;
constexpr int   ALGAE_GROW_CHANCE = 10;
constexpr int   HUNGRY_HERB       = 20;
constexpr int   HUNGRY_PRED       = 25;

/* ── размножение ── */
constexpr int   BIRTH_HERB = 30;
constexpr int   BIRTH_PRED = 20;

/* ── старение ── */
constexpr int AGE_LIMIT = 200;
constexpr int MAX_POP   = 10;

/* ── краб ── */
constexpr int CRAB_PERIOD_MS = 10000;

#endif
