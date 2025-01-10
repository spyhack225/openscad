/*
 *  OpenSCAD (www.openscad.org)
 *  Copyright (C) 2009-2019 Clifford Wolf <clifford@clifford.at> and
 *                          Marius Kintel <marius@kintel.net>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  As a special exception, you have permission to link this program
 *  with the CGAL library and distribute executables, as long as you
 *  follow the requirements of the GNU GPL in regard to all of the
 *  software in the executable aside from CGAL.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include "gui/ExportPdfDialog.h"

#include <QString>
#include <QDialog>

#include "QSettingsCached.h"
#include "export.h"

namespace {

paperSizes sizeString2Enum(const QString& current){
   for(size_t i = 0; i < paperSizeStrings.size(); i++){
       if (current.toStdString()==paperSizeStrings[i]) return static_cast<paperSizes>(i);
   };
   return paperSizes::A4;
};

paperOrientations orientationsString2Enum(const QString& current){
   for(size_t i = 0; i < paperOrientationsStrings.size(); i++){
       if (current.toStdString()==paperOrientationsStrings[i]) return static_cast<paperOrientations>(i);
   };
   return paperOrientations::PORTRAIT;
};

}

ExportPdfDialog::ExportPdfDialog()
{
  setupUi(this);

  QSettingsCached settings;
  // Get current settings or defaults modify the two enums (next two rows) to explicitly use default by lookup to string (see the later set methods).
  this->setPaperSize(sizeString2Enum(settings.value("exportPdfOpts/paperSize", QString::fromStdString(paperSizeStrings[static_cast<int>(options.paperSize)])).toString()));  // enum map
  this->setOrientation(orientationsString2Enum(settings.value("exportPdfOpts/orientation", QString::fromStdString(paperOrientationsStrings[static_cast<int>(options.Orientation)])).toString()));  // enum map
  this->setShowDesignFilename(settings.value("exportPdfOpts/showDsgnFN", options.showDesignFilename).toBool());
  this->setShowScale(settings.value("exportPdfOpts/showScale", options.showScale).toBool());
  this->setShowScaleMsg(settings.value("exportPdfOpts/showScaleMsg", options.showScaleMsg).toBool());
  this->setShowGrid(settings.value("exportPdfOpts/showGrid", options.showGrid).toBool());
  this->setGridSize(settings.value("exportPdfOpts/gridSize", options.gridSize).toDouble());
}

int ExportPdfDialog::exec()
{
  const auto result = QDialog::exec();

  if (result == QDialog::Accepted) {
    options.paperSize = this->getPaperSize();
    options.Orientation = this->getOrientation();
    options.showDesignFilename = this->getShowDesignFilename();
    options.showScale = this->getShowScale();
    options.showScaleMsg = this->getShowScaleMsg();
    options.showGrid = this->getShowGrid();
    options.gridSize = this->getGridSize();

    QSettingsCached settings;
    settings.setValue("exportPdfOpts/paperSize", QString::fromStdString(paperSizeStrings[static_cast<int>(this->getPaperSize())]));
    settings.setValue("exportPdfOpts/orientation", QString::fromStdString(paperOrientationsStrings[static_cast<int>(this->getOrientation())]));
    settings.setValue("exportPdfOpts/showDsgnFN", this->getShowDesignFilename());
    settings.setValue("exportPdfOpts/showScale", this->getShowScale());
    settings.setValue("exportPdfOpts/showScaleMsg", this->getShowScaleMsg());
    settings.setValue("exportPdfOpts/showGrid", this->getShowGrid());
    settings.setValue("exportPdfOpts/gridSize", this->getGridSize());
  }

  return result;
}

double ExportPdfDialog::getGridSize() const
{
  //  5 values are coded:  2, 2.5, 4, 5, 10; All are integer divisors of 20.
  switch (gridButtonGroup->checkedId()) {
  case -2: return 2.; break;
  case -3: return 2.5; break;
  case -4: return 4.; break;
  case -5: return 5.; break;
  case -6: return 10.; break;
  default:  // Always return a valid value.
    // LOG(error);
    return 5;
    break;
  }
}

paperOrientations ExportPdfDialog::getOrientation() const
{
  switch (orientButtonGroup->checkedId()) {
  case -2: return paperOrientations::PORTRAIT; break;
  case -3: return paperOrientations::LANDSCAPE; break;
  case -4: return paperOrientations::AUTO; break;
  default:  // Always return a valid value.
    // LOG(error);
    return paperOrientations::AUTO;
    break;
  }
}

paperSizes ExportPdfDialog::getPaperSize() const
{
  switch (sizeButtonGroup->checkedId()) {
  case -2: return paperSizes::A4; break;
  case -3: return paperSizes::A3; break;
  case -4: return paperSizes::LETTER; break;
  case -5: return paperSizes::LEGAL; break;
  case -6: return paperSizes::TABLOID; break;
  default:  // Always return a valid value.  Matches default.
    // LOG(error);
    return paperSizes::A4;
    break;
  }
}

bool ExportPdfDialog::getShowScale() const { return groupScale->isChecked(); }
bool ExportPdfDialog::getShowScaleMsg() const { return cbScaleUsg->isChecked(); }
bool ExportPdfDialog::getShowDesignFilename() const { return cbDsnFn->isChecked(); }
bool ExportPdfDialog::getShowGrid() const { return groupGrid->isChecked(); }

// Setters
void ExportPdfDialog::setShowScale(bool state) { groupScale->setChecked(state); }
void ExportPdfDialog::setShowScaleMsg(bool state) { cbScaleUsg->setChecked(state); }
void ExportPdfDialog::setShowDesignFilename(bool state) { cbDsnFn->setChecked(state); }
void ExportPdfDialog::setShowGrid(bool state) { groupGrid->setChecked(state); }

void ExportPdfDialog::setPaperSize(paperSizes paper)
{
  // setButtonInGroup(sizeButtonGroup, paper2buttons[paper]);
  switch (paper) {
  case paperSizes::A4:      rbS_A4->setChecked(TRUE); break;
  case paperSizes::A3:      rbS_A3->setChecked(TRUE); break;
  case paperSizes::LETTER:  rbS_Ltr->setChecked(TRUE); break;
  case paperSizes::LEGAL:   rbS_Leg->setChecked(TRUE); break;
  case paperSizes::TABLOID: rbS_Tab->setChecked(TRUE); break;
  default:  // provide a sane default.  Shouldn't execute, but needed at least to compile.
    rbS_A4->setChecked(TRUE);
    //  LOG(message_group::Export_Warning, "Export Paper Size Unknon( %1$8c )", paper);
  }
}

void ExportPdfDialog::setOrientation(paperOrientations orient)
{
  switch (orient) {
  case paperOrientations::PORTRAIT:  rbOPort->setChecked(TRUE); break;
  case paperOrientations::LANDSCAPE: rbOLand->setChecked(TRUE); break;
  case paperOrientations::AUTO:      rbOAuto->setChecked(TRUE); break;
  default:  // provide a sane default.  Shouldn't execute, but needed at least to compile.
    //  LOG(message_group::Export_Warning, "Export Paper Size Unknon( %1$8c )", paper);
    rbOAuto->setChecked(TRUE);
    break;
  }
}
void ExportPdfDialog::setGridSize(double value)
{
  //  need to bin to match enums, but tolerate numerical error.
  //  5 values are coded:  2, 2.5, 4, 5, 10; All are integer divisors of 20.
  if (value < 2.24) {  // match 2
    rbGs_2mm->setChecked(TRUE);
  } else if (value < 3.1) {  // match 2.5
    rbGs_2r5mm->setChecked(TRUE);
  } else if (value < 4.4) {  // match 4
    rbGs_4mm->setChecked(TRUE);
  } else if (value < 7.5) {  // match 5
    rbGs_5mm->setChecked(TRUE);
  } else {  // match 10
    rbGs_10mm->setChecked(TRUE);
  };
}
