#include "tp_maps_ui/widgets/Slider.h"
#include "tp_maps_ui/DrawHelper.h"

#include "tp_maps/MouseEvent.h"

#include "tp_utils/TimeUtils.h"

#include "glm/gtx/transform.hpp" // IWYU pragma: keep

#include <memory>

namespace tp_maps_ui
{

//##################################################################################################
struct Slider::Private
{
  TP_REF_COUNT_OBJECTS("tp_maps_ui::Slider::Private");
  TP_NONCOPYABLE(Private);
  Private() = default;

  //This represents the button state, normal, hover, presses, etc...
  VisualModifier currentVisualModifier{VisualModifier::Normal};

  int64_t min{0};
  int64_t max{100};
  int64_t value{50};

  int64_t margin{16};
  int64_t handleWidth{32};//{14};
  int64_t trackHeight{6};
};

//##################################################################################################
Slider::Slider(Widget* parent):
  Widget(parent),
  d(new Private())
{

}

//##################################################################################################
Slider::~Slider()
{
  delete d;
}

//##################################################################################################
std::pair<Dim, Dim> Slider::sizeHint() const
{
  return {{0.0f, 1.0f}, {12.0f, 0.0f}};
}

//##################################################################################################
void Slider::setRange(int64_t min, int64_t max)
{
  d->min = min;
  d->max = tpMax(min, max);
  setValue(d->value);
}

//##################################################################################################
void Slider::setValue(int64_t value)
{
  d->value = tpBound(d->min, value, d->max);
  update();
}

//##################################################################################################
int64_t Slider::min() const
{
  return d->min;
}

//##################################################################################################
int64_t Slider::max() const
{
  return d->max;
}

//##################################################################################################
int64_t Slider::value() const
{
  return d->value;
}

//##################################################################################################
void Slider::render(tp_maps::RenderInfo& renderInfo)
{
  TP_FUNCTION_TIME("Slider::render");

  TP_UNUSED(renderInfo);

  auto m = matrix();

  if(drawHelper())
  {
    //Draw the button background.
    //drawHelper()->drawBox(m, width(), height(), BoxType::Raised, FillType::Button, d->currentVisualModifier);
    {
      int trackLength = int(std::lround(width())) - (2*int(d->margin));

      int64_t range = d->max - d->min;

      float f = float(d->value-d->min) / float(range);

      {
        auto mm = glm::translate(m, {float(d->margin), float(height()-d->trackHeight)/2.0f, 0.0f});
        drawHelper()->drawBox(mm, float(trackLength), float(d->trackHeight), BoxType::Raised, FillType::Editable, VisualModifier::Pressed);
      }

      {
        glm::vec2 size = drawHelper()->recommendedSize(FillType::Slider);

        if(size.x<1.0f)size.x = 14.0f;
        if(size.y<1.0f)size.y = 14.0f;

        int xPos = int(std::lround(float(trackLength)*f)) + int(d->margin)-(int(size.x)/2);
        int yPos = (int(height()) - int(size.y))/2;

        auto mm = glm::translate(m, {float(xPos), float(yPos), 0.0f});
        drawHelper()->drawBox(mm, size.x, size.y, BoxType::Raised, FillType::Slider, d->currentVisualModifier);
      }
    }
  }

}

//##################################################################################################
void Slider::invalidateBuffers()
{
  Widget::invalidateBuffers();
}

//##################################################################################################
bool Slider::mouseEvent(const tp_maps::MouseEvent& event)
{
  switch(event.type)
  {
  case tp_maps::MouseEventType::Press:
  {
    d->currentVisualModifier = VisualModifier::Pressed;
    focus();
    update();
    return true;
  }

  case tp_maps::MouseEventType::Move:
  {
    if(d->currentVisualModifier == VisualModifier::Pressed)
    {
      int trackPosition = event.pos.x-(int(screenX())+int(d->margin));
      int trackLength = int(std::lround(width())) - (2*int(d->margin));

      int64_t range = d->max - d->min;
      float f = float(trackPosition)/float(trackLength);

      auto v = tpBound(d->min, d->min + int64_t(f*float(range)), d->max);
      if(v != d->value)
      {
        d->value = v;
        valueChanged();
      }

      update();
      return true;
    }
    break;
  }

  case tp_maps::MouseEventType::Release:
  {
    if(d->currentVisualModifier == VisualModifier::Pressed)
    {
      d->currentVisualModifier = VisualModifier::Normal;

      update();
      return true;
    }
    break;
  }

  case tp_maps::MouseEventType::Wheel:
  {
    break;
  }

  case tp_maps::MouseEventType::DoubleClick:
  {
    break;
  }

  case tp_maps::MouseEventType::Click:
  {
    break;
  }

  case tp_maps::MouseEventType::DragStart:
  {
    break;
  }
  }

  return Widget::mouseEvent(event);
}

//##################################################################################################
void Slider::animate(double timestampMS)
{
  Widget::animate(timestampMS);
}

}
