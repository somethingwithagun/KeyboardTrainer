#include "engine.h"
#include "wordinput.h"

Engine::Engine(int fps) : fps(fps) {
  if(!font.loadFromFile(NOTOSANS_FONT_PATH))
    std::cerr << "Engine::Engine(int): Could not open font" << std::endl;
	else {
		exitButton = new Button(sf::Vector2f(50,50),"X",font,RED);
		clearRatingButton = new Button(sf::Vector2f(150,50),"Clear Rating",font,buttonColor);
		requestLyricsButton = new Button(sf::Vector2f(150,50),"Request Song",font,buttonColor);
		loadLyricsButton = new Button(sf::Vector2f(150,50),"Load Lyrics",font,buttonColor);
		translateWordButton = new Button(sf::Vector2f(150,50),"Translate",font,buttonColor);

		wordLabel = sf::Text("",font);  
		wordInput.setFont(font);
		translatedWordLabel = sf::Text("",font);  
		mistakesLabel = sf::Text("0.00%",font);
		mistakesLabel.setFillColor(RED);
		mistakesLabel.setOrigin(mistakesLabel.getGlobalBounds().width/2,mistakesLabel.getGlobalBounds().height/2);

		correctLabel = sf::Text("0.00%",font);
		correctLabel.setFillColor(GREEN);
	}
	
	plot.setSize(sf::Vector2f(1000,400));
	plot.setFont(font);
  plot.setOrigin(plot.getSize().x/2,plot.getSize().y/2);
}

Engine::~Engine() {	
  delete window;
	if(exitButton)
		delete exitButton;

	if(clearRatingButton)
		delete clearRatingButton;
	
	if(requestLyricsButton)
		delete requestLyricsButton;
	
	if(loadLyricsButton)
		delete loadLyricsButton;
	
	if(translateWordButton)
		delete translateWordButton;
}

void Engine::init() {
  window = new sf::RenderWindow(sf::VideoMode(1920,1080,32), "Keyboard Trainer controller", sf::Style::Default | sf::Style::Fullscreen);
  window->setFramerateLimit(fps);
	
	if(exitButton)
		exitButton->setPosition(sf::Vector2f(window->getSize().x-25, 25));
	
	if(clearRatingButton)
		clearRatingButton->setPosition(sf::Vector2f(window->getSize().x/2 - 160-80, window->getSize().y-clearRatingButton->getSize().y/2));
	
	if(requestLyricsButton)
		requestLyricsButton->setPosition(sf::Vector2f(window->getSize().x/2 - 80, window->getSize().y-requestLyricsButton->getSize().y/2));
	
	if(loadLyricsButton)
		loadLyricsButton->setPosition(sf::Vector2f(window->getSize().x/2 + 80, window->getSize().y-loadLyricsButton->getSize().y/2));
	
	if(translateWordButton) {
		translateWordButton->setPosition(sf::Vector2f(window->getSize().x/2 +160 + 80, window->getSize().y-translateWordButton->getSize().y/2));
		translateWordButton->setText("Translate");
		translateWordButton->setFillColor(buttonColor);
	}

  mistakesLabel.setPosition(window->getSize().x/2 + 150, 100);
  correctLabel.setPosition(window->getSize().x/2 - 150, 100);
	
	translatedWordLabel.setOrigin(translatedWordLabel.getGlobalBounds().width/2,translatedWordLabel.getGlobalBounds().height/2);
	translatedWordLabel.setPosition(window->getSize().x/2, 425);

  plot.setPosition(window->getSize().x/2, window->getSize().y/2 + 200);
  plot.loadFromFile(RATING_TXT_PATH);


	initVariables();

}

void Engine::handleDialogKeybinds(const sf::Event& e) {
	if(requestLyricsDialog && requestLyricsDialog->getIsOpen()){					
		if(e.text.unicode == BACKSPACE_CODE)
			requestLyricsDialog->symErased();
		else if(e.text.unicode >= SPACE_CODE && e.text.unicode < 128)
			requestLyricsDialog->symEntered(e.text.unicode);
	} else if(loadLyricsDialog && loadLyricsDialog->getIsOpen()){
		
		if(e.text.unicode == BACKSPACE_CODE)
			loadLyricsDialog->symErased();
		else if(e.text.unicode >= SPACE_CODE && e.text.unicode < 128)
			loadLyricsDialog->symEntered(e.text.unicode);
	}
}

void Engine::handleMainKeybinds(const sf::Event& e) {
	if(e.text.unicode == BACKSPACE_CODE) {
		wordInput.removeLetter();
	} else if(e.text.unicode == ENTER_CODE || e.text.unicode == SPACE_CODE) {
		if(isGameOver) {
			initVariables();
			isGameOver = false;
		}else if(wordInput.isFull() && !words.empty()) {
			float stat = calculateStats(mistakes,summaryChars);

			mistakesLabel.setString(ftostr(stat)+"%");
			mistakesLabel.setOrigin(mistakesLabel.getGlobalBounds().width/2,mistakesLabel.getGlobalBounds().height/2);

			correctLabel.setString(ftostr(100.f - stat)+"%");
			correctLabel.setOrigin(correctLabel.getGlobalBounds().width/2,correctLabel.getGlobalBounds().height/2);

			
			translatedWordLabel.setString("");
			translateWordButton->setText("Translate");
			translateWordButton->setFillColor(buttonColor);
			translatePressed = false;
			
			m_tryRating += round(float(summaryChars-mistakes)/(timer.restart().asSeconds()*10)); // TODO: improve formula

			currentWordIndex++;
			if(currentWordIndex < words.size())
				setWordToDisplay(words[currentWordIndex]);
			else {
				std::cout << "*** SONG ENDED ***\n";
				std::cout << "Rating: "<< m_tryRating << '\n';
				std::cout << "Symbols written: "<< summaryChars << '\n';
				std::cout << "Errors: " << mistakes << '\n' << std::flush;

				std::string curTime = getCurrentTime();
				plot.apply(curTime,m_tryRating);
				writeRating(curTime,m_tryRating);

				translatedWordLabel.setString("");
				wordLabel.setString("Song ended! Press Enter to restart");
				wordLabel.setOrigin(wordLabel.getGlobalBounds().width/2,wordLabel.getGlobalBounds().height/2);
				wordLabel.setPosition(window->getSize().x/2, 200);

				wordInput.clear();

				isGameOver = true;
			}
		}

	}
	else if(e.text.unicode > SPACE_CODE && e.text.unicode < 128) {
		if(!wordInput.appendLetter(e.text.unicode)) 
			mistakes++;

		wordInput.update(window->getSize());
	}
}

void Engine::handleKeybinds(const sf::Event& e) {
	if(isAnyDialogsOpen)
		handleDialogKeybinds(e);	
	else if(!isAnyDialogsOpen)
		handleMainKeybinds(e);
}

void Engine::handleMouseClick(const sf::Vector2f& mouseClickPosition) {
	if (exitButton->getGlobalBounds().contains(mouseClickPosition)) {
		if (exitButton->isReady()) {
			exitButton->setPressed(true);
			isQuit = true;
			window->close();
		}
	} else if(clearRatingButton->getGlobalBounds().contains(mouseClickPosition)) {
		if(clearRatingButton->isReady()) {
			clearRatingButton->setPressed(true);
			
			std::ofstream ratingFile(RATING_TXT_PATH);
			if(ratingFile.is_open()){
				std::cout << "Cleared rating at " << getCurrentTime() << std::endl;
				ratingFile.close();

				plot.reset();
			}

		}
	} else if(requestLyricsButton->getGlobalBounds().contains(mouseClickPosition)) {
		if(requestLyricsButton->isReady()) {
			requestLyricsButton->setPressed(true);
			
			requestLyricsDialog = new RequestSongDialog("Request Song", sf::Vector2f(600,400),sf::Vector2f(150,50),sf::Color(151, 137, 139),font);
			requestLyricsDialog->setPosition(sf::Vector2f(window->getSize().x/2,window->getSize().y/2));
			requestLyricsDialog->show();
			isAnyDialogsOpen = true;

		}
	} else if(loadLyricsButton->getGlobalBounds().contains(mouseClickPosition)) {
			if(loadLyricsButton->isReady()) {
				loadLyricsButton->setPressed(true);
				
				// open dialog with author and song name text fields
				loadLyricsDialog = new LoadLyricsDialog("Load Lyrics", sf::Vector2f(600,400),sf::Vector2f(150,50),sf::Color(151, 137, 139),font);
				loadLyricsDialog->setPosition(sf::Vector2f(window->getSize().x/2,window->getSize().y/2));
				loadLyricsDialog->show();
				isAnyDialogsOpen = true;
			}
	} else if(translateWordButton->getGlobalBounds().contains(mouseClickPosition)) {
		if(translateWordButton->isReady() && !isGameOver && !words.empty()) {
			if(!translatePressed) {
				translateWordButton->setPressed(true);
				
				// change button state
				translateWordButton->setFillColor(RED);
				translateWordButton->setText("Hide");

				currentTranslatedWord = translateWord(words[currentWordIndex]);
				translatedWordLabel.setString(currentTranslatedWord);
				translatedWordLabel.setOrigin(translatedWordLabel.getGlobalBounds().width/2,translatedWordLabel.getGlobalBounds().height/2);
				translatedWordLabel.setPosition(window->getSize().x/2, 425);

				translatePressed = true;
			} else {
				translateWordButton->setPressed(true);
				
				// change button state
				translateWordButton->setFillColor(buttonColor);
				translateWordButton->setText("Translate");

				currentTranslatedWord = "";
				translatedWordLabel.setString(currentTranslatedWord);
				translatePressed = false;
			}
		}
	}
}

void Engine::updateDialogs() {
	if(requestLyricsDialog && requestLyricsDialog->getIsOpen()) {
		requestLyricsDialog->update(dt);
		DialogBase::Result res = requestLyricsDialog->getResult();
		if(res == DialogBase::Accepted) {
			std::string author = requestLyricsDialog->getAuthorName().toAnsiString(); /* get author name from text field */
			std::string songname = requestLyricsDialog->getSongName().toAnsiString(); /* get song name from text field */

			if(author.empty() || songname.empty())
				std::cerr << "author or songname cannot be empty" << std::endl;
			else {
				AzLyricsParser parser;
				words = parser.getLyricsList(author, songname);

				initVariables();
			}
		}
		if(res != DialogBase::None) {
			freeDialog(requestLyricsDialog);
			requestLyricsDialog = nullptr;
		}
	
	} else if(loadLyricsDialog && loadLyricsDialog->getIsOpen()) {
		loadLyricsDialog->update(dt);
		DialogBase::Result res = loadLyricsDialog->getResult();
		if(res == DialogBase::Accepted) {
			std::string filepath = loadLyricsDialog->getPath().toAnsiString(); /* get song name from text field */
			if(filepath.empty())
				std::cerr << "cannot read the empty path" << std::endl;
			else{
				loadLyrics(filepath);
				initVariables();
			}
		}
		if(res != DialogBase::None){
			freeDialog(loadLyricsDialog);
			loadLyricsDialog = nullptr;
		}
	}
}

void Engine::updateButtons() {
	exitButton->update(dt);
	clearRatingButton->update(dt);
	requestLyricsButton->update(dt);
	loadLyricsButton->update(dt);
	translateWordButton->update(dt);
}

void Engine::freeDialog(DialogBase* dialog) {
	dialog->close();
	delete dialog;
	isAnyDialogsOpen = false;
}

void Engine::initVariables() {
  dt = 0.f;
	mouseDown = false;
  currentWordIndex = 0;
	m_tryRating = 0;
	summaryChars = 0;
	mistakes = 0;
	m_tryRating = 0;
	isAnyDialogsOpen = false;
	isGameOver = false;
	translatePressed = false;

	float stat = calculateStats(mistakes,summaryChars);

	mistakesLabel.setString(ftostr(stat)+"%");
	mistakesLabel.setOrigin(mistakesLabel.getGlobalBounds().width/2,mistakesLabel.getGlobalBounds().height/2);

	correctLabel.setString(ftostr(100.f - stat)+"%");
	correctLabel.setOrigin(correctLabel.getGlobalBounds().width/2,correctLabel.getGlobalBounds().height/2);

	currentTranslatedWord = "";
	
	if(!words.empty())
		setWordToDisplay(words[currentWordIndex]);

	dtClock.restart();
	timer.restart();
}

void Engine::run() {
  while(window->isOpen()) {
		sf::Event e;
		while(window->pollEvent(e)) {
			switch(e.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::TextEntered:
				if(isAnyDialogsOpen)
					handleDialogKeybinds(e);	
				else if(!isAnyDialogsOpen)
					handleMainKeybinds(e);

				break;
			case sf::Event::MouseButtonReleased:
				if(!isAnyDialogsOpen)
					handleMouseClick(static_cast<sf::Vector2f>(sf::Mouse::getPosition()));

				break;
			}
		}
		
		updateDialogs();
		updateButtons();

		render();

		dt = dtClock.restart().asSeconds();
	}

	wordInput.clear();
}

void Engine::render() {
	window->clear(sf::Color(26, 26, 26));
	window->draw(wordLabel);
	window->draw(wordInput);

	window->draw(translatedWordLabel);

	window->draw(mistakesLabel);
	window->draw(correctLabel);
	window->draw(plot);
	window->draw(*exitButton);
	window->draw(*clearRatingButton);
	window->draw(*requestLyricsButton);
	window->draw(*loadLyricsButton);
	window->draw(*translateWordButton);


	if(requestLyricsDialog)
		requestLyricsDialog->render(window);

	if(loadLyricsDialog)
		loadLyricsDialog->render(window);

	window->display();
}

void Engine::loadLyrics(std::string path) {
	
	words.clear();

	std::ifstream in(path);

	if(!in.is_open()){
		std::cout << "Unable to open file" << std::endl;
		return;
	}

	std::string word;
	while(in >> word)
		words.push_back(word);

	in.close();

}

float Engine::calculateStats(const int mistakes, const int letters) const {
	if(letters == 0)
		return 0;
  float num = (float(mistakes)/float(letters)) * 100;
	return num;	
}

std::string Engine::ftostr(float x) const {
	if(x < 0.1)
		return "0.00";
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << x;
	return stream.str();
}

std::string Engine::getRandomWord() {
	srand(time(0));
	return words[rand() % (words.size())];
}

std::string Engine::getCurrentTime() {
	time_t t = time(0);
	tm* t1 = localtime(&t);
	int h = t1->tm_hour,
			m = t1->tm_min,
			s = t1->tm_sec;
	return ((h<10?"0":"") + std::to_string(h) + ':' +(m<10?"0":"") + std::to_string(m) + ':' + (s<10?"0":"")+ std::to_string(s));
}

std::string Engine::getNextWord() {
	return (words.size() - 1 < currentWordIndex && !words.empty()) ? "" : words[currentWordIndex + 1];
}

void Engine::setWordToDisplay(sf::String word) {
	wordLabel.setString(word);
	wordLabel.setOrigin(wordLabel.getGlobalBounds().width/2,wordLabel.getGlobalBounds().height/2);
	wordLabel.setPosition(window->getSize().x/2, 200);
	
	wordInput.clear();
	wordInput.update(window->getSize());
	wordInput.setWord(word);

	summaryChars += word.getSize();
}

sf::String Engine::translateWord(std::string word) {
	std::ofstream tout("transtmp.txt");
	tout << word;
	tout.close();

	/* compatibility turned off */
	system("python translate.py");
	std::ifstream tin("transtmp.txt");
	std::string a;
	tin >> a;
	tin.close();
	return decodeTranslatedWord(a);
}

sf::String Engine::decodeTranslatedWord(std::string word) {
	sf::String res;
	for(int i = 0; i < word.size(); i++)
		if(toRus.count(word[i]) == 0)
			res += word[i];
		else res += toRus[word[i]];

	return res;
}

template<class T> void Engine::freeVector(std::vector<T*>& vec) {			
	while(!vec.empty()) {
    delete vec.back();
    vec.pop_back();
  }
}

bool Engine::writeRating(std::string time, float rating) {
	ratingFile.open(RATING_TXT_PATH, std::ios_base::out | std::ios_base::app);
	if(!ratingFile.is_open()) {
		std::cerr << "Engine::writeRating(std::string, float): error in opening rating file" << std::endl;
		return false;
	}
	ratingFile << rating << ' ' << time << '\n';
	ratingFile.close();
	return true;
}
