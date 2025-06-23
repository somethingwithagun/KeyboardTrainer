#include "plotwidget.h"

template<class T>
PlotWidget<T>::PlotWidget(){
  currentMx = INT_MIN;
  currentMn = INT_MAX;

  brush.color = sf::Color::Black;
  brush.style = sf::LinesStrip;
  brush.size = 3;
  currentDots = 0;
}

template <class T>
PlotWidget<T>::PlotWidget(const sf::Vector2f& size) {
  setSize(size);

  PlotWidget();
}

template<class T>
PlotWidget<T>::~PlotWidget() {
  while(!objects.empty()) {
    delete objects.back();
    objects.pop_back();
  }
  while(!levelLines.empty()) {
    delete levelLines.back();
    levelLines.pop_back();
  }
  while(!points.empty()) {
    delete points.back();
    points.pop_back();
  }
  if(background != nullptr)
    delete background;
}

template<class T>
void PlotWidget<T>::process(const std::vector<std::pair<T,float>>& passed_data) {
  if(passed_data.empty()) {
    update();
    return;
  }
  reset();

  for(int i = 0; i < passed_data.size(); i++)
    apply(passed_data[i].first, passed_data[i].second);

  update();
}
template <class T>
bool PlotWidget<T>::loadFromFile(const std::string& path) {
  // assert(0 && "not implemented");
  bool success = true;
  std::ifstream in(path);
  if(in.is_open()) {
    
    std::string time;
    float rating;
    
    while(in >> rating >> time)
      apply(time, rating);
    
    in.close();
  } else success = false;
  update();
  return success;
}
template <>
bool PlotWidget<float>::loadFromFile(const std::string& path) {
  bool success = true;
  std::ifstream in(path);
  if(in.is_open()) {
    
    float time;
    float rating;
    
    while(in >> rating >> time)
      apply(time, rating);
    
    in.close();
  } else success = false;
  update();
  return success;
}
template <class T>
void PlotWidget<T>::reset() {
  data.clear();
  objects.clear();
  levelLines.clear();
  points.clear();
  linesY.clear();
  update();
}

template<class T>
int PlotWidget<T>::findLevelExceed(const int& y) {
  int level_exceed = y;
  while (level_exceed >= levelDifference) level_exceed -= levelDifference;

  return level_exceed;
}

template<class T>
void PlotWidget<T>::update() {
  brush.color = sf::Color::White;

  if(!background){
    background = create_rect(ylabelsOffset, -xLabelsOffset, bgWidth, bgHeight, brush.color);
    setRelativePosition(background);
  }

  rescale();
}

template <class T>
void PlotWidget<T>::rescale() {

  // Clear arrays with any objects
  points.clear();
  linesY.clear();
  levelLines.clear();
  objects.clear();

  if(data.empty()) {
    // leave 0(zero)
    brush.color = sf::Color(58, 61, 59,90);

    int y = bgHeight - xLabelsOffset;
    
    linesY.push_back(0); //not in pixels
    
    levelLines.push_back(create_line(ylabelsOffset,y,
          bgWidth + ylabelsOffset,y, brush.size, brush.color));
    setRelativePosition(levelLines.back());

    brush.color = sf::Color::White;

    // draw label
    objects.push_back(
      create_text(std::to_string(0),
        0, y, 16, &font, brush.color)
    );
    setRelativePosition(objects.back());
  }

  if(data.size() > 1)
    pointDistance = bgWidth/(data.size()-1);

  int mn = currentMn - findLevelExceed(currentMn);
  int lines_amount = (currentMx-currentMn)/levelDifference + 2;
  if(data.empty()) lines_amount = 1;
  int level_height = bgHeight/lines_amount;
  
  int i = 0;
  for(i = 0; i < lines_amount;i++) {
    brush.color = sf::Color(58, 61, 59,90);

    int y = bgHeight -(i*level_height) - xLabelsOffset;
    
    linesY.push_back(i*levelDifference+mn); //not in pixels
    
    levelLines.push_back(create_line(ylabelsOffset,y,
          bgWidth + ylabelsOffset,y, brush.size, brush.color));
    setRelativePosition(levelLines.back());

    brush.color = sf::Color::White;

    // draw labels
    objects.push_back(
      create_text(std::to_string(linesY[i]),
        0, y, 16, &font, brush.color)
    );
    setRelativePosition(objects.back());
  }

  int label_x = offsetX + ylabelsOffset;
  int brush_size = brush.size;
  brush.size = 1;
  
  for(i = 0; i<data.size();i++) {

    /* * * Draw Label by X * * */

    brush.color = sf::Color::White;
    sf::String label = toText(data[i].first);
    objects.push_back(create_text(label,label_x,bgHeight+8, 16, &font, brush.color));
    setRelativePosition(objects.back());
    label_x += pointDistance;

    /* * * Draw Points * * */

    int level_exceed = findLevelExceed(data[i].second), line_index = 0;

    auto line_iter = std::find(linesY.begin(),linesY.end(),data[i].second-level_exceed);    
    if(line_iter != linesY.end())
      line_index = line_iter - linesY.begin();

    int x,y;
    y = (bgHeight - line_index*level_height - xLabelsOffset) - (level_exceed*level_height/levelDifference); // some maths
    x = i*pointDistance;

    brush.color = sf::Color::Red;
    
    points.push_back(create_ellipse(x - pointRadius + offsetX + ylabelsOffset, 
         y - pointRadius,pointRadius*2,pointRadius*2, brush.color));
    setRelativePosition(points.back());
    pointsY[data[i].second] = y;

    if(i > 0) { // if it's not the first point we should connect two points with a line
      brush.color = sf::Color::Black;
      int prev_point_x = (i-1) * pointDistance;

      objects.push_back(create_line(
        prev_point_x + offsetX + ylabelsOffset, // x1 position in pixels
        pointsY[data[i-1].second], // y1 position in pixels
        x + offsetX + ylabelsOffset, // x2 position in pixels
        pointsY[data[i].second], // y2 position in pixels
        brush.size, // thickness
        brush.color // color
      ));

      setRelativePosition(objects.back());

      brush.color = sf::Color::Red;
    }
  }
  brush.size = brush_size;
}

template <class T>
void PlotWidget<T>::apply(T x, float y){
  data.push_back(std::pair<T, float>(x,y));

  currentMx = std::max(int(y),currentMx);
  currentMn = std::min(int(y),currentMn);

  std::sort(data.begin(),data.end(), [](const std::pair<T,int>& first, const std::pair<T,int>& second) {
    return (first.first < second.first);
  });

  rescale();
}

template <class T>
void PlotWidget<T>::draw(sf::RenderTarget& wnd, sf::RenderStates states) const {
  wnd.draw(*background);
  for (int i = 0; i < objects.size(); i++)
    wnd.draw(*dynamic_cast<sf::Drawable*>(objects[i]));
  for (int i = 0; i < levelLines.size(); i++)
    wnd.draw(*levelLines[i]);
  for (int i = 0; i < points.size(); i++)
    wnd.draw(*points[i]);
}

template<class T>
void PlotWidget<T>::setSize(const sf::Vector2f& size) {
  this->size = size;
  bgWidth  = size.x - ylabelsOffset;
  bgHeight = size.y - xLabelsOffset;
}

template<class T>
const sf::Vector2f& PlotWidget<T>::getSize() const {
  return size;
}

template<class T>
void PlotWidget<T>::setFont(sf::Font font) {
  this->font = font;
}