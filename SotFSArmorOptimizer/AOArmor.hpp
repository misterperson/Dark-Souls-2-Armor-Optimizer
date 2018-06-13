#ifndef AO_ARMOR_HPP
#define AO_ARMOR_HPP

#include <array>
#include <string>
#include "AODefenseType.hpp"
#include "AOStats.hpp"

typedef std::array<size_t, DefenseType::DEFENSE_COUNT> Defenses;
Defenses const& getNoDefense(void);

struct Armor
{
  // http://darksouls2.wikidot.com/head ordering
  std::string name;
  size_t id;

  Defenses defenses;

  float weight;

  std::array<size_t, Stats::STATS_COUNT> requirements;

  bool meetsRequirements(std::array<int, Stats::STATS_COUNT> const& stats) const;
  size_t getStat(DefenseType stat, Defenses const& stats = getNoDefense()) const;
};

struct ArmorSet
{
  Armor const* head;
  Armor const* body;
  Armor const* arms;
  Armor const* legs;

  float getWeight() const;
 
  size_t getStat(DefenseType stat, Defenses const& stats = getNoDefense()) const;

  Armor const*const* begin() const;
  Armor const*const* end() const;
};

Armor const& getNothing();
Armor const& getDefault();

#endif //AO_ARMOR_HPP 