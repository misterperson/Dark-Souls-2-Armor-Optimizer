#include <iostream>
#include <fstream>
#include <algorithm>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include "AOException.hpp"
#include "AOptimizer.hpp"
#include "AOParser.hpp"

template<typename T>
static T GetMember(rapidjson::Value & value, const char * name, T def)
{
  return value.HasMember(name) ? value[name].Get<T>() : def;
}

static void AOStart(Parser && _parser, AOSettings && _settings);
static void TextInput(const char * path);
static void UserInput(void);

int main(int argc, char *argv[])
try
{
  TextInput("Input.json");

  std::cout << "Press ENTER to continue . . .";
  getc(stdin);
  return 0;
}
catch (AOException err)
{
  std::cout << err.what() << '\n';
  std::cout << "Press ENTER to continue . . .";
  getc(stdin);
  return 2;
}



static void TextInput(const char * path)
try
{
  Parser parser;
  AOSettings settings;
  
  std::string buf = [&]
  {
    std::ifstream file{ path, std::ios::ate };
    std::streamsize size = file.tellg();
    std::string buf;
    buf.resize(static_cast<size_t>(size));
    file.seekg(0, std::ios::beg);
    file.read(&buf[0], size);
    return buf;
  }();
  rapidjson::StringStream ss{ buf.c_str() };
  rapidjson::Document doc;
  doc.ParseStream(ss);

  if (!doc.IsObject())
    throw AOException{ "Error reading file", AOException::AOError::badfile };

  settings.equipload    = GetMember(doc, "Max Burden", 0.0f);
  settings.build_weight = GetMember(doc, "Weight Without Armor", 0.0f);
  settings.max_burden   = GetMember(doc, "Target Equipload", 70.0f);

  if (doc.HasMember("Stats"))
  {
    auto && stats = doc["Stats"];
    settings.stats[Strength]     = GetMember(stats, "Strength",     3); // Sorcerer
    settings.stats[Dexterity]    = GetMember(stats, "Dexterity",    5); // Cleric
    settings.stats[Intelligence] = GetMember(stats, "Intelligence", 1); // Bandit
    settings.stats[Faith]        = GetMember(stats, "Faith",        4); // Sorcerer
  }
  else
  {
    settings.stats[Strength]     = 3;
    settings.stats[Dexterity]    = 5;
    settings.stats[Intelligence] = 1;
    settings.stats[Faith]        = 4;
  }

  // load constraints
  if (doc.HasMember("Constraints"))
  {
    auto && constraints = doc["Constraints"];
    for (size_t i = 0; i != DefenseType::DEFENSE_COUNT; ++i)
    {
      if (constraints.HasMember(defenseType::stringsVerbose[i])
        && constraints[defenseType::stringsVerbose[i]].IsArray()
        && constraints[defenseType::stringsVerbose[i]].Size() == 2)
      {
        auto && vector2 = constraints[defenseType::stringsVerbose[i]];
        settings.constraints[i] = Constraint{vector2[0].GetUint(), vector2[1].GetUint()};
      }
      else
      {
        settings.constraints[i] = Constraint{ 0, 9999 };
      }
    }
  }

  settings.optimizeFor = static_cast<DefenseType>(GetMember(doc, "Optimize For", 0));




  AOStart(std::move(parser), std::move(settings));
}
catch (AOException)
{
  throw;
}

static void UserInput(void)
try
{
  Parser parser;
  AOSettings settings;

  std::cout << "Enter your maximum equipload\n> ";
  std::cin >> settings.equipload;

  std::cout << "Enter your current equipload (without armor)\n> ";
  std::cin >> settings.build_weight;

  std::cout << "Enter your target percentage equipload (as a decimal, from 0 to 1)\n> ";
  std::cin >> settings.max_burden;

  std::cout << "Enter your Strength\n> ";
  std::cin >> settings.stats[Strength];

  std::cout << "Enter your Dexterity\n> ";
  std::cin >> settings.stats[Dexterity];

  std::cout << "Enter your Intelligence\n> ";
  std::cin >> settings.stats[Intelligence];

  std::cout << "Enter your Faith\n> ";
  std::cin >> settings.stats[Faith];


  size_t optimizerfor;
  do
  {
    std::cout << "Enter the stat to optimize for\n"
      << "0 = Poise, 1 = Physical, 2 = Strike, 3 = Slash, 4 = Thrust\n"
      << "5 = Magic, 6 = Fire, 7 = Lightning, 8 = Dark\n"
      << "9 = Poison, 10 = Bleed, 11 = Petrification, 12 = Curse\n"
      << "> ";
    std::cin >> optimizerfor;
  } while (optimizerfor < 0 || optimizerfor >= DefenseType::DEFENSE_COUNT);

  settings.optimizeFor = static_cast<DefenseType>(optimizerfor);
  settings.constraints.fill(Constraint{ std::numeric_limits<size_t>::min(), std::numeric_limits<size_t>::max() });

  AOStart(std::move(parser), std::move(settings));
}
catch (AOException)
{
  throw;
}

static void AOStart(Parser && _parser, AOSettings && _settings)
{
  Parser parser       = _parser;
  AOSettings settings = _settings;
  DefenseType optimizerfor = settings.optimizeFor;

  settings.AllowedGear[0] = parser.moveHead();
  settings.AllowedGear[1] = parser.moveBody();
  settings.AllowedGear[2] = parser.moveArms();
  settings.AllowedGear[3] = parser.moveLegs();

  for (size_t i = 0; i < 4; ++i)
    settings.AllowedGear[i].emplace_back(getDefault());

  std::cout << "Working. . . ";

  Optimizer optimizer{ std::move(settings) };

  bool result = optimizer.Optimize();

  std::cout << "Done\n";

  if (!result)
  {
    std::cout << "No valid armor combination was found\n";
    return;
  }

  size_t count = 1;
  for (auto && set : optimizer.getArmor())
  {
    if (set.first == false)
      break;

    std::cout << count++ << ") ";

    auto && armorSet = set.second;
    std::cout << armorSet.head->name << ", "
      << armorSet.body->name << ", "
      << armorSet.arms->name << ", "
      << armorSet.legs->name << '\n';

    std::cout << "Weight: " << armorSet.getWeight() << '\n';

    for (size_t i = 0; i < DefenseType::DEFENSE_COUNT; ++i)
    {
      if (i == optimizerfor)
        std::cout << '>';

      std::cout << defenseType::strings[i] << ": "
        << armorSet.getStat(static_cast<DefenseType>(i));

      if (i == optimizerfor)
        std::cout << '<';
      std::cout << '\n';
    }

    std::cout << '\n';
  }
}