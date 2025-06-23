#include "TextInput.h"

TextInput::TextInput() : TextInput(DEFAULT_BACKGROUND_SIZE, font, 16)
{
	//sf::Font font = new sf:
}

TextInput::TextInput(const sf::Vector2f& bgsize, sf::Font& font, const int character_size) : font(font)
{
	background.setSize(bgsize);
	background.setFillColor(sf::Color::White);
	background.setOrigin(background.getSize().x / 2, background.getSize().y / 2);

	label.setCharacterSize(character_size);
	label.setFillColor(sf::Color::Black);
	label.setFont(font);

	placeholderLabel.setCharacterSize(character_size);
	placeholderLabel.setFont(font);
	placeholderLabel.setFillColor(sf::Color(label.getFillColor().r, label.getFillColor().g, label.getFillColor().b, label.getFillColor().a + 40));
	placeholderLabel.setStyle(sf::Text::Italic);
}

TextInput::~TextInput() {
}

const sf::String& TextInput::getPlaceholderText() const
{
	return placeholderText;
}

void TextInput::update(double deltaT)
{

	switch (alignment)
	{
	case TextInput::Left:
		label.setOrigin((background.getSize().x / 2) - 5, (background.getSize().y / 2) - label.getGlobalBounds().height / 4);
		placeholderLabel.setOrigin((background.getSize().x / 2) - 5, (background.getSize().y / 2) - placeholderLabel.getGlobalBounds().height / 4);
		break;
	case TextInput::Center:
		label.setOrigin(label.getGlobalBounds().width / 2, (label.getGlobalBounds().height / 2) + label.getGlobalBounds().height / 4);
		placeholderLabel.setOrigin(placeholderLabel.getGlobalBounds().width / 2, (placeholderLabel.getGlobalBounds().height / 2) + label.getGlobalBounds().height / 4);
		break;
	case TextInput::Right:
		label.setOrigin((label.getGlobalBounds().width - background.getSize().x / 2) + 5, (label.getGlobalBounds().height / 2) + label.getGlobalBounds().height / 4);
		placeholderLabel.setOrigin((placeholderLabel.getGlobalBounds().width - background.getSize().x / 2) + 5, (placeholderLabel.getGlobalBounds().height / 2) + label.getGlobalBounds().height / 4);
		break;
	default:
		break;
	}

	if (text.getSize() > 0 && !active && text[text.getSize() - 1] == '_') {
		text.erase(text.getSize() - 1, 1);
		this->setText(text);
	}
	else if (text.getSize() > 0 && active && text[text.getSize() - 1] != '_')
		this->setText(text);

	if (counter > 0.06)
	{

	}

	counter += deltaT;
}

void TextInput::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(background);	

	if (text.getSize())
		target.draw(label);
	else if (placeholderText.getSize())
		target.draw(placeholderLabel);
}

void TextInput::setPlaceholderText(const sf::String& newText) {
	if (newText.getSize() > charactersLimit)
	{
		std::cerr << "Max characters limit reached!\n";
		return;
	}

	placeholderText = newText;

	placeholderLabel.setString(placeholderText);
}

void TextInput::setAlign(TextAlign align)
{
	alignment = align;
}

void TextInput::setActive(bool active)
{
	this->active = active;
}

void TextInput::setFont(const sf::Font& font)
{
	this->font = font;
	label.setFont(font);
	placeholderLabel.setFont(font);
}

void TextInput::setBackgroundColor(const sf::Color& color)
{
	background.setFillColor(color);
}

void TextInput::setTextColor(const sf::Color& color)
{
	label.setFillColor(color);
	placeholderLabel.setFillColor(sf::Color(label.getFillColor().r, label.getFillColor().g, label.getFillColor().b, label.getFillColor().a + 40));
}

void TextInput::setCharacterSize(const unsigned int& size)
{
	label.setCharacterSize(size);
	placeholderLabel.setCharacterSize(size);
}

void TextInput::setSize(const sf::Vector2f& size)
{
	background.setSize(size);
}

void TextInput::setPosition(const sf::Vector2f& pos)
{
	background.setPosition(pos);
	label.setPosition(background.getPosition());
	placeholderLabel.setPosition(background.getPosition());
}

void TextInput::setText(const sf::String& newText)
{
	if (newText.getSize() > charactersLimit)
	{
		std::cerr << "Max characters limit reached!\n";
		return;
	}

	if (active)
		this->text = newText + '_';

	label.setString(this->text);
}

void TextInput::setCharactersLimit(const unsigned int limit)
{
	charactersLimit = limit;
}

const int TextInput::getCharactersLimit() const
{
	return charactersLimit;
}

const sf::FloatRect TextInput::getGlobalBounds() const
{
	return background.getGlobalBounds();
}

const sf::Vector2f TextInput::getSize() const
{
	return background.getSize();
}

const int TextInput::getCharacterSize() const
{
	return label.getCharacterSize();
}

const sf::String TextInput::getText() const {
	if (text.getSize() > 0 && active && text[text.getSize() - 1] == '_') {
		sf::String newText = text;
		newText.erase(text.getSize() - 1, 1);
		return newText;
	}

	return text;
}

const sf::Color& TextInput::getTextColor() const
{
	return label.getFillColor();
}

const sf::Color& TextInput::getBackgroundColor() const
{
	return background.getFillColor();
}

const bool TextInput::getActive() const
{
	return active;
}

const TextInput::TextAlign& TextInput::getAlign() const
{
	return alignment;
}
