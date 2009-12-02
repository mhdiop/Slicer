#include "qSlicerMainWindow.h" 

#include "ui_qSlicerMainWindow.h" 

// SlicerQT includes
#include "qSlicerApplication.h"
#include "qSlicerMainWindowCore.h"

// QT includes
#include <QDebug>

//-----------------------------------------------------------------------------
struct qSlicerMainWindowPrivate: public qCTKPrivate<qSlicerMainWindow>, public Ui_qSlicerMainWindow
{
  QCTK_DECLARE_PUBLIC(qSlicerMainWindow);
  qSlicerMainWindowPrivate()
    {
    this->Core = 0;
    }
  qSlicerMainWindowCore* Core;
};

//-----------------------------------------------------------------------------
qSlicerMainWindow::qSlicerMainWindow(QWidget *parent)
 :Superclass(parent)
{
  QCTK_INIT_PRIVATE(qSlicerMainWindow);
  qctk_d()->setupUi(this);
  
  // Main window core helps to coordinate various widgets and panels
  qctk_d()->Core = new qSlicerMainWindowCore(this);
  
  this->setupMenuActions();
}

//-----------------------------------------------------------------------------
QCTK_GET_CXX(qSlicerMainWindow, qSlicerMainWindowCore*, core, Core);

//-----------------------------------------------------------------------------
void qSlicerMainWindow::setupMenuActions()
{
  this->connect(
    qctk_d()->actionFileExit, SIGNAL(triggered()),
    qSlicerApplication::instance(), SLOT(quit()));
}
