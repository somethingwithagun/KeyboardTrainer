#ifndef ENGINE_H_
#define ENGINE_H_

#define BACKSPACE_CODE '\b'
#define SPACE_CODE ' '
#define ENTER_CODE '\r'
#define ESC_CODE 0x1B

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <unordered_map>

#include "button.h"
#include "radiobutton.h"
#include "plotwidget.h"
#include "constants.h"
#include "parser.h"
#include "requestsongdialog.h"
#include "loadlyricsdialog.h"

class Engine {
public:
  Engine(int fps = 60);
  ~Engine();

  void start();

private:
  void init();
  void initVariables();
  
  std::string setRandomWord();
  void nextWord();

  float calculateStats(const int mistakes, const int letters) const;
  std::string ftostr(float x) const;

  std::string currentTime();

  void changeWord();
  void loadLyrics(std::string path);
  sf::String translateWord(std::string word);
  sf::String decodeTranslatedWord(std::string word);

  bool writeRating(std::string time, float rating);

  template<class T> void freeVector(std::vector<T*>& vec);

  std::unordered_map<char, sf::String> toRus{
    {'a', L"а"}, {'b', L"б"}, {'c', L"ц"}, {'d', L"д"},
    {'e', L"е"}, {'f', L"ф"}, {'g', L"г"},
    {'h', L"х"}, {'i', L"и"}, {'j', L"ж"},{'k', L"к"},
    {'l', L"л"}, {'m', L"м"}, {'n', L"н"}, {'o', L"о"}, {'p', L"п"},
    {'q', L"п"}, {'r', L"р"}, {'s', L"с"}, {'t', L"т"},
    {'u', L"ю"}, {'v', L"в"}, {'w', L"ш"}, {'x', L"щ"},
    {'y', L"у"}, {'z', L"з"}, {'1', L"ь"}, {'2', L"ъ"},
    {'3', L"э"}, {'4', L"ч"}, {'5', L"я"}, {'6', L"ы"},
    {'7', L"й"},
  }; //symbol to rus

  int m_wordIndex;
  
  const int m_charSpacing = 20;

	sf::Clock m_dtClock;
	sf::Clock m_timer;

  sf::RenderWindow* m_app = nullptr;
  int m_fps;
  float m_dt;
  bool m_quit = false;
  float m_counterA;
  bool m_mouseDown;
  bool m_isGameOver;
  bool m_isAnyDialogsOpen;
  
  sf::String m_currentWord;
  sf::String m_currentTranslatedWord;
	sf::Text m_wordLabel;
	sf::Text m_translatedWordLabel;
	sf::Text m_mistakesLabel;
	sf::Text m_correctLabel;

  sf::Color m_buttonColor = sf::Color(151, 137, 139);

  Button* m_exitButton = nullptr;
  Button* m_clearRatingButton = nullptr;
  Button* m_requestLyricsButton = nullptr;
  Button* m_loadLyricsButton = nullptr;

  Button* m_translateWordButton = nullptr;
  bool m_translatePressed;
  
  RequestSongDialog* m_requestLyricsDialog = nullptr;
  LoadLyricsDialog* m_loadLyricsDialog = nullptr;

  PlotWidget<sf::String> m_plot;

	int mistakes = 0;
	int summaryChars = 0;
  int m_tryRating;

  std::fstream m_ratingFile;

  sf::Font m_font;

  std::vector<std::string> m_words;
  std::vector<sf::String> m_translatedWords;
  std::vector<sf::RectangleShape*> m_letterPlaces;
	std::vector<sf::Text*> m_letters;


};

#endif