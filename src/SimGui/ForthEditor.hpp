#ifndef FORTHEDITOR_HPP_
#  define FORTHEDITOR_HPP_

#  include "TextEditor.hpp"
#  include "SimForth.hpp"
#  include <chrono>
#  include "Redirection.hpp"


class TextColorGTK: public TextColor
{
public:
  TextColorGTK(Glib::RefPtr<Gtk::TextBuffer> buf)
  {
    m_buf = buf;
    m_color = "normal";

    m_bold_tag = m_buf->create_tag("bold");
    m_bold_tag->set_property("weight", Pango::WEIGHT_BOLD);

    m_bold_tag = m_buf->create_tag("red");
    m_bold_tag->set_property("style", Pango::STYLE_ITALIC);
  }
  ~TextColorGTK() { };
  inline virtual TextColorGTK *clone() const override
  {
    return new TextColorGTK(*this);
  }
  inline void normal() override { m_color = "normal"; }
  inline void red() override { m_color = "red"; }
  inline void green() override { m_color = "bold"; }
  inline void blue() override { m_color = "red"; }
  inline void yellow() override { m_color = "bold"; }
  inline void azul() override { m_color = "red"; }
  inline void grey() override { m_color = "bold"; }
  inline void white() override { m_color = "red"; }

  inline std::ostream& operator<<(std::ostream& os)
  {
    std::stringstream ss;
    ss << os.rdbuf();

    std::cerr << "operateur<< insert_with_tag '" << m_color << "'" << ss.str() << std::endl;
    m_buf->insert_with_tag(m_buf->end(), ss.str(), m_color);
    return os;
  }

protected:
  inline void display(std::ostream &os) const override
  {
    (void) os;
  }

  Glib::RefPtr<Gtk::TextTag> m_bold_tag;
  Glib::RefPtr<Gtk::TextTag> m_red_tag;
  const char* m_color;
  Glib::RefPtr<Gtk::TextBuffer> m_buf;
};

// *************************************************************************************************
//
// *************************************************************************************************
class ForthEditor : public TextEditor
{
public:
  ForthEditor();
  ~ForthEditor();
  void empty();
  void templated();
  void exec1(const std::string &script);
  void exec();
  std::string elapsedTime();
  void loadDictionary();
  void dumpDictionary();

  Gtk::Notebook       m_res_notebooks; // FIXME: attention collision de noms TextEditor::m_notebook
  Gtk::ScrolledWindow m_scrolledwindow[4];
  Gtk::TextView       m_result;
  Gtk::TextView       m_history;
  Gtk::TextView       m_debug;
  Gtk::TreeView       m_dico;
  Gtk::Statusbar      m_statusbar;

  //SimForth* m_forth; // FIXME: utile si on gere plusieurs cartes

protected:
  class ModelColumns : public Gtk::TreeModelColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_word);
      add(m_token);
    }

    Gtk::TreeModelColumn<Glib::ustring> m_word;
    Gtk::TreeModelColumn<int> m_token;
  };

  ModelColumns m_columns;
  Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;
  std::chrono::nanoseconds m_elapsed_time;
  streamgui m_cout; // std::cout redirected inside the GUI
  streamgui m_cerr; // std::cerr redirected inside the GUI
  TextColorGTK m_color;
};

#endif /* FORTHEDITOR_HPP_ */
