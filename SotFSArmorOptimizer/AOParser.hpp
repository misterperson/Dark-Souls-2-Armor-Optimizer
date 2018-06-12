#ifndef AO_PARSER_HPP
#define AO_PARSER_HPP

#include <string>
#include <vector>
#include "AOArmor.hpp"

// fwd decl
class Parser
{
private:
  std::vector<Armor> head;
  std::vector<Armor> body;
  std::vector<Armor> arms;
  std::vector<Armor> legs;

  std::vector<Armor> LoadFile(const char *filename);
  

  public:

  // ctor/dtor
  Parser();
  ~Parser() noexcept = default;

  // copy
  Parser(Parser const&) = default;
  Parser & operator=(Parser const&) = default;
  
  // move
  Parser(Parser &&) noexcept = default;
  Parser & operator=(Parser &&) noexcept = default;

  std::vector<Armor> const& getHead() const noexcept;
  std::vector<Armor> const& getBody() const noexcept;
  std::vector<Armor> const& getArms() const noexcept;
  std::vector<Armor> const& getLegs() const noexcept;

  std::vector<Armor> moveHead() noexcept;
  std::vector<Armor> moveBody() noexcept;
  std::vector<Armor> moveArms() noexcept;
  std::vector<Armor> moveLegs() noexcept;

  // potentially unsafe
  std::vector<Armor> moveSlot(int i) noexcept;
  std::vector<Armor> const& getSlot(int i) noexcept;
}; 
#endif