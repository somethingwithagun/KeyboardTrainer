#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Constants.h"

#define DEFAULT_BACKGROUND_SIZE sf::Vector2f(220,30)
#define DEFAULT_CHARACTER_LIMIT 50

class TextInput : public sf::Drawable
{

public:
	TextInput();
	TextInput(const sf::Vector2f& bgsize, sf::Font& font, const int character_size = 16);
	~TextInput();

	enum TextAlign
	{
		Right = 0,
		Left = 1,
		Center = 2
	};

	void update(double deltaT);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/* SETTERS */

	void setPlaceholderText(const sf::String& text);

	void setAlign(TextAlign align);

	void setActive(bool active);

	void setFont(const sf::Font& font);

	void setBackgroundColor(const sf::Color& color);	

	void setTextColor(const sf::Color& color);

	void setCharacterSize(const unsigned int& size);

	void setSize(const sf::Vector2f& size);

	void setPosition(const sf::Vector2f& pos);

	void setText(const sf::String& text);

	void setCharactersLimit(const unsigned int limit);

	/* GETTERS */

	const int getCharactersLimit() const;

	const sf::FloatRect getGlobalBounds() const;

	const sf::Vector2f getSize() const;

	const int getCharacterSize() const;

	const sf::String getText() const;

	const sf::String& getPlaceholderText() const;

	const sf::Color& getTextColor() const;

	const sf::Color& getBackgroundColor() const;

	const bool getActive() const;

	const TextAlign& getAlign() const;
private:

	sf::Font font;

	sf::RectangleShape background;

	sf::Text label;

	sf::Text placeholderLabel;

	sf::String text;

	sf::String placeholderText;

	bool active = false;

	double counter = 0.0;

	TextAlign alignment = Center;

	int charactersLimit = DEFAULT_CHARACTER_LIMIT;

};

