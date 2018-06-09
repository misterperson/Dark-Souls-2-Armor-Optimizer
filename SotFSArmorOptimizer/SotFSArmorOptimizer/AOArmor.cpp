#include "AOArmor.hpp"

Armor const & getNothing()
{
  static bool init = true;
  static Armor none;

  if (init)
  {
    none.name = "N/A";
    none.defenses.fill(0);
    none.weight = 0.0f;
    none.requirements.fill(0);

    init = false;
  }

  return none;
}

Armor const& getDefault()
{
  static bool init = true;
  static Armor naked;

  if (init)
  {
    naked.name = "Naked";
    naked.defenses.fill(0);
    naked.weight = 0.0f;
    naked.requirements.fill(0);

    init = false;
  }

  return naked;
}

float ArmorSet::getWeight() const
{
  return head->weight + body->weight + arms->weight + legs->weight;
}

size_t ArmorSet::getStat(DefenseType stat) const
{

  if (stat == DefenseType::lowest_elemental)
  {
    std::array<size_t, 4> elementalDef;
    elementalDef.fill(0);

    int i = DefenseType::magic;
    for (auto && def : elementalDef)
    {
      for (auto && gear : *this)
        def += gear->defenses[i];
      ++i;
    }

    return *std::min(elementalDef.cbegin(), elementalDef.cend());
  }
  else
  {
    size_t total = 0;
    for (auto && gear : *this)
      total += gear->defenses[stat];
    return total;
  }
}

bool Armor::meetsRequirements(std::array<int, Stats::STATS_COUNT> const & stats) const
{
  size_t i = 0;
  for (auto && stat : stats)
    if (stat < requirements[i++])
      return false;
  return true;
}

Armor const *const* ArmorSet::begin() const
{
  return &head;
}

Armor const *const* ArmorSet::end() const
{
  return &legs + 1;
}
