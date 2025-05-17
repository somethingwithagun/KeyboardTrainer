#include "wordinput.h"


WordInput::WordInput(){}

void WordInput::update(sf::Vector2u appWindowSize) {
	this->appWindowSize = appWindowSize;

	if(!letters.empty()){
		letters.back()->setOrigin(letters.back()->getGlobalBounds().width/2,letters.back()->getGlobalBounds().height/2);
		letters.back()->setPosition(letterPlaces[letters.size()-1]->getPosition().x, 300);
	}
}

void WordInput::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  for (int i = 0; i < letterPlaces.size(); i++)
    target.draw(*letterPlaces[i]);
  for (int i = 0; i < letters.size(); i++)
    target.draw(*letters[i]);
}

bool WordInput::appendLetter(char letter) {
	if(letters.size() + 1 <= letterPlaces.size()) {
  	letters.push_back(new sf::Text(sf::String(letter), font));
	
		if(letter == word[letters.size() - 1])
			letters.back()->setFillColor(sf::Color::White);
		else
			letters.back()->setFillColor(sf::Color::Red);

	}
	
	return (letter == word[letters.size() - 1]);
}

void WordInput::removeLetter() {
	if(!letters.empty()) {
		delete letters.back();
		letters.pop_back();
	}
}

void WordInput::clear() {
	cleanVector(letters);
	cleanVector(letterPlaces);
}

bool WordInput::isFull() {
  return getActualLettersCount() == getExpectedLettersCount();
}

char WordInput::getActualLetter(int index) {
	if(index < letters.size() && index >= 0)
  	return letters.at(index)->getString()[0];

	return '\0';
}

char WordInput::getExpectedLetter(int index)
{
  if(index < word.getSize() && index >= 0)
  	return word[index];

	return '\0';
}

int WordInput::getActualLettersCount() {
  return letters.size();
}

int WordInput::getExpectedLettersCount() {
  return word.getSize();
}

void WordInput::setWord(const sf::String& word) {
	this->word = word;
	
	int word_container_width = word.getSize()*(100 +m_charSpacing) - m_charSpacing;
	int offset = appWindowSize.x/2 - word_container_width/2 + 50;
	for(int i = 0; i < word.getSize(); i++) {
			letterPlaces.push_back(new sf::RectangleShape(sf::Vector2f(100,15)));
			letterPlaces[i]->setOrigin(letterPlaces[i]->getSize().x/2,letterPlaces[i]->getSize().y/2);
			letterPlaces[i]->setPosition(sf::Vector2f((m_charSpacing+100) * i + offset, 350));
	}
}

void WordInput::setFont(sf::Font font)
{
  this->font = font;
}

template<class T> void WordInput::cleanVector(std::vector<T*>& vec) {			
	while(!vec.empty()) {
    delete vec.back();
    vec.pop_back();
  }
}