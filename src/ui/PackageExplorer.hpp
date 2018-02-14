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

#  include <gtkmm.h>
#  include "ILoader.tpp"

class SimTaDynMap;

// **************************************************************
//! \brief
// **************************************************************
class PackageExplorer: public ILoaderListener<SimTaDynMap>
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

  virtual void onLoadFailure(std::string const& filename, std::string const& msg) override;
  virtual void onLoadSucess(std::string const& filename) override;
  virtual void onSaveFailure(std::string const& filename, std::string const& msg) override;
  virtual void onSaveSucess(std::string const& filename) override;

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
