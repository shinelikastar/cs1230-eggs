#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "view.h"
#include "Settings.h"
#include "Databinding.h"
#include <QGLFormat>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    settings.loadSettingsOrDefaults();

    QGLFormat qglFormat;
    qglFormat.setVersion(4, 3);
    qglFormat.setProfile(QGLFormat::CoreProfile);
    qglFormat.setSampleBuffers(true);

    ui->setupUi(this);

    dataBind();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dataBind() {
#define BIND(b) { \
    DataBinding *_b = (b); \
    m_bindings.push_back(_b); \
    assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); \
}
    QButtonGroup *dofButtonGroup = new QButtonGroup;
    QButtonGroup *bezierPOVGroup = new QButtonGroup;
    QButtonGroup *bezierPathGroup = new QButtonGroup;

    m_buttonGroups.push_back(dofButtonGroup);
    m_buttonGroups.push_back(bezierPOVGroup);
    m_buttonGroups.push_back(bezierPathGroup);

    // facing egg radio buttons
    BIND(ChoiceBinding::bindRadioButtons(bezierPOVGroup, 2, settings.facingEgg,
                                         ui->eggButton, ui->contentButton));
    // different path choices
    BIND(ChoiceBinding::bindRadioButtons(bezierPathGroup, 3, settings.pathNum,
                                         ui->pathOneButton, ui->pathTwoButton, ui->pathThreeButton));

    // paused binding
    BIND(BoolBinding::bindCheckbox(ui->motionCheck, settings.paused));
    // height and scale binding
    BIND(FloatBinding::bindSliderAndTextbox(ui->heightSlider, ui->heightInput, settings.height, 2.f, 4.f));
    BIND(FloatBinding::bindSliderAndTextbox(ui->scaleSlider, ui->scaleInput, settings.scale, 1.f, 4.f));

    // Depth of Field
    BIND(BoolBinding::bindCheckbox(ui->dofEnableCheckbox, settings.enableDof));
    BIND(FloatBinding::bindLogSliderAndTextbox(
             ui->fStopSlider, ui->fStopEdit, settings.fStop, 0.1, 10.));
    BIND(FloatBinding::bindSliderAndTextbox(
             ui->planeInFocusSlider, ui->planeInFocusEdit, settings.planeInFocus, 0.1, 55.));

    // Mapping
    BIND(BoolBinding::bindCheckbox(ui->textureCheck, settings.enableTextures));
    BIND(BoolBinding::bindCheckbox(ui->displacementCheck, settings.enableDisplacement));


#undef BIND

}

void MainWindow::settingsChanged() {
    ui->view->settingsChanged();
}
