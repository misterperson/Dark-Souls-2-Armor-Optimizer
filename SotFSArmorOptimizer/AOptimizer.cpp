#include <array>
#include <thread>
#include <numeric>
#include <future>
#include "AOptimizer.hpp"

static OptimalArmors const& getEmpty()
{
  static OptimalArmors armor;

  static bool init = true;
  if (init)
  {
    for (size_t i = 0; i != 10; ++i)
      armor.emplace_back(false, ArmorSet{}, 0);
    init = false;
  }

  return armor;
}


struct ThreadSettings
{
  std::array < Constraint, DefenseType::DEFENSE_COUNT> constraints;
  std::array<size_t, DEFENSE_COUNT> baseDef;

  std::array<std::vector<size_t>, 4> allowed_ids;

  DefenseType optimizeFor;

  ThreadSettings(AOSettings const& settings) :
    optimizeFor{ settings.optimizeFor }
  {
    std::copy(
      settings.constraints.cbegin(), settings.constraints.cend(),
      constraints.begin()
    );

    std::copy(
      settings.baseDef.cbegin(), settings.baseDef.cend(),
      baseDef.begin()
    );

    for (size_t i = 0; i != 4; ++i)
    {
      allowed_ids[i].resize(settings.AllowedGear[i].size());
      std::iota(allowed_ids[i].begin(), allowed_ids[i].end(), size_t{ 0 });
    }
  }
};

static void TryInsert(ArmorSet && set, AOSettings const& settings, OptimalArmors & optimal)
{
  size_t stat = set.getStat(settings.optimizeFor, settings.baseDef);
  auto && begin = optimal.begin();
  auto && end = optimal.end();
  while (begin != end)
  {
    if (!begin->first || stat > begin->third)
    {
      optimal.emplace(begin, true, set, stat);
      optimal.pop_back();
      return;
    }

    ++begin;
  }
}

static void ThreadTryInsert(ArmorSet && set, ThreadSettings const& settings, OptimalArmors & optimal)
{
  size_t stat = set.getStat(settings.optimizeFor, settings.baseDef);
  auto && begin = optimal.begin();
  auto && end = optimal.end();
  while (begin != end)
  {
    if (!begin->first || stat > begin->third)
    {
      optimal.emplace(begin, true, set, stat);
      optimal.pop_back();
      return;
    }



    ++begin;
  }
}

#if 0
static bool meetsConstraints(ArmorSet const & set, AOSettings const& settings, float available)
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
#endif

static bool ThreadMeetsConstraints(ArmorSet const & set, ThreadSettings const& settings, float available)
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
  : settings{ _settings }, optimal{ getEmpty() },
  available{ settings.max_burden * settings.equipload - settings.build_weight }
{
}

Optimizer::Optimizer(AOSettings && _settings)
  : settings{ _settings }, optimal{ getEmpty() },
  available{ settings.max_burden * settings.equipload - settings.build_weight }
{
}

bool Optimizer::Optimize()
{
  if (available <= 0)
    return false;

  // get rid of armors our stats will not allow
  // note to self: move to outside of function

  for (size_t i = 0; i != 4; ++i)
  {
    std::vector<Armor> & Gear = settings.AllowedGear[i];
    // remove-erase idiom
    Gear.erase(
      std::remove_if(Gear.begin(), Gear.end(),
        [&](Armor const& armor) {
      return !armor.meetsRequirements(settings.stats)
        || armor.weight > available;
    }),
      Gear.end()
      );
  }


  AOSettings const& _settings = settings;
  size_t largest = [&]
  {
    size_t largest = 0;
    size_t size = std::numeric_limits<size_t>::min();
    for (size_t i = 0; i != 4; ++i)
    {
      if (_settings.AllowedGear[i].size() > size)
      {
        largest = i;
        size = _settings.AllowedGear[i].size();
      }
    }
    return largest;
  }();

  std::vector<Armor> toDivide = std::move(settings.AllowedGear[largest]);
  settings.AllowedGear[largest].resize(0);

#ifdef _DEBUG
  unsigned thread_count = 1;
#else
  unsigned thread_count = std::thread::hardware_concurrency();
#endif
  std::vector<std::thread> threads; 
  threads.reserve(thread_count);

  std::vector<ThreadSettings> threadSettings;
  threadSettings.reserve(thread_count);

  std::vector<std::future<OptimalArmors>> sets;
  sets.reserve(thread_count);

  for (unsigned i = 0; i != thread_count; ++i)
  {
    threadSettings.emplace_back(settings);
    threadSettings.back().allowed_ids[largest].reserve((size_t)std::ceil((double)toDivide.size() / (double)thread_count));
  }

  for (unsigned i = 0; i != toDivide.size(); ++i)
  {
    threadSettings[i % thread_count].allowed_ids[largest].emplace_back(i);
  }

  settings.AllowedGear[largest] = std::move(toDivide);

  auto && findBest = [&globalSettings = _settings](ThreadSettings & settings, float available, std::promise<OptimalArmors> promise)
  {
    OptimalArmors sets = getEmpty();

    ArmorSet currentSet;
    for (auto && headgear : settings.allowed_ids[0])
      for (auto && chestpiece : settings.allowed_ids[1])
      {
        for (auto && gauntlets : settings.allowed_ids[2])
        {
          for (auto && leggings : settings.allowed_ids[3])
          {
            currentSet = ArmorSet{
             { &globalSettings.AllowedGear[0][headgear] },
            { &globalSettings.AllowedGear[1][chestpiece] },
            { &globalSettings.AllowedGear[2][gauntlets] },
            { &globalSettings.AllowedGear[3][leggings] } };

            if (ThreadMeetsConstraints(currentSet, settings, available))
              ThreadTryInsert(std::move(currentSet), settings, sets);
          }
        }
      }
    promise.set_value(sets);
  };

  int i = 0;
  for (auto && threadSetting : threadSettings)
  {
    std::promise<OptimalArmors> set;
    sets.emplace_back(set.get_future());
    threads.emplace_back(std::thread{ findBest, std::reference_wrapper<ThreadSettings>(threadSetting), available, std::move(set) });
    ++i;
  }



  for (auto && thread : threads)
  {
    thread.join();
  }

  std::vector<OptimalArmors> optimalSets;
  optimalSets.reserve(thread_count);
  for (auto && set : sets)
  {
    set.wait();
    optimalSets.emplace_back(set.get());
  }

  for (auto && optimalSet : optimalSets)
  {
    for (auto && set : optimalSet)
    {
      if (!set.first) break;
      TryInsert(std::move(set.second), settings, optimal);
    }
  }


  return optimal.front().first;
}

OptimalArmors const & Optimizer::getArmor() const
{
  return optimal;
}
