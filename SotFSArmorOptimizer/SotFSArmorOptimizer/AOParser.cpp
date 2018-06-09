#include <iostream>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>
#include "AOException.hpp"
#include "AOParser.hpp"

static Armor ParseArmor(rapidjson::Value & value);
template<typename T>
static T GetMember(rapidjson::Value & value, const char * name, T def)
{
  return value.HasMember(name) ? value[name].Get<T>() : def;
}

//template<>
//static const char * GetMember(rapidjson::Value & value, const char * name, const char * def)
//{
//  return value.HasMember(name) ? value[name].GetString() : def;
//}

std::vector<Armor> Parser::LoadFile(const char * filename)
{
  std::vector<Armor> Armors;
  std::string file = 
    [&] {
    std::ifstream infile{ filename, std::ios::ate };
    if(!infile)
      throw AOException{ "Error reading file", AOException::AOError::badfile };
    std::string buf;
    std::streamsize size = infile.tellg();
    buf.resize(static_cast<size_t>(size));
    infile.seekg(0, std::ios::beg);
    infile.read(&buf[0], size);
    return buf;
  }();
  rapidjson::Document document;
  rapidjson::StringStream ss{file.c_str()};
  document.ParseStream(ss);

  if (!document.IsObject() || !document.HasMember("Gear"))
    throw AOException{ "Error reading file", AOException::AOError::badfile };
  
  auto && Gear = document["Gear"];

  if (!Gear.IsArray())
    throw AOException{ "Error reading file", AOException::AOError::badfile };
  
  try
  {
    Armors.reserve(static_cast<size_t>(Gear.GetArray().Size()));
  }
  catch (std::bad_alloc)
  {
    throw AOException{ "Error allocating memory", AOException::AOError::badalloc };
  }

  for (auto && armor : Gear.GetArray())
    Armors.emplace_back(ParseArmor(armor));

  return Armors;
}



// load everything from file
Parser::Parser()
{
  std::cout << "Loading gear. . .";
  
  try
  {
    head = LoadFile("Head.json");
    body = LoadFile("Body.json");
    arms = LoadFile("Arms.json");
    legs = LoadFile("Legs.json");
  }
  catch (AOException)
  {
    std::cout << " error loading!\n";
    throw;
  }

  std::cout << " done\n";
}

std::vector<Armor> const & Parser::getHead() const noexcept
{
  return head;
}

std::vector<Armor> const & Parser::getBody() const noexcept
{
  return body;
}

std::vector<Armor> const & Parser::getArms() const noexcept
{
  return arms;
}

std::vector<Armor> const & Parser::getLegs() const noexcept
{
  return legs;
}

std::vector<Armor> Parser::moveHead() noexcept
{
  std::vector<Armor> blank;
  blank.swap(head);
  return blank;
}

std::vector<Armor> Parser::moveBody() noexcept
{
  std::vector<Armor> blank;
  blank.swap(body);
  return blank;
}

std::vector<Armor> Parser::moveArms() noexcept
{
  std::vector<Armor> blank;
  blank.swap(arms);
  return blank;
}

std::vector<Armor> Parser::moveLegs() noexcept
{
  std::vector<Armor> blank;
  blank.swap(legs);
  return blank;
}

std::vector<Armor> Parser::moveSlot(int i) noexcept
{
  std::vector<Armor> blank;
  (&head + i)->swap(blank);
  return blank;
}

std::vector<Armor> const & Parser::getSlot(int i) noexcept
{
  return *(&head + i);
}


static Armor ParseArmor(rapidjson::Value & value)
{
  Armor armor;
  armor.name = GetMember(value, "Name", "");

  armor.id = GetMember<size_t>(value, "ItemID", 8484); // default = error id

  for (size_t i = 0; i != DefenseType::DEFENSE_COUNT; ++i)
    armor.defenses[i] = GetMember<size_t>(value, defenseType::stringsVerbose[i], 0);

  armor.weight = GetMember(value, "Weight", 0.0f);

  armor.requirements[Strength] = GetMember<size_t>(value, "Strength", 0);
  armor.requirements[Dexterity] = GetMember<size_t>(value, "Dexterity", 0);
  armor.requirements[Intelligence] = GetMember<size_t>(value, "Intelligence", 0);
  armor.requirements[Faith] = GetMember<size_t>(value, "Faith", 0);

  return armor;
}