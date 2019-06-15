//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef MAP_EXPLORER_HPP
#  define MAP_EXPLORER_HPP

#  include "ILoader.tpp"
#  include "SimTaDynMap.hpp"
#  include "Gtkmm.tpp"

// **************************************************************
//! \brief Show and edit the content (transform matrix) of a
//! SceneNode.  This window is displayed when the user clicks on a
//! scene node (= SimTaDynSheet) of a SimTaDynMap (= SceneGraph) in
//! the SimTaDynMapExplorer.
// **************************************************************
class SceneNodeEditor : public Gtk::Window // FIXME https://github.com/fra-zz-mer/RenderMasterEditor
{
private:

  enum LabelNames { LabelLayerName, LabelFilePath, LabelMapProjection,
                    LabelPosition, LabelRotation, LabelGlobScale, LabelLocalScale,
                    LabelAxis, LabelAxisX, LabelAxisY, LabelAxisZ, LabelAngle,
                    MaxLabels_ };
  enum EntryNames { EntryLayerName, EntryFilePath, EntryMapProjection,
                    EntryPositionX, EntryPositionY, EntryPositionZ,
                    EntryRotationX, EntryRotationY, EntryRotationZ, EntryRotationAng,
                    EntryGlobScaleX, EntryGlobScaleY, EntryGlobScaleZ,
                    EntryLocScaleX, EntryLocScaleY, EntryLocScaleZ,
                    EntryMatrix00, EntryMatrix01, EntryMatrix02, EntryMatrix03,
                    EntryMatrix10, EntryMatrix11, EntryMatrix12, EntryMatrix13,
                    EntryMatrix20, EntryMatrix21, EntryMatrix22, EntryMatrix23,
                    EntryMatrix30, EntryMatrix31, EntryMatrix32, EntryMatrix33,
                    MaxEntries_};

public:

  //-----------------------------------------------------------------
  //! \brief Create the GTK+ window.
  //-----------------------------------------------------------------
  SceneNodeEditor();

  //-----------------------------------------------------------------
  //! \brief Display the content of a SceneGraph node
  //-----------------------------------------------------------------
  void bind(SceneNodePtr node);

private:

  //-----------------------------------------------------------------
  //! \brief Get the float
  //-----------------------------------------------------------------
  float getFloat(EntryNames const it);

  //-----------------------------------------------------------------
  //! \brief Signal when the user has entered a value in the text entry.
  //! Check if the inserted text is a number.
  //-----------------------------------------------------------------
  bool entryKeyReleaseAmount(GdkEventKey* event, EntryNames const it);

  //-----------------------------------------------------------------
  //! \brief Signal when the user click on the "apply" button. Update
  //! the scene node from GTK+ widgets.
  //-----------------------------------------------------------------
  void apply();
  void cancel();

private:

  Gtk::Frame        m_frame[3];
  Gtk::Table        m_table[3];
  Gtk::Label        m_labels[LabelNames::MaxLabels_];
  Gtk::Entry        m_entries[EntryNames::MaxEntries_];
  Gtk::Button       m_apply;
  Gtk::Button       m_cancel;
  Gtk::Statusbar    m_statusbar;
  Gtk::VBox         m_vbox;
  Gtk::HBox         m_hbox[2];
  std::shared_ptr<SceneNode> m_node;
  SceneNode         m_backup_node;
};

// *************************************************************************************************
//! \brief GTK+ window displaying the content of a SimTaDynMap.
//! TODO limitation: affiche pour le moment une unique carte
// *************************************************************************************************
class SimTaDynMapExplorer : public Gtk::ScrolledWindow
{
private:

  // Shortname aliases
  using PixbufPtr = Glib::RefPtr<Gdk::Pixbuf>;
  using TreeIterator = Gtk::TreeModel::iterator;
  using TreeRow = Gtk::TreeModel::Row;

  // GTK+ treeview node categories
  enum Category { Folders, Maps, Sheets, Scripts, Resources, Index, MaxCategories_ };

  // FIXME: Creer un IconManager accessible par toutes les fenetres GTK+
  enum Icon { Ok, Ko, Warning, Graph, File, Folder, Sheet, Script, Resource, MaxIcons_ };

  // **************************************************************
  //! \brief GTK+ Tree model columns
  // **************************************************************
  class ModelColumns: public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    {
      // TODO: ajouter: editable, visible, runnable, graph direct/indirect
      add(category);
      add(icon_type);
      add(filename); // FIXME: ajouter fullpath (pour coller au ForthEditor)
      add(icon_status);
      add(need_save);
      add(is_folder);
      add(map);
      add(node);
      add(tooltip);
    }

    //! \brief
    Gtk::TreeModelColumn<SimTaDynMapPtr> map;
    //! \brief
    Gtk::TreeModelColumn<SceneNodePtr> node;
    //! \brief
    Gtk::TreeModelColumn<Category> category;
    //! \brief Icon displaying the type of the SceneNode.
    Gtk::TreeModelColumn<PixbufPtr> icon_type;
    //! \brief Display the file name of the document (without its path).
    Gtk::TreeModelColumn<Glib::ustring> filename;
    //! \brief Icon showing the state of the file (ok, ko, modified)
    Gtk::TreeModelColumn<PixbufPtr> icon_status;
    //! \brief FIXME: mettre un observateur a la place du bool
    Gtk::TreeModelColumn<bool> need_save;
    //! \brief First nodes of the GTK+ treeview is made for categorize
    //! the SimTaDyn fields.
    Gtk::TreeModelColumn<bool> is_folder;
    //! \brief Text to be displayed in a tooltip.
    Gtk::TreeModelColumn<Glib::ustring> tooltip;
  };

public:

  //-----------------------------------------------------------------
  //! \brief Construct the whole GTK+ widgets of the SimTaDyn Map
  //! Explorer as well as widgets of the SceneNodeEditor.
  //-----------------------------------------------------------------
  SimTaDynMapExplorer();

  //-----------------------------------------------------------------
  //! \brief
  //-----------------------------------------------------------------
  ~SimTaDynMapExplorer() { }

  //-----------------------------------------------------------------
  //! \brief Return the SceneNode editor
  //-----------------------------------------------------------------
  SceneNodeEditor& nodeEditor()
  {
    return m_node_editor;
  }

  //-----------------------------------------------------------------
  //! \brief Return the GTK+ widget. This allows this class to be
  //! attached to a GTK+ window.
  //-----------------------------------------------------------------
  inline Gtk::Widget& widget()
  {
    return *this;
  }

  //-----------------------------------------------------------------
  //! \brief Add the SimTaDynMap in the GTK+ Treeview.
  //-----------------------------------------------------------------
  void addMap(SimTaDynMapPtr map);

  //-----------------------------------------------------------------
  //! \brief Refresh Gtk::TreeView associated to map
  //-----------------------------------------------------------------
  void updateMap(SimTaDynMapPtr map/*, SceneNodePtr sheet*/);

  void removeMap(SimTaDynMapPtr map);

private:

  //-----------------------------------------------------------------
  //! \brief Traverse the whole SimTaDynSheets and populate the GTK+
  //! TreeView.
  //! \param sceneNode the root of the SceneGraph storing SimTaDynSheets.
  //! \pram gtkNode the root of the GTK+ Treeview.
  //-----------------------------------------------------------------
  void traverseSceneGraph(SceneNodePtr const sceneNode, TreeIterator& gtkNode);

  //-----------------------------------------------------------------
  //! \brief
  //! TODO: Create an IconManager instead
  //-----------------------------------------------------------------
  bool loadAllIcons();

  //-----------------------------------------------------------------
  //! \brief
  //! TODO: Create an IconManager instead
  //-----------------------------------------------------------------
  bool loadIcon(Icon const icon, std::string const& icon_name);

  //-----------------------------------------------------------------
  //! \brief Signal when the user mouse is placed on a GTK+ Treeview.
  //! This will hide the tooltip displaying the text stored inside the
  //! tree usually an error message (Forth script ko, OpenGL shader
  //! compilation error, map loading error ...)
  //-----------------------------------------------------------------
  bool onQueryTooltip(int x, int y, bool /*keyboard_tooltip*/,
                      const Glib::RefPtr<Gtk::Tooltip>& tooltip);

  //-----------------------------------------------------------------
  //! \brief Signal when the user clicked on a GTK+ treeview node.
  //! Depending on the mouse button either open the SceneNodeEditor
  //! (for editing a SceneNode) or a popup menu for modifing the SceneGraph.
  //-----------------------------------------------------------------
  bool onButtonPressEvent(GdkEventButton *ev);

private:

  //! \brief
  SceneNodeEditor m_node_editor;
  //! \brief
  Gtk::TreeView m_tree_view;
  //! \brief
  ModelColumns m_columns;
  //! \brief
  Glib::RefPtr<Gtk::TreeStore> m_tree_store;
  //! \brief Save
  //TreeIterator m_folders[Category::MaxCategories_];
  //! \brief
  PixbufPtr m_icons[Icon::MaxIcons_];
  Gtk::Menu      m_popup_menu;
  Gtk::MenuItem  m_menuitem[3];
  //Gtk::Entry m_search;

  // TODO evolution:
  // Lazy loading: http://scheffter.de/dirk/C++/gtkmm/treeview-lazy/index.html
  // Search box: https://stackoverflow.com/questions/46176129/searching-in-treeview-and-highlight-select-the-row-that-contains-the-item-that-i
};

#endif // MAP_EXPLORER_HPP
