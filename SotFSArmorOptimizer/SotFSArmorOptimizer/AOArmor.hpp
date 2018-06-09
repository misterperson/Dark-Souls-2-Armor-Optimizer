#ifndef AO_ARMOR_HPP
#define AO_ARMOR_HPP

#include <array>
#include <string>
#include "AODefenseType.hpp"
#include "AOStats.hpp"

struct Armor
{
  // http://darksouls2.wikidot.com/head ordering
  std::string name;
  size_t id;

  std::array<size_t, DefenseType::DEFENSE_COUNT> defenses;

  float weight;

  std::array<size_t, Stats::STATS_COUNT> requirements;

  bool meetsRequirements(std::array<int, Stats::STATS_COUNT> const& stats) const;

};

struct ArmorSet
{
  Armor * head;
  Armor * body;
  Armor * arms;
  Armor * legs;

  float getWeight() const;
  size_t getStat(DefenseType stat) const;

  Armor const*const* begin() const;
  Armor const*const* end() const;
};

Armor const& getNothing();
Armor const& getDefault();

#endif //AO_ARMOR_HPP 