#ifndef AO_OPTIMIZER_HPP
#define AO_OPTIMIZER_HPP

#include <vector>
#include <array>
#include <list>
#include "AOArmor.hpp"

struct Armor;

struct Constraint
{
  size_t min;
  size_t max;
};

struct AOSettings
{
  float equipload;    // the equipload of the player (based on vit)

  float build_weight; // weight of other items

  float max_burden;   // percentage of equipload, from 0 to 1 inclusive

  std::array<int, Stats::STATS_COUNT> stats; // used for armor requirements

  std::array<size_t, DefenseType::DEFENSE_COUNT> baseDef;

  // QoL feature: only accept armor sets that meet the minimum defenses desired
  std::array<Constraint, DefenseType::DEFENSE_COUNT> constraints;

  DefenseType optimizeFor;          // basic stat to optimize for after meeting constraints & stat reqs
  std::array<std::vector<Armor>, 4> AllowedGear; // QoL Feature: force use these armors for the algorithm
};

// maybe replace with dynarray and allow end-user to configure
// valid armor set + armor set
template<typename T1, typename T2, typename T3> 
struct triplet
{
  triplet(T1 const& _first, T2 const& _second, T3 const& _third)
    : first{ _first }, second{ _second }, third{ _third } {}

  T1 first;
  T2 second;
  T3 third;
};

template<typename T1, typename T2, typename T3>
triplet<T1,T2,T3> make_triplet(T1 _first, T2 _second, T3 _third)
{
  return triplet < T1, T2, T3 >(_first, _second, _third);
}




typedef std::list<triplet<bool, ArmorSet, size_t>> OptimalArmors;
class Optimizer
{
private:
  AOSettings settings;
  OptimalArmors optimal;
  float available;

  //void TryInsert(ArmorSet && set);
  //bool meetsConstraints(ArmorSet const& set) const;
  //bool elemental{ false };
public:
  Optimizer(AOSettings const& _settings);
  Optimizer(AOSettings && _settings);

  bool Optimize();
  
  OptimalArmors const& getArmor() const;
};

#endif //AO_OPTIMIZER_HPP