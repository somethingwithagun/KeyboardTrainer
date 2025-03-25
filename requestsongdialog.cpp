#include "requestsongdialog.h"

RequestSongDialog::RequestSongDialog(const sf::String& text, const sf::Vector2f background_size,
  const sf::Vector2f buttons_size, const sf::Color buttons_color, const sf::Font& font) : DialogBase(background_size,buttons_size,buttons_color,font) {
  m_text.setString(text);
  m_textRelPos = {m_text.getGlobalBounds().width/2,175};
  m_text.setPosition(m_background.getPosition()-m_textRelPos);

  sf::Vector2f textInputSize = {background_size.x - 40,50};
  m_authorInput = new TextInput(textInputSize,m_font);
  m_authorInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/4));
  m_authorInput->setPlaceholderText("Author");

  m_songInput = new TextInput(textInputSize,m_font);
  m_songInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/2));
  m_songInput->setPlaceholderText("Song");
}

RequestSongDialog::~RequestSongDialog() {
  if(m_authorInput)
    delete m_authorInput;
   if(m_songInput)
    delete m_songInput; 
}

sf::String RequestSongDialog::getAuthorName() {
  if(m_authorInput)
    return m_authorInput->getText();
  // else
  return sf::String(); 
}

sf::String RequestSongDialog::getSongName() {
  if(m_songInput)
    return m_songInput->getText();
  // else
  return sf::String(); 
}

void RequestSongDialog::symEntered(char c) {
  if(m_authorInput->getActive())
    m_authorInput->setText(m_authorInput->getText()+c);  
  else if(m_songInput->getActive())
    m_songInput->setText(m_songInput->getText()+c);  
}

void RequestSongDialog::symErased() {
  if(m_authorInput->getActive())
    m_authorInput->setText(m_authorInput->getText().substring(0,m_authorInput->getText().getSize()-1));  
  else if(m_songInput->getActive())
    m_songInput->setText(m_songInput->getText().substring(0,m_songInput->getText().getSize()-1));
}

void RequestSongDialog::update(const float& dt) {
  DialogBase::update(dt);

  m_authorInput->update(dt);
  m_songInput->update(dt);

  if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if(m_authorInput->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))){
      m_authorInput->setActive(true);
      m_songInput->setActive(false);
    }
    else if(m_songInput->getGlobalBounds().contains(static_cast<sf::Vector2f>(sf::Mouse::getPosition()))) {
      m_songInput->setActive(true);
      m_authorInput->setActive(false);
    }
    
  }
}

void RequestSongDialog::render(sf::RenderTarget* target) {
  DialogBase::render(target);
  
  m_authorInput->render(target);
  m_songInput->render(target);
}

void RequestSongDialog::setPosition(const sf::Vector2f& pos) {
  DialogBase::setPosition(pos);
  sf::Vector2f background_size = m_background.getSize();
  m_authorInput->setPosition({m_background.getPosition().x, m_background.getPosition().y - 75});
  m_songInput->setPosition({m_background.getPosition().x, m_background.getPosition().y + 25});
  // m_songInput->setPosition(m_background.getPosition() + sf::Vector2f(background_size.x/2, background_size.y/2));  
}
