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
#include "wordinput.h"

class Engine {
public:
  Engine(int fps = 60);
  ~Engine();

  void run();

  void init();
  
private:
  void handleDialogKeybinds(const sf::Event& e);
  void handleMainKeybinds(const sf::Event& e);
  void handleKeybinds(const sf::Event& e);
  void handleMouseClick(const sf::Vector2f& mouseClickPosition);

  void updateDialogs();
  void updateButtons();
  void update();

  void render();


  void freeDialog(DialogBase* dialog);
  void initVariables();
  
  std::string getRandomWord();

  std::string getNextWord();

  float calculateStats(const int mistakes, const int letters) const;
  std::string ftostr(float x) const;

  std::string getCurrentTime();

  void setWordToDisplay(sf::String word);
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

  int currentWordIndex;

	sf::Clock dtClock;
	sf::Clock timer;

  sf::RenderWindow* window = nullptr;
  int fps;
  float dt;
  bool isQuit = false;
  bool mouseDown;
  bool isGameOver;
  bool isAnyDialogsOpen;
  
  sf::String currentTranslatedWord;

	sf::Text wordLabel;
  WordInput wordInput;
	sf::Text translatedWordLabel;
	sf::Text mistakesLabel;
	sf::Text correctLabel;

  sf::Color buttonColor = sf::Color(151, 137, 139);

  Button* exitButton = nullptr;
  Button* clearRatingButton = nullptr;
  Button* requestLyricsButton = nullptr;
  Button* loadLyricsButton = nullptr;

  Button* translateWordButton = nullptr;
  bool translatePressed;
  
  RequestSongDialog* requestLyricsDialog = nullptr;
  LoadLyricsDialog* loadLyricsDialog = nullptr;

  PlotWidget<sf::String> plot;

	int mistakes = 0;
	int summaryChars = 0;
  int m_tryRating;

  std::fstream ratingFile;

  sf::Font font;

  std::vector<std::string> words;
  std::vector<sf::String> translatedWords;
};

#endif