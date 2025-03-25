#ifndef SONGREQUESTDIALOG_H_
#define SONGREQUESTDIALOG_H_

#include "DialogBase.h"
#include "TextInput.h"

class RequestSongDialog : public DialogBase {
public:
  RequestSongDialog(const sf::String& text, const sf::Vector2f background_size, const sf::Vector2f buttons_size, const sf::Color buttons_color,const sf::Font& font);
  ~RequestSongDialog();

  sf::String getAuthorName();
  sf::String getSongName();

  void symEntered(char c);
  void symErased();

  virtual void update(const float& dt) override;
	virtual void render(sf::RenderTarget* target) override;

  virtual void setPosition(const sf::Vector2f& pos);
private:
  TextInput* m_authorInput = nullptr;
  TextInput* m_songInput = nullptr;
};

#endif