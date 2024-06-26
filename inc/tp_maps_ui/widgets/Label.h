#ifndef tp_maps_ui_Label_h
#define tp_maps_ui_Label_h

#include "tp_maps_ui/Widget.h"

namespace tp_maps_ui
{

//##################################################################################################
class TP_MAPS_UI_SHARED_EXPORT Label : public Widget
{
  TP_DQ;
public:
  //################################################################################################
  Label(Widget* parent=nullptr);

  //################################################################################################
  Label(const std::u16string& text, Widget* parent=nullptr);

  //################################################################################################
  ~Label() override;

  //################################################################################################
  const std::u16string& text() const;

  //################################################################################################
  void setText(const std::u16string& text);

  //################################################################################################
  void setTextNoUpdate(const std::u16string& text);

  //################################################################################################
  void setHAlignment(HAlignment alignment);

  //################################################################################################
  void setColor(const glm::vec4& color);

  //################################################################################################
  std::pair<Dim, Dim> sizeHint() const override;

protected:
  //################################################################################################
  void render(tp_maps::RenderInfo& renderInfo) override;

  //################################################################################################
  void invalidateBuffers() override;

  //################################################################################################
  bool mouseEvent(const tp_maps::MouseEvent& event) override;

  //################################################################################################
  void animate(double timestampMS) override;
};
}
#endif

