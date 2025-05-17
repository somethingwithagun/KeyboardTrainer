#ifndef WORDINPUT_H_
#define WORDINPUT_H_

#include <vector>
#include <SFML/Graphics.hpp>

class WordInput : public sf::Transformable, public sf::Drawable {

public:
  explicit WordInput();

  void update(sf::Vector2u appWindowSize);
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  bool appendLetter(char letter);
  void removeLetter();

  void clear();

  bool isFull();

  char getActualLetter(int index);
  char getExpectedLetter(int index);
  int getActualLettersCount();
  int getExpectedLettersCount();

  void setWord(const sf::String& word);
  void setFont(sf::Font font);
  
private:
  template<class T> void cleanVector(std::vector<T*>& vec);

  const int m_charSpacing = 20;
  sf::Vector2u appWindowSize;

  sf::Font font;
  sf::String word;

  std::vector<sf::RectangleShape*> letterPlaces;
	std::vector<sf::Text*> letters;
};

#endif