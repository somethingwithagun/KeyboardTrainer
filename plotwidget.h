#ifndef PLOT_H_
#define PLOT_H_
#define DEBUG_


#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cassert>
#include <algorithm>

#include "drawfuncs.h"
#include "constants.h"

/*
  *** PlotWidget class declaration ***

 * This class allows to use plots in c++.
 * Template accepts X and Y axis types (only Y implemented by 03.08.24).
 * To load some data you can pass data using process(const std::vector<T,U>&) function.
 * Scaling options are not very flexible,
      by now you can only change font and size of the widget itself.
 * to draw window 

  Y VALUE TO RELATIVE Y PIXELS CONVERTATION FORMULAS: 
    * index * levelDifference -- line relative height in value
    * bgHeight - index * level_height - x_labels_offset -- line relative height in pixels 


    Plot should look like this

    ^ Y, value
    |
    |
    |
    0--------> X, value 

*/

template<class T>
class PlotWidget : public sf::Transformable, public sf::Drawable{
public:
  explicit PlotWidget();
  explicit PlotWidget(const sf::Vector2f& size);
  ~PlotWidget();
  
  void process(const std::vector<std::pair<T,float>>& passed_data);

  /// @brief Allows you to pass data for plot straight from file
  /// @param path Path to the source file
  /// @return Result of opening and reading the source file
  bool loadFromFile(const std::string& path);

  void reset();

  void apply(T x, float y);

  void draw(sf::RenderTarget& wnd, sf::RenderStates states) const;

  void update();
  
  // setters
  void setSize(const sf::Vector2f& size);
  const sf::Vector2f& getSize() const;

  void setFont(sf::Font font);
  
private:

  /// @brief If the value is not belong to the levels line then it should be somewhere in between two lines; 
  // this block of code is calculating the points y including its offset
  /// @param y Position of the point
  /// @return The exceeding of the y over the nearest level line height
  int findLevelExceed(const int& y);

  void rescale();

  void setRelativePosition(sf::Transformable* object) {
    sf::Vector2f pos = this->getPosition();
    sf::Vector2f origin = this->getOrigin();
    object->move(pos.x - origin.x - ylabelsOffset/2, pos.y - origin.y);
  }

  struct {
    sf::Color color;
    sf::PrimitiveType style;
    int size;
  } brush; 

  std::string toText(std::string str) {return str;}

  std::string toText(float num, int accuracy = 1) {
    std::stringstream ss;
    ss << std::fixed
      << std::setprecision(std::log(accuracy) / std::log(10)) /* base-10 logarithm*/
      << num;
    return ss.str();
  }

  int bgHeight;
  int bgWidth;
  
  int currentMx;
  int currentMn;

  std::vector<std::pair<T,int>> data;

  const int levelDifference = 1000;

  // this might be a bit confusing
  const int ylabelsOffset = 30; // this is the offset for the labels that go along the Y axis. So it moves graph along X axis right
  const int xLabelsOffset = 30; // this is the offset for the labels that go along the X axis. So it moves graph along Y axis up
  
  int maxDots;
  int currentDots;

  const int offsetX = 0; // left margin relatively to the background(that means that labels offsets are included too)
  int pointDistance = 90; // distance between the dots
  const int pointRadius = 3; // radius of the dot

  sf::Vector2f size;

  sf::Font font;

  std::vector <sf::Transformable*> objects;
  
  std::vector <sf::RectangleShape*> levelLines;
  std::vector <int> linesY;

  std::vector <sf::CircleShape*> points;
  std::unordered_map <int,int> pointsY; // value, position y

  sf::RectangleShape* background = nullptr;
};

template class PlotWidget<sf::String>;
template class PlotWidget<float>;

#endif