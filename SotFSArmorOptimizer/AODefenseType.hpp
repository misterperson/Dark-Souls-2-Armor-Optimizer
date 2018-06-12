#ifndef POISE_BOIS
#define POISE_BOIS

enum DefenseType
{
  // POISE BOIS
  poise = 0,

  // physical
  physical,
  strike,
  slash,
  thrust,

  // elemental
  magic,
  fire,
  lightning,
  dark,

  // status
  poison,
  bleed,
  petrification,
  curse,

  // bookkeeping
  DEFENSE_COUNT,

  // for optimizer
  lowest_elemental = DEFENSE_COUNT
};

namespace defenseType
{
  constexpr const char * strings[DEFENSE_COUNT]
  {
    "PSE",

    "PHY",
    "STK",
    "SLS",
    "THR",

    "MAG",
    "FIR",
    "LHT",
    "DRK",

    "PSO",
    "BLE",
    "PET",
    "CRS"
  };

  constexpr const char * stringsVerbose[DEFENSE_COUNT]
  {
    "Poise",

    "Physical",
    "Strike",
    "Slash",
    "Thrust",

    "Magic",
    "Fire",
    "Lightning",
    "Dark",

    "Poison",
    "Bleed",
    "Petrification",
    "Curse"
  };
}



#endif // POISE_BOIS