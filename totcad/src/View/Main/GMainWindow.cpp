#include "View/Main/GMainWindow.hpp"

#include "Controller/GAnnotationController.hpp"
#include "Controller/GDocumentController.hpp"
#include "Controller/GSelectionController.hpp"
#include "Controller/GToolController.hpp"
#include "IO/GRecentFileManager.hpp"
#include "Model/Annotation/GAnnotationDocument.hpp"
#include "Model/CAD/GCADDocument.hpp"
#include "Model/GInstanceTreeModel.hpp"
#include "Model/GLayerTableModel.hpp"
#include "Model/GTypeTableModel.hpp"
#include "View/CAD/GCADScene.hpp"
#include "View/CAD/GCADView.hpp"
#include "View/Dialog/GAboutDialog.hpp"
#include "View/Dock/GInstanceDockWidget.hpp"
#include "View/Dock/GLayerDockWidget.hpp"
#include "View/Dock/GTypeDockWidget.hpp"

#include <QAction>
#include <QActionGroup>
#include <QCloseEvent>
#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QStyle>
#include <QToolBar>
#include <QUndoStack>

namespace totcad {

GMainWindow::GMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setObjectName(QStringLiteral("AnnotationWindow"));
    resize(1440, 900);
    setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks);
    createWorkspace();
    createActions();
    createMenusAndToolbars();
    connectWorkspace();
    updateStatus();
    updateActionStates();
    updateWindowTitle();
}

void GMainWindow::createWorkspace()
{
    m_cadDocument = new GCADDocument(this);
    m_annotationDocument = new GAnnotationDocument(this);
    m_undoStack = new QUndoStack(this);
    m_cadScene = new GCADScene(this);
    m_cadScene->setDocuments(m_cadDocument, m_annotationDocument);
    m_cadView = new GCADView(m_cadScene, this);
    setCentralWidget(m_cadView);

    m_documentController = new GDocumentController(m_cadDocument, m_annotationDocument, m_undoStack, this);
    m_annotationController = new GAnnotationController(m_annotationDocument, m_undoStack, this);
    m_selectionController = new GSelectionController(m_cadScene, this);
    m_toolController = new GToolController(m_cadView, this);

    m_typeModel = new GTypeTableModel(m_annotationDocument, m_undoStack, this);
    m_instanceModel = new GInstanceTreeModel(m_annotationDocument, m_undoStack, this);
    m_layerModel = new GLayerTableModel(this);

    m_typeDock = new GTypeDockWidget(this);
    m_typeDock->setModel(m_typeModel);
    m_instanceDock = new GInstanceDockWidget(this);
    m_instanceDock->setModel(m_instanceModel);
    m_layerDock = new GLayerDockWidget(this);
    m_layerDock->setModel(m_layerModel);
    addDockWidget(Qt::LeftDockWidgetArea, m_typeDock);
    addDockWidget(Qt::LeftDockWidgetArea, m_instanceDock);
    addDockWidget(Qt::RightDockWidgetArea, m_layerDock);
    m_layerDock->hide();

    m_pathLabel = new QLabel(tr("未打开文件"), this);
    m_entityCountLabel = new QLabel(this);
    m_typeCountLabel = new QLabel(this);
    m_instanceCountLabel = new QLabel(this);
    m_coordinateLabel = new QLabel(this);
    m_pathLabel->setMinimumWidth(280);
    statusBar()->addWidget(m_pathLabel, 1);
    statusBar()->addPermanentWidget(m_entityCountLabel);
    statusBar()->addPermanentWidget(m_typeCountLabel);
    statusBar()->addPermanentWidget(m_instanceCountLabel);
    statusBar()->addPermanentWidget(m_coordinateLabel);
}

void GMainWindow::createActions()
{
    m_openAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("打开文件…"), this);
    m_openAction->setObjectName(QStringLiteral("OpenFileAction"));
    m_openAction->setShortcut(QKeySequence::Open);
    m_openAction->setStatusTip(tr("打开 DXF 文件"));
    m_saveAction = new QAction(tr("保存"), this);
    m_saveAction->setObjectName(QStringLiteral("SaveFileAction"));
    m_saveAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    m_saveAction->setShortcut(QKeySequence::Save);
    m_closeAction = new QAction(tr("关闭"), this);
    m_closeAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F4));
    m_undoAction = m_undoStack->createUndoAction(this, tr("撤销"));
    m_undoAction->setShortcut(QKeySequence::Undo);
    m_redoAction = m_undoStack->createRedoAction(this, tr("重复"));
    m_redoAction->setShortcuts({QKeySequence::Redo, QKeySequence(Qt::CTRL | Qt::Key_Y)});
    m_typeAnnotationAction = new QAction(tr("类型标注"), this);
    m_typeAnnotationAction->setCheckable(true);
    m_instanceAnnotationAction = new QAction(tr("实例标注"), this);
    m_instanceAnnotationAction->setCheckable(true);
    m_showAllAction = new QAction(tr("显示全图"), this);
    m_realtimeZoomAction = new QAction(tr("实时缩放"), this);
    m_realtimeZoomAction->setCheckable(true);
    m_windowZoomAction = new QAction(tr("窗口缩放"), this);
    m_windowZoomAction->setCheckable(true);
    m_panAction = new QAction(tr("实时平移"), this);
    m_panAction->setCheckable(true);

    auto *toolGroup = new QActionGroup(this);
    toolGroup->setExclusive(true);
    toolGroup->addAction(m_typeAnnotationAction);
    toolGroup->addAction(m_instanceAnnotationAction);
    toolGroup->addAction(m_realtimeZoomAction);
    toolGroup->addAction(m_windowZoomAction);
    toolGroup->addAction(m_panAction);
}

void GMainWindow::createMenusAndToolbars()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("文件"));
    fileMenu->addAction(m_openAction);
    fileMenu->addAction(m_saveAction);
    fileMenu->addAction(m_closeAction);
    fileMenu->addSeparator();
    m_recentMenu = fileMenu->addMenu(tr("最近打开文件"));
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction(tr("退出"));

    QMenu *editMenu = menuBar()->addMenu(tr("编辑"));
    editMenu->addAction(m_undoAction);
    editMenu->addAction(m_redoAction);
    editMenu->addSeparator();
    editMenu->addAction(m_typeAnnotationAction);
    editMenu->addAction(m_instanceAnnotationAction);

    QMenu *viewMenu = menuBar()->addMenu(tr("视图"));
    QMenu *visibilityMenu = viewMenu->addMenu(tr("界面可见性控制"));
    visibilityMenu->addAction(m_typeDock->toggleViewAction());
    visibilityMenu->addAction(m_instanceDock->toggleViewAction());
    visibilityMenu->addAction(m_layerDock->toggleViewAction());
    QAction *statusBarAction = visibilityMenu->addAction(tr("状态栏"));
    statusBarAction->setCheckable(true);
    statusBarAction->setChecked(true);
    connect(statusBarAction, &QAction::toggled, statusBar(), &QStatusBar::setVisible);
    viewMenu->addSeparator();
    viewMenu->addAction(m_showAllAction);
    viewMenu->addAction(m_realtimeZoomAction);
    viewMenu->addAction(m_windowZoomAction);
    viewMenu->addAction(m_panAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("帮助"));
    QAction *helpAction = helpMenu->addAction(tr("帮助"));
    QAction *aboutAction = helpMenu->addAction(tr("关于"));

    m_fileToolBar = addToolBar(tr("文件"));
    m_fileToolBar->setObjectName(QStringLiteral("FileToolBar"));
    m_fileToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    m_fileToolBar->addAction(m_openAction);
    m_fileToolBar->addAction(m_saveAction);
    m_editToolBar = addToolBar(tr("编辑"));
    m_editToolBar->setObjectName(QStringLiteral("EditToolBar"));
    m_editToolBar->addAction(m_undoAction);
    m_editToolBar->addAction(m_redoAction);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_typeAnnotationAction);
    m_editToolBar->addAction(m_instanceAnnotationAction);
    m_viewToolBar = addToolBar(tr("视图"));
    m_viewToolBar->setObjectName(QStringLiteral("ViewToolBar"));
    m_viewToolBar->addAction(m_showAllAction);
    m_viewToolBar->addAction(m_realtimeZoomAction);
    m_viewToolBar->addAction(m_windowZoomAction);
    m_viewToolBar->addAction(m_panAction);
    visibilityMenu->addSeparator();
    visibilityMenu->addAction(m_fileToolBar->toggleViewAction());
    visibilityMenu->addAction(m_editToolBar->toggleViewAction());
    visibilityMenu->addAction(m_viewToolBar->toggleViewAction());

    connect(m_openAction, &QAction::triggered, this, &GMainWindow::showOpenFileDialog);
    connect(m_saveAction, &QAction::triggered, this, &GMainWindow::saveDocument);
    connect(m_closeAction, &QAction::triggered, this, &GMainWindow::closeDocument);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    connect(m_showAllAction, &QAction::triggered, m_cadView, &GCADView::showAll);
    connect(m_typeAnnotationAction, &QAction::triggered, this, [this] {
        if (m_typeDock->currentTypeId().isEmpty()) {
            QMessageBox::information(this, tr("类型标注"), tr("请先新增并选中一个类型。"));
            m_typeAnnotationAction->setChecked(false);
            return;
        }
        m_toolController->activate(GToolController::Mode::TypeAnnotation);
        statusBar()->showMessage(tr("选择图元，右键确认；Esc 清除选择/退出命令"));
    });
    connect(m_instanceAnnotationAction, &QAction::triggered, this, [this] {
        if (m_instanceDock->currentInstanceId().isEmpty()) {
            QMessageBox::information(this, tr("实例标注"), tr("请先新增并选中一个实例。"));
            m_instanceAnnotationAction->setChecked(false);
            return;
        }
        m_toolController->activate(GToolController::Mode::InstanceAnnotation);
        statusBar()->showMessage(tr("选择图元，右键确认；Esc 清除选择/退出命令"));
    });
    connect(m_realtimeZoomAction, &QAction::triggered, this, [this] { m_toolController->activate(GToolController::Mode::RealtimeZoom); });
    connect(m_windowZoomAction, &QAction::triggered, this, [this] { m_toolController->activate(GToolController::Mode::WindowZoom); });
    connect(m_panAction, &QAction::triggered, this, [this] { m_toolController->activate(GToolController::Mode::Pan); });
    connect(helpAction, &QAction::triggered, this, [this] {
        QMessageBox::information(this, tr("帮助"),
                                 tr("打开 DXF 后，在类型或实例面板中创建记录。\n"
                                    "进入标注命令，点选或框选图元，右键确认。\n"
                                    "滚轮缩放，中键拖动平移，Esc 取消。"));
    });
    connect(aboutAction, &QAction::triggered, this, [this] { GAboutDialog(this).exec(); });
    updateRecentFiles();
}

void GMainWindow::showOpenFileDialog()
{
    QFileDialog dialog(this, tr("打开 DXF 文件"));
    dialog.setObjectName(QStringLiteral("OpenDXFFileDialog"));
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setNameFilters({tr("DXF 文件 (*.dxf)"), tr("所有文件 (*)")});
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.resize(900, 600);

    if (dialog.exec() != QDialog::Accepted)
        return;
    const QStringList selectedFiles = dialog.selectedFiles();
    if (!selectedFiles.isEmpty())
        openFile(selectedFiles.first());
}

void GMainWindow::connectWorkspace()
{
    connect(m_typeDock, &GTypeDockWidget::addRequested, this, [this] {
        m_annotationController->addType();
        m_typeDock->selectRow(m_typeModel->rowCount() - 1);
    });
    connect(m_typeDock, &GTypeDockWidget::deleteRequested,
            m_annotationController, &GAnnotationController::deleteType);
    connect(m_typeDock, &GTypeDockWidget::moveRequested, this,
            [this](const QString &id, int offset) {
                const int newRow = m_typeDock->currentRow() + offset;
                m_annotationController->moveType(id, offset);
                m_typeDock->selectRow(newRow);
            });
    connect(m_typeDock, &GTypeDockWidget::colorRequested, this, [this](const QString &id) {
        const GAnnotationType *type = m_annotationDocument->type(id);
        if (!type) return;
        const QColor color = QColorDialog::getColor(type->color, this, tr("选择标注颜色"));
        if (color.isValid()) m_annotationController->changeTypeColor(id, color);
    });
    connect(m_typeDock, &GTypeDockWidget::currentTypeChanged,
            m_instanceModel, &GInstanceTreeModel::setCurrentType);
    connect(m_instanceDock, &GInstanceDockWidget::addRequested, this, [this] {
        m_annotationController->addInstance(m_typeDock->currentTypeId());
    });
    connect(m_instanceDock, &GInstanceDockWidget::deleteRequested,
            m_annotationController, &GAnnotationController::deleteInstance);
    connect(m_instanceDock, &GInstanceDockWidget::instanceActivated, this, [this](const QString &instanceId) {
        m_cadView->zoomToSceneRect(m_cadScene->instanceBounds(instanceId), 0.8);
    });
    connect(m_toolController, &GToolController::typeAssignmentRequested, this, [this](const QStringList &ids) {
        m_annotationController->assignType(ids, m_typeDock->currentTypeId());
    });
    connect(m_toolController, &GToolController::instanceAssignmentRequested, this, [this](const QStringList &ids) {
        m_annotationController->assignInstance(ids, m_instanceDock->currentInstanceId());
    });
    connect(m_toolController, &GToolController::modeChanged, this, [this](GToolController::Mode mode) {
        m_typeAnnotationAction->setChecked(mode == GToolController::Mode::TypeAnnotation);
        m_instanceAnnotationAction->setChecked(mode == GToolController::Mode::InstanceAnnotation);
        m_panAction->setChecked(mode == GToolController::Mode::Pan);
        m_realtimeZoomAction->setChecked(mode == GToolController::Mode::RealtimeZoom);
        m_windowZoomAction->setChecked(mode == GToolController::Mode::WindowZoom);
    });
    connect(m_cadView, &GCADView::mouseScenePositionChanged, this, [this](const QPointF &point) {
        m_coordinateLabel->setText(tr("X: %1  Y: %2").arg(point.x(), 0, 'f', 2).arg(-point.y(), 0, 'f', 2));
    });
    const auto refresh = [this] { updateStatus(); updateActionStates(); updateWindowTitle(); };
    connect(m_annotationDocument, &GAnnotationDocument::typesChanged, this, refresh);
    connect(m_annotationDocument, &GAnnotationDocument::instancesChanged, this, refresh);
    connect(m_annotationDocument, &GAnnotationDocument::assignmentsChanged, this, refresh);
    connect(m_annotationDocument, &GAnnotationDocument::dirtyChanged, this, refresh);
    connect(m_undoStack, &QUndoStack::cleanChanged, this, [this] { updateActionStates(); updateWindowTitle(); });
}

bool GMainWindow::openFile(const QString &filePath)
{
    if (!confirmSaveIfModified())
        return false;
    QString error;
    if (!m_documentController->open(filePath, &error)) {
        QMessageBox::critical(this, tr("打开失败"), error);
        updateRecentFiles();
        return false;
    }
    m_layerModel->setDocument(m_cadDocument);
    m_cadScene->rebuild();
    m_cadView->showAll();
    updateRecentFiles();
    updateStatus();
    updateActionStates();
    updateWindowTitle();
    return true;
}

bool GMainWindow::saveDocument()
{
    QString error;
    if (m_documentController->save(&error)) {
        statusBar()->showMessage(tr("已保存：%1").arg(m_documentController->annotationFilePath()), 4000);
        updateActionStates();
        updateWindowTitle();
        return true;
    }
    QMessageBox::critical(this, tr("保存失败"), error);
    return false;
}

bool GMainWindow::confirmSaveIfModified()
{
    if (!m_documentController->isModified())
        return true;
    const auto result = QMessageBox::warning(this, tr("未保存的标注"),
                                             tr("当前标注已修改，是否保存？"),
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                                             QMessageBox::Save);
    if (result == QMessageBox::Cancel)
        return false;
    return result == QMessageBox::Discard || saveDocument();
}

void GMainWindow::closeDocument()
{
    if (!confirmSaveIfModified())
        return;
    m_toolController->activate(GToolController::Mode::Select);
    m_documentController->close();
    m_layerModel->setDocument(nullptr);
    m_cadScene->rebuild();
    m_instanceModel->setCurrentType({});
    updateStatus();
    updateActionStates();
    updateWindowTitle();
}

void GMainWindow::updateRecentFiles()
{
    m_recentMenu->clear();
    const QStringList files = GRecentFileManager{}.files();
    if (files.isEmpty()) {
        QAction *empty = m_recentMenu->addAction(tr("（无）"));
        empty->setEnabled(false);
        return;
    }
    for (const QString &path : files) {
        QAction *action = m_recentMenu->addAction(QFileInfo(path).fileName());
        action->setToolTip(path);
        connect(action, &QAction::triggered, this, [this, path] {
            if (!QFileInfo::exists(path)) {
                GRecentFileManager{}.removeFile(path);
                updateRecentFiles();
                QMessageBox::warning(this, tr("文件不存在"), tr("最近文件已不存在：\n%1").arg(path));
                return;
            }
            openFile(path);
        });
    }
}

void GMainWindow::updateStatus()
{
    const bool open = m_documentController->hasDocument();
    m_pathLabel->setText(open ? m_cadDocument->sourcePath : tr("未打开文件"));
    m_entityCountLabel->setText(tr("实体：%1").arg(open ? m_cadDocument->entityCount() : 0));
    m_typeCountLabel->setText(tr("类型标注：%1").arg(m_annotationDocument->assignedTypeEntityCount()));
    m_instanceCountLabel->setText(tr("实例标注：%1").arg(m_annotationDocument->assignedInstanceEntityCount()));
}

void GMainWindow::updateActionStates()
{
    const bool open = m_documentController->hasDocument();
    m_saveAction->setEnabled(open && m_documentController->isModified());
    m_closeAction->setEnabled(open);
    m_typeAnnotationAction->setEnabled(open);
    m_instanceAnnotationAction->setEnabled(open);
    m_showAllAction->setEnabled(open);
    m_realtimeZoomAction->setEnabled(open);
    m_windowZoomAction->setEnabled(open);
    m_panAction->setEnabled(open);
}

void GMainWindow::updateWindowTitle()
{
    QString title = tr("矢量图 AI 标注工具");
    if (m_documentController->hasDocument())
        title = QStringLiteral("%1%2 — %3")
                    .arg(m_documentController->isModified() ? QStringLiteral("*") : QString{},
                         QFileInfo(m_cadDocument->sourcePath).fileName(), title);
    setWindowTitle(title);
}

void GMainWindow::closeEvent(QCloseEvent *event)
{
    if (confirmSaveIfModified())
        event->accept();
    else
        event->ignore();
}

} // namespace totcad
