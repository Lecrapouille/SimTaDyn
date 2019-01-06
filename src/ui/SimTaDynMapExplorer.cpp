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

#include "SimTaDynMapExplorer.hpp"
#include "MapEditor.hpp"

//-----------------------------------------------------------------
SceneNodeEditor::SceneNodeEditor()
  : m_table{ Gtk::Table(1, 1), Gtk::Table(6, 6), Gtk::Table(5, 5) }
{
  // SceneNode informations
  m_frame[0].set_label("Scene Node");
  m_frame[0].add(m_hbox[0]);
  m_hbox[0].pack_start(m_labels[LabelNames::LabelLayerName]);
  m_hbox[0].pack_start(m_entries[EntryNames::EntryLayerName]);
  m_hbox[0].pack_start(m_labels[LabelNames::LabelFilePath]);
  m_hbox[0].pack_start(m_entries[EntryNames::EntryFilePath]);

  m_labels[LabelNames::LabelLayerName].set_text("Layer Name:");
  m_labels[LabelNames::LabelFilePath].set_text("File Path:");
  //m_labels[LabelNames::LabelMapProjection].set_text("Projection:");

  // States of the Movable class
  m_frame[1].set_label("Inputs");
  m_frame[1].add(m_table[1]);
  m_table[1].attach(m_labels[LabelNames::LabelAxis],       0, 1, 0, 1);
  m_table[1].attach(m_labels[LabelNames::LabelPosition],   1, 2, 0, 1);
  m_table[1].attach(m_labels[LabelNames::LabelRotation],   2, 3, 0, 1);
  m_table[1].attach(m_labels[LabelNames::LabelGlobScale],  3, 4, 0, 1);
  m_table[1].attach(m_labels[LabelNames::LabelLocalScale], 4, 5, 0, 1);

  m_labels[LabelNames::LabelAxis].set_text("Axis:");
  m_labels[LabelNames::LabelAxisX].set_text("X:");
  m_labels[LabelNames::LabelAxisY].set_text("Y:");
  m_labels[LabelNames::LabelAxisZ].set_text("Z:");
  m_labels[LabelNames::LabelAngle].set_text("Angl:");
  m_labels[LabelNames::LabelPosition].set_text("Position:");
  m_labels[LabelNames::LabelRotation].set_text("Rotation Axis:");
  m_labels[LabelNames::LabelGlobScale].set_text("Global Scale:");
  m_labels[LabelNames::LabelLocalScale].set_text("Local Scale:");

  m_table[1].attach(m_labels[LabelNames::LabelAngle],  0, 1, 4, 5);
  m_table[1].attach(m_labels[LabelNames::LabelAxisZ],  0, 1, 3, 4);
  m_table[1].attach(m_labels[LabelNames::LabelAxisY],  0, 1, 2, 3);
  m_table[1].attach(m_labels[LabelNames::LabelAxisX],  0, 1, 1, 2);

  m_table[1].attach(m_entries[EntryNames::EntryPositionZ], 1, 2, 3, 4);
  m_table[1].attach(m_entries[EntryNames::EntryPositionY], 1, 2, 2, 3);
  m_table[1].attach(m_entries[EntryNames::EntryPositionX], 1, 2, 1, 2);

  m_table[1].attach(m_entries[EntryNames::EntryRotationZ],   2, 3, 3, 4);
  m_table[1].attach(m_entries[EntryNames::EntryRotationY],   2, 3, 2, 3);
  m_table[1].attach(m_entries[EntryNames::EntryRotationX],   2, 3, 1, 2);
  m_table[1].attach(m_entries[EntryNames::EntryRotationAng], 2, 3, 4, 5);

  m_table[1].attach(m_entries[EntryNames::EntryGlobScaleZ], 3, 4, 3, 4);
  m_table[1].attach(m_entries[EntryNames::EntryGlobScaleY], 3, 4, 2, 3);
  m_table[1].attach(m_entries[EntryNames::EntryGlobScaleX], 3, 4, 1, 2);

  m_table[1].attach(m_entries[EntryNames::EntryLocScaleZ], 4, 5, 3, 4);
  m_table[1].attach(m_entries[EntryNames::EntryLocScaleY], 4, 5, 2, 3);
  m_table[1].attach(m_entries[EntryNames::EntryLocScaleX], 4, 5, 1, 2);

  // Matrix transform results
  m_frame[2].set_label("Result: Matrix transform");
  m_frame[2].add(m_table[2]);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix00], 0, 1, 3, 4);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix10], 1, 2, 3, 4);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix20], 2, 3, 3, 4);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix30], 3, 4, 3, 4);

  m_table[2].attach(m_entries[EntryNames::EntryMatrix01], 0, 1, 4, 5);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix11], 1, 2, 4, 5);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix21], 2, 3, 4, 5);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix31], 3, 4, 4, 5);

  m_table[2].attach(m_entries[EntryNames::EntryMatrix02], 0, 1, 5, 6);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix12], 1, 2, 5, 6);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix22], 2, 3, 5, 6);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix32], 3, 4, 5, 6);

  m_table[2].attach(m_entries[EntryNames::EntryMatrix03], 0, 1, 6, 7);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix13], 1, 2, 6, 7);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix23], 2, 3, 6, 7);
  m_table[2].attach(m_entries[EntryNames::EntryMatrix33], 3, 4, 6, 7);

  // Buttons
  m_apply.set_label("Apply");
  m_cancel.set_label("Cancel");
  m_hbox[1].pack_start(m_apply);
  m_hbox[1].pack_start(m_cancel);

  // Pack all widgets
  m_vbox.pack_start(m_frame[0]);
  m_vbox.pack_start(m_frame[1]);
  m_vbox.pack_start(m_frame[2]);
  m_vbox.pack_start(m_hbox[1]);
  m_vbox.pack_start(m_statusbar);
  add(m_vbox);

  m_apply.signal_clicked().connect(sigc::mem_fun(*this, &SceneNodeEditor::hide));
  m_cancel.signal_clicked().connect(sigc::mem_fun(*this, &SceneNodeEditor::cancel));
  m_cancel.signal_clicked().connect([this](){ this->hide(); });

  for (uint32_t it = 0; it < EntryNames::MaxEntries_; ++it)
    {
      m_entries[it].signal_key_release_event().connect(
        sigc::bind<EntryNames>(
          sigc::mem_fun(*this, &SceneNodeEditor::entryKeyReleaseAmount),
          static_cast<EntryNames>(it)));
    }

  show_all_children();
}

//-----------------------------------------------------------------
void SceneNodeEditor::bind(SceneNodePtr node)
{
  SimTaDynSheetPtr sheet = nullptr;

  if (nullptr != node)
    {
      m_backup_node = *node;
      m_node = node;

      set_title("Scene node settings: " + node->id());

      Vector<float, 3u> const &p = node->position();
      m_entries[EntryNames::EntryPositionX].set_text(std::to_string(p.x));
      m_entries[EntryNames::EntryPositionY].set_text(std::to_string(p.y));
      m_entries[EntryNames::EntryPositionZ].set_text(std::to_string(p.z));

      Vector<float, 4u> const &r = node->rotation();
      m_entries[EntryNames::EntryRotationX].set_text(std::to_string(r.x));
      m_entries[EntryNames::EntryRotationY].set_text(std::to_string(r.y));
      m_entries[EntryNames::EntryRotationZ].set_text(std::to_string(r.z));
      m_entries[EntryNames::EntryRotationAng].set_text(std::to_string(r.w));

      Vector<float, 3u> const &g = node->scale();
      m_entries[EntryNames::EntryGlobScaleX].set_text(std::to_string(g.x));
      m_entries[EntryNames::EntryGlobScaleY].set_text(std::to_string(g.y));
      m_entries[EntryNames::EntryGlobScaleZ].set_text(std::to_string(g.z));

      Vector<float, 3u> const &s = node->localScale();
      m_entries[EntryNames::EntryLocScaleX].set_text(std::to_string(s.x));
      m_entries[EntryNames::EntryLocScaleY].set_text(std::to_string(s.y));
      m_entries[EntryNames::EntryLocScaleZ].set_text(std::to_string(s.z));

      Matrix<float, 4u, 4u> const &M = node->transform();
      for (uint8_t i = 0; i < 16u; ++i)
        m_entries[EntryNames::EntryMatrix00 + i].set_text(std::to_string(M.m_data[i]));

      sheet = node->renderable();
    }
  else
    {
      m_entries[EntryNames::EntryPositionX].set_text("");
      m_entries[EntryNames::EntryPositionY].set_text("");
      m_entries[EntryNames::EntryPositionZ].set_text("");

      m_entries[EntryNames::EntryRotationX].set_text("");
      m_entries[EntryNames::EntryRotationY].set_text("");
      m_entries[EntryNames::EntryRotationZ].set_text("");

      m_entries[EntryNames::EntryGlobScaleX].set_text("");
      m_entries[EntryNames::EntryGlobScaleY].set_text("");
      m_entries[EntryNames::EntryGlobScaleZ].set_text("");

      m_entries[EntryNames::EntryLocScaleX].set_text("");
      m_entries[EntryNames::EntryLocScaleY].set_text("");
      m_entries[EntryNames::EntryLocScaleZ].set_text("");

      for (uint8_t i = 0; i < 16u; ++i)
        m_entries[EntryNames::EntryMatrix00 + i].set_text("");
    }

  if (nullptr != sheet)
    {
      m_entries[EntryNames::EntryLayerName].set_text(sheet->name());
      m_entries[EntryNames::EntryFilePath].set_text(sheet->path());
      // m_entries[EntryNames::EntryMapProjection].set_text(
      //  std::to_string(static_cast<unsigned>(sheet->mapProjectionType())));
    }
  else
    {
      m_entries[EntryNames::EntryLayerName].set_text("");
      m_entries[EntryNames::EntryFilePath].set_text("");
      //m_entries[EntryNames::EntryMapProjection].set_text("None");
    }
}

//-----------------------------------------------------------------
static bool is_number(const std::string& s)
{
  return (!s.empty()) &&
    (s.end() == std::find_if(s.begin(), s.end(), [](char c)
    {
      return (!std::isdigit(c)) && (c != ',') && (c != '.');
    }));
}

//-----------------------------------------------------------------
bool SceneNodeEditor::entryKeyReleaseAmount(GdkEventKey*, EntryNames const it)
{
  m_statusbar.remove_all_messages();
  auto const& text = m_entries[it].get_text();
  if (is_number(text))
    {
      m_statusbar.push("");
      apply();
    }
  else
    {
      m_statusbar.push("Error: " + text + ": is not a valid number");
    }
  return true;
}

//-------------------------------------------------
float SceneNodeEditor::getFloat(EntryNames const it)
{
  return atof(m_entries[it].get_text().c_str());
}

//-------------------------------------------------
void SceneNodeEditor::cancel()
{
  *m_node = m_backup_node;
  hide();
}

//-------------------------------------------------
void SceneNodeEditor::apply()
{
  // Check if all entries have numeric values
  for (uint32_t it = EntryNames::EntryPositionX; it < EntryNames::MaxEntries_; ++it)
    {
      auto const& text = m_entries[it].get_text();
      if (!is_number(text))
        {
          m_statusbar.push("Error: " + text + ": is not a valid number");
          return ;
        }
    }

  static int i = 0;
  m_statusbar.push("Apply " + std::to_string(i++));

  m_node->position(Vector3f(getFloat(EntryNames::EntryPositionX),
                            getFloat(EntryNames::EntryPositionY),
                            getFloat(EntryNames::EntryPositionZ)));
  m_node->rotate(getFloat(EntryNames::EntryRotationAng),
                 Vector3f(getFloat(EntryNames::EntryRotationX),
                          getFloat(EntryNames::EntryRotationY),
                          getFloat(EntryNames::EntryRotationZ)));
  m_node->scale(Vector3f(getFloat(EntryNames::EntryGlobScaleX),
                         getFloat(EntryNames::EntryGlobScaleY),
                         getFloat(EntryNames::EntryGlobScaleZ)));
  m_node->localScale(Vector3f(getFloat(EntryNames::EntryLocScaleX),
                              getFloat(EntryNames::EntryLocScaleY),
                              getFloat(EntryNames::EntryLocScaleZ)));

  Matrix<float, 4u, 4u> const &M = m_node->transform();
  for (uint8_t i = 0; i < 16u; ++i)
    m_entries[EntryNames::EntryMatrix00 + i].set_text(std::to_string(M.m_data[i]));
}

//-----------------------------------------------------------------
SimTaDynMapExplorer::SimTaDynMapExplorer()
{
  // Preload some icons. FIXME: create an IconManager
  if (!loadAllIcons())
    {
      LOGE("Some icons will be missing inside the GUI");
    }

  // Create the Tree model
  m_tree_store = Gtk::TreeStore::create(m_columns);

  // Create the Tree view
  m_tree_view.set_model(m_tree_store);
  m_tree_view.append_column("Icon", m_columns.icon_type);
  m_tree_view.append_column("Name", m_columns.filename);
  m_tree_view.append_column("Status", m_columns.icon_status);
  m_tree_view.set_enable_tree_lines();
  m_tree_view.set_has_tooltip();

  // Add an entry for searching in the tree
  // https://www.kksou.com/php-gtk2/sample-codes/setup-interactive-search-in-GtkTreeView-for-PHP-GTK-v2.0-Part-3-start-on-any-keypress.php
  // m_tree_view.set_enable_search();
  // m_tree_view.set_search_entry(m_search);

  // Insert widgets
  Gtk::ScrolledWindow::add(m_tree_view);
  Gtk::ScrolledWindow::set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

  // Create the popup menu appearing when right click button on the treeview
  m_menuitem[0].set_label("Delete Sheet");
  m_menuitem[0].signal_activate().connect([this](){ /* TODO Select sheet + delete it: MapEditor::instance.removeSheet(row[m_columns.sheet]) */ });
  m_popup_menu.append(m_menuitem[0]);
  m_menuitem[1].set_label("Add child Sheet");
  m_menuitem[1].signal_activate().connect([this](){ /* TODO Select sheet + add SG child */ });
  m_popup_menu.append(m_menuitem[1]);
  m_menuitem[2].set_label("Add brother Sheet");
  m_menuitem[2].signal_activate().connect([this](){ /* TODO Select sheet + add SG brother */ });
  m_popup_menu.append(m_menuitem[2]);
  m_popup_menu.attach_to_widget(m_tree_view);

  // Signals
  m_tree_view.signal_query_tooltip().connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::on_param_tree_view_query_tooltip));
  m_tree_view.signal_button_press_event().connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::on_mytreeview_button_press_event));
  MapEditor::instance().loaded_success.connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::onSuccessMapLoaded));
  MapEditor::instance().sheet_changed.connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::on_sheet_changed));
  MapEditor::instance().loaded_failure.connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::onFailMapLoaded));
  MapEditor::instance().saved_success.connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::onSuccessMapSaved));
  MapEditor::instance().saved_failure.connect(
     sigc::mem_fun(*this, &SimTaDynMapExplorer::onFailMapSaved));
}

//-----------------------------------------------------------------
bool SimTaDynMapExplorer::loadIcon(Icon const icon, std::string const& icon_name)
{
  bool res = false;

  try
    {
      // FIXME Resource Manager
      m_icons[icon] = Gdk::Pixbuf::create_from_file(
        PathManager::instance().expand("icons/" + icon_name),
        16, 16);
      res = true;
    }
  catch (const Gio::ResourceError& ex)
    {
      LOGC("ResourceError: '%s'", ex.what().c_str());
    }
  catch (const Gdk::PixbufError& ex)
    {
      LOGC("PixbufError: '%s'", ex.what().c_str());
    }
  catch (const Glib::FileError& ex)
    {
      LOGC("FileError: '%s'", ex.what().c_str());
    }

  return res;
}

//-----------------------------------------------------------------
bool SimTaDynMapExplorer::loadAllIcons()
{
  bool res = true;

  res &= loadIcon(Icon::Folder,   "folder.png");
  res &= loadIcon(Icon::Sheet,    "map.png");
  res &= loadIcon(Icon::Graph,    "graph.png");
  res &= loadIcon(Icon::Script,   "script.png");
  res &= loadIcon(Icon::File,     "index.png");
  res &= loadIcon(Icon::Resource, "file.png");
  res &= loadIcon(Icon::Ok,       "ok.png");
  res &= loadIcon(Icon::Ko,       "ko.png");
  res &= loadIcon(Icon::Warning,  "warning.png");
  return res;
}

//-----------------------------------------------------------------
void SimTaDynMapExplorer::addMap(SimTaDynMapPtr map)
{
  // Append Map folder
  TreeRow root = *(m_tree_store->append());
  {
    root[m_columns.category] = Category::Maps;
    root[m_columns.icon_type] = m_icons[Icon::Folder];
    root[m_columns.filename] = map->name();
    root[m_columns.tooltip] = map->about;
    root[m_columns.need_save] = false;
    root[m_columns.map] = map;
  }

  // Index
  {
    TreeRow index = *(m_tree_store->append(root.children()));
    index[m_columns.category] = Category::Index;
    index[m_columns.icon_type] = m_icons[Icon::File];
    index[m_columns.filename] = "Index";
    index[m_columns.need_save] = false;
    index[m_columns.tooltip] = "Index tooltip";
    //FIXME index[m_columns.ptr.file] = &(map->m_index_file);
  }

  // Sheets
  {
    TreeIterator it = m_tree_store->append(root.children());
    TreeRow sheets = *(it);
    sheets[m_columns.category] = Category::Folders;
    sheets[m_columns.icon_type] = m_icons[Icon::Folder];
    sheets[m_columns.filename] = "Sheets/";
    traverseSceneGraph(map->rootSceneGraph(), it);
  }

  // Forth scripts
  {
    TreeRow scripts = *(m_tree_store->append(root.children()));
    scripts[m_columns.category] = Category::Folders;
    scripts[m_columns.icon_type] = m_icons[Icon::Folder];
    scripts[m_columns.filename] = "Scripts/";

    for (auto& it: map->m_scripts_forth)
      {
        TreeRow row = *(m_tree_store->append(scripts.children()));
        row[m_columns.category] = Category::Scripts;
        row[m_columns.icon_type] = m_icons[Icon::Script];
        row[m_columns.filename] = it;
        row[m_columns.need_save] = false;
        //FIXME row[m_columns.ptr.file] = &it;
      }
  }

  // Resources
  {
    TreeRow resources = *(m_tree_store->append(root.children()));
    resources[m_columns.category] = Category::Folders;
    resources[m_columns.icon_type] = m_icons[Icon::Folder];
    resources[m_columns.filename] = "Resources/";
    resources[m_columns.need_save] = false;

    // TODO for (auto& it: map->m_resources)
  }

  m_tree_view.expand_all();
}

//-----------------------------------------------------------------
// https://stackoverflow.com/questions/39833800/how-to-remove-all-child-nodes-from-parent-treeiter-from-gtk-treestore
void SimTaDynMapExplorer::on_sheet_changed(SimTaDynMapPtr map/*, SceneNodePtr sheet*/)
{
  std::cout << "on_sheet_added map " << map->name() << std::endl;

  // Search for the map name
  Gtk::TreeModel::Children const& children = m_tree_store->children();
  for (auto iter = children.begin(); iter != children.end(); ++iter)
    {
      Gtk::TreeModel::Row row = *iter;
      if ((Category::Maps == row.get_value(m_columns.category)) &&
          (map->name() == row.get_value(m_columns.filename)))
        {
          std::cout << "Found map " << map->name() << std::endl;

          // Search for the folder Sheets/
          Gtk::TreeModel::Children const& child = row.children();
          for (auto it = child.begin(); it != child.end(); ++it)
            {
              if ("Sheets/" == (*it).get_value(m_columns.filename))
                {
                  // Erase older nodes and rebuild nodes
                  std::cout << "Found Sheets " << std::endl;

                  auto c = it->children().begin();
                  if (c != it->children().end())
                    {
                      TreeIterator itt = m_tree_store->erase(c);
                      traverseSceneGraph(map->rootSceneGraph(), it);
                    }
                  else
                    {
                      traverseSceneGraph(map->rootSceneGraph(), it);
                    }
                  m_tree_view.expand_all();
                  return ;
                }
            }
        }
    }

  /*
  TreeIterator iter = m_tree_store->get_iter("0");
  for (auto const& it: m_tree_store->get_iter())
    {
      auto const& row = *it;
      if ((Category::Maps == row.get_value(m_columns.category)) &&
          (map->name() == row.get_value(m_columns.filename)))
        {
          TreeIterator itt = m_tree_store->erase(it);
          traverseSceneGraph(map->rootSceneGraph(), itt);
          return ;
        }
    }
  */
}

//-----------------------------------------------------------------
void SimTaDynMapExplorer::traverseSceneGraph(SceneNodePtr sceneNode, TreeIterator& gtkNode)
{
  if (nullptr == sceneNode)
    {
      LOGI("SceneNode nullptr");
      return ;
    }

  LOGI("traverseSceneGraph '%s' %u childs", sceneNode->id().c_str(), sceneNode->nbChildren());

  // Node

  TreeIterator root_child = m_tree_store->append(gtkNode->children());
  TreeRow childrow = *(root_child);

  childrow[m_columns.icon_type] = m_icons[Icon::Sheet];
  childrow[m_columns.category] = Category::Sheets;
  childrow[m_columns.filename] = sceneNode->id();
  childrow[m_columns.icon_status] = m_icons[Icon::Ok];
  childrow[m_columns.is_folder] = false;
  childrow[m_columns.need_save] = false;
  childrow[m_columns.node] = sceneNode;

  // Child

  for (auto child: sceneNode->children())
    {
      CPP_LOG(logger::Info) << "Child " << child->id() << "\n";
      traverseSceneGraph(child, root_child);
    }
}

//-----------------------------------------------------------------
// https://www.kksou.com/php-gtk2/sample-codes/display-tooltips-in-GtkTreeView-Part-1.php
// http://gtk.10911.n7.nabble.com/Gtk-Tooltip-and-Gtk-TreeView-td43664.html
bool SimTaDynMapExplorer::on_param_tree_view_query_tooltip(int x, int y,
                                                           bool /*keyboard_tooltip*/,
                                                           const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
  Gtk::TreeModel::Path path;
  Gtk::TreeViewColumn *column;
  int cell_x, cell_y;
  int bx, by;

  m_tree_view.convert_widget_to_bin_window_coords(x, y, bx, by);
  if (!m_tree_view.get_path_at_pos(bx, by, path, column, cell_x,cell_y))
    return false;

  Gtk::TreeIter iter(m_tree_view.get_model()->get_iter(path));
  if (!iter)
    return false;

  Gtk::TreeRow row = *(iter);
  Glib::ustring tooltip_string(row[m_columns.tooltip]);
  if (tooltip_string.empty())
    return false;

  tooltip->set_text(tooltip_string);
  m_tree_view.set_tooltip_row(tooltip, path);
  return true;
}

//-----------------------------------------------------------------
bool SimTaDynMapExplorer::on_mytreeview_button_press_event(GdkEventButton *ev)
{
  if (ev->button == 1)
    {
      Glib::RefPtr<Gtk::TreeSelection> selection = m_tree_view.get_selection();
      TreeIterator selectedRow = selection->get_selected();
      TreeRow row = *selectedRow;
      if (Category::Sheets == row.get_value(m_columns.category))
        {
          Glib::ustring name = row.get_value(m_columns.filename);
          LOGD("Mouse click on Sheet '%s'", name.c_str());
          m_node_editor.bind(row.get_value(m_columns.node));
          m_node_editor.show();
        }
      return false;
    }
  else if (ev->button == 2)
    {
      std::cout << "pressed2 " << std::endl;
      m_popup_menu.show_all();
      m_popup_menu.popup(ev->button, ev->time);
      return false;
    }

  return false;
}

//-----------------------------------------------------------------
void SimTaDynMapExplorer::onSuccessMapLoaded(SimTaDynMapPtr map)
{
  if (nullptr == map)
    {
      LOGC("Should not received a succesfully loaded map with nullptr");
      return ;
    }

  LOGC("SimTaDynMapExplorer::onSuccessMapLoaded '%s'", map->m_name.c_str());
  addMap(map);
}

//-----------------------------------------------------------------
void SimTaDynMapExplorer::onFailMapLoaded(const std::string &filename, const std::string &message)
{
  LOGC("SimTaDynMapExplorer::onFailMapLoaded '%s': '%s' ", filename.c_str(), message.c_str());
  //clear(); // FIXME: non: on perd les ancinnes informations
  //addDocument(Category::Sheets, filename, Icon::Ko, message);
}

//-----------------------------------------------------------------
void SimTaDynMapExplorer::onSuccessMapSaved(SimTaDynMapPtr map)
{
  LOGC("SimTaDynMapExplorer::onSuccessMapSaved '%s'", map->m_name.c_str());
  // TODO: remove the not saved "*" symbol
  // row[m_columns.need_save] = false;
}

//-----------------------------------------------------------------
void SimTaDynMapExplorer::onFailMapSaved(const std::string &filename, const std::string &message)
{
  LOGC("SimTaDynMapExplorer::onFailMapSaved '%s': '%s' ", filename.c_str(), message.c_str());
  //addDocument(Category::Sheets, filename, Icon::Ko, message);
  // TODO: store error in tooltips and change to red the color
  // row[m_columns.tooltip] = message;
}
