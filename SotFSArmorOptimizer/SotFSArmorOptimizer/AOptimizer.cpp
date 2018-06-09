#include <array>
#include "AOptimizer.hpp"

void Optimizer::TryInsert(ArmorSet && set)
{
  const size_t stat = set.getStat(settings.optimizeFor);
  auto && begin = optimal.begin();
  auto && end = optimal.end();
  while (begin != end)
  {
    if (!begin->first ||
      stat >
      begin->second.getStat(settings.optimizeFor))
    {
      std::rotate(begin, end - 1, end);
      begin->first = true;
      begin->second = set;
      return;
    }

    ++begin;
  }
}

bool Optimizer::meetsConstraints(ArmorSet const & set) const
{
  if (set.getWeight() > available)
    return false;

  for (size_t i = 0; i != DEFENSE_COUNT; ++i)
  {
    if (set.getStat(static_cast<DefenseType>(i)) < settings.constraints[i].min ||
        set.getStat(static_cast<DefenseType>(i)) > settings.constraints[i].max)
      return false;
  }

  return true;
}

Optimizer::Optimizer(AOSettings const & _settings)
  : settings{_settings},
  available{ settings.max_burden * settings.equipload - settings.build_weight }
{
  optimal.fill(std::make_pair(false, ArmorSet{}));
}

Optimizer::Optimizer(AOSettings && _settings)
  : settings{std::move(_settings)}, 
  available{ settings.max_burden * settings.equipload - settings.build_weight }
{
  optimal.fill(std::make_pair(false, ArmorSet{}));

}

bool Optimizer::Optimize()
{
  if (settings.optimizeFor == DefenseType::lowest_elemental)
    elemental = true;
  if (available <= 0)
    return false;

  // get rid of armors our stats will not allow
  // note to self: move to outside of function
  for (size_t i = 0; i != 4; ++i)
  {
    std::vector<Armor> & Gear = settings.AllowedGear[i];
    // remove-erase idiom
    Gear.erase ( 
      std::remove_if(Gear.begin(), Gear.end(), 
        [&](Armor const& armor) {
      return !armor.meetsRequirements(settings.stats)
        || armor.weight > available;
      }),
      Gear.end()
    );
  }

  ArmorSet currentSet;
  for (auto && headgear : settings.AllowedGear[0])
    for (auto && chestpiece : settings.AllowedGear[1])
    {
      for (auto && gauntlets : settings.AllowedGear[2])
      {
        for (auto && leggings : settings.AllowedGear[3])
        {
          currentSet = ArmorSet{ { &headgear },{ &chestpiece },{ &gauntlets },{ &leggings } };
          if (meetsConstraints(currentSet))
            TryInsert(std::move(currentSet));
        }
      }
    }


  return optimal[0].first;
}

OptimalArmors const & Optimizer::getArmor() const
{
  return optimal;
}
