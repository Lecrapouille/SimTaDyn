#ifndef KEYBOARDCOMMANDS_HPP
#define KEYBOARDCOMMANDS_HPP

class MapEditorWindow;

enum class CameraDirection { Up, Down, Left, Right };

class MapEditorCommand
{
public:
  virtual ~MapEditorCommand() {}
  virtual void execute(MapEditorWindow& editor) = 0;
};

class MoveCameraCommand: public MapEditorCommand
{
public:

  MoveCameraCommand(CameraDirection const direction)
    : m_direction(direction)
  {}

  virtual void execute(MapEditorWindow& editor) override;

private:

  CameraDirection m_direction;
};

class SplitScreenCommand: public MapEditorCommand
{
public:

  SplitScreenCommand(Gtk::Orientation const orientation)
    : m_oritentation(orientation)
  {}

  virtual void execute(MapEditorWindow& editor) override;

private:

  Gtk::Orientation const m_oritentation;
};

#endif
