//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef PACKAGE_EXPLORER_HPP_
#  define PACKAGE_EXPLORER_HPP_

#  include "ILoader.tpp"
#  include "SimTaDynMap.hpp"
#  include <gtkmm.h>

class SimTaDynMap;

// **************************************************************
//! \brief
// **************************************************************
class SceneGraphWindow : public Gtk::Window
{
public:

  SceneGraphWindow(SceneNode_t* node);
  void node(SceneNode_t* node);

private:

  enum LabelNames { LabelLayerName, LabelFilePath, LabelScale,
                    LabelMapProjection, LabelMatrix,
                    LabelButtons,
                    LastLabelNames = LabelButtons };
  enum EntryNames { EntryLayerName, EntryFilePath,
                    EntryScaleX, EntryScaleY, EntryScaleZ,
                    EntryMapProjection,
                    EntryMatrix00, EntryMatrix01, EntryMatrix02, EntryMatrix03,
                    EntryMatrix10, EntryMatrix11, EntryMatrix12, EntryMatrix13,
                    EntryMatrix20, EntryMatrix21, EntryMatrix22, EntryMatrix23,
                    EntryMatrix30, EntryMatrix31, EntryMatrix32, EntryMatrix33,
                    LastEntryNames = EntryMatrix33 };

  //Gtk::VBox                m_vbox;
  //Gtk::HBox                m_hbox[LastLabelNames + 1u];
  Gtk::Table               m_table;
  Gtk::Label               m_labels[LastLabelNames + 1u];
  Gtk::Entry               m_entries[LastEntryNames + 1u];
  Gtk::Button              m_ok;
  Gtk::Button              m_cancel;

  SceneNode_t             *m_node;
};

// **************************************************************
//! \brief
// **************************************************************
class PackageExplorer
{
private:

  //
  typedef Glib::RefPtr<Gdk::Pixbuf> PixbufPtr;
  //
  enum Category { Index, Maps, Scripts, Resources, MaxCategory = Resources };

  // **************************************************************
  //! \brief Tree model columns
  // **************************************************************
  class ModelColumns: public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    {
      add(m_icon_type);
      add(m_filename); // FIXME: ajouter fullpath (pour coller au ForthEditor)
      add(m_icon_status);
      add(m_is_folder);
    }

    //! \brief Icon displaying either a folder or a document.
    Gtk::TreeModelColumn<PixbufPtr> m_icon_type;
    //! \brief Display the file name of the document (without its path).
    Gtk::TreeModelColumn<Glib::ustring> m_filename;
    //! \brief Icon showing the state of the file (ok, changed, error)
    Gtk::TreeModelColumn<PixbufPtr> m_icon_status;
    //
    Gtk::TreeModelColumn<bool> m_is_folder;
  };

public:

  //! \brief
  PackageExplorer();

  //! \brief
  ~PackageExplorer();

  //! \brief
  // TODO addSubMap(Glib::ustring const &filename) // TODO afin d'avoir un scenegraph
  inline Gtk::TreeModel::Row const *addMap(Glib::ustring const &filename)
  {
    return addDocument(Maps, filename);
  }
  //! \brief
  inline Gtk::TreeModel::Row const *addResource(Glib::ustring const &filename)
  {
    return addDocument(Resources, filename);
  }
  //! \brief
  inline Gtk::TreeModel::Row const *addScript(Glib::ustring const &filename)
  {
    return addDocument(Scripts, filename);
  }
  inline Gtk::Widget& widget()
  {
    return m_tree_view;
  }

  // TODO loadPak("paris.pak"); --> faire un clear de l'arbre

  // TODO changeStatusKO(string reason) {
  // childrow[m_columns.m_icon_status] = m_icon_status_KO;
  // childrow[m_columns.m_error_msg] = reason;
  // }
  // TODO changeStatusOK() {
  // childrow[m_columns.m_icon_status] = m_icon_status_KO;
  // childrow[m_columns.m_error_msg] = "";
  // }
  // TODO ajouter des observers pour charger/decharger le contenu, appeller changerStatus ...

  void onSuccessMapLoaded(SimTaDynMapPtr map);
  void onFailMapLoaded(const std::string &filename, const std::string &message);
  void onSuccessMapSaved(SimTaDynMapPtr map);
  void onFailMapSaved(const std::string &filename, const std::string &message);

public:

  SceneGraphWindow m_scene_graph_window;

protected:

  //! \brief
  Gtk::TreeModel::Row const *addDocument(PackageExplorer::Category const type, Glib::ustring const &filename);
  //! \brief
  void addFolder(Category const type, Glib::ustring const &name);
  void addIndex();

protected:

  //! \brief
  Gtk::TreeView m_tree_view;
  //! \brief
  ModelColumns m_columns;
  //! \brief
  Glib::RefPtr<Gtk::TreeStore> m_tree_store;
  //! \brief Save
  Gtk::TreeModel::Row m_folders[MaxCategory + 1U];

private:

  //! \brief
  bool on_mytreeview_button_press_event(GdkEventButton *ev);
  // TODO: callback souris affiche un tooltip affichant le nom
  // complet du fichier, son status et l'erreur (forth script ko,
  // shader mal charge, carte inconnue, etc ...)

  //! \brief
  PixbufPtr m_icon_folder;
  //! \brief
  PixbufPtr m_icon_document;
  //! \brief
  PixbufPtr m_icon_status_ok;
  PixbufPtr m_icon_status_warning;
  PixbufPtr m_icon_status_ko;
};

#endif
