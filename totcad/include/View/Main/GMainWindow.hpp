#pragma once

#include <QMainWindow>

class QAction;
class QLabel;
class QMenu;
class QToolBar;
class QUndoStack;

namespace totcad {

class GCADDocument;
class GAnnotationDocument;
class GCADScene;
class GCADView;
class GDocumentController;
class GAnnotationController;
class GSelectionController;
class GToolController;
class GTypeTableModel;
class GInstanceTreeModel;
class GLayerTableModel;
class GTypeDockWidget;
class GInstanceDockWidget;
class GLayerDockWidget;

class GMainWindow final : public QMainWindow
{
    Q_OBJECT
public:
    explicit GMainWindow(QWidget *parent = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createWorkspace();
    void createActions();
    void createMenusAndToolbars();
    void connectWorkspace();
    bool openFile(const QString &filePath);
    bool saveDocument();
    bool confirmSaveIfModified();
    void closeDocument();
    void updateRecentFiles();
    void updateStatus();
    void updateActionStates();
    void updateWindowTitle();

    GCADDocument *m_cadDocument{nullptr};
    GAnnotationDocument *m_annotationDocument{nullptr};
    QUndoStack *m_undoStack{nullptr};
    GCADScene *m_cadScene{nullptr};
    GCADView *m_cadView{nullptr};
    GDocumentController *m_documentController{nullptr};
    GAnnotationController *m_annotationController{nullptr};
    GSelectionController *m_selectionController{nullptr};
    GToolController *m_toolController{nullptr};
    GTypeTableModel *m_typeModel{nullptr};
    GInstanceTreeModel *m_instanceModel{nullptr};
    GLayerTableModel *m_layerModel{nullptr};
    GTypeDockWidget *m_typeDock{nullptr};
    GInstanceDockWidget *m_instanceDock{nullptr};
    GLayerDockWidget *m_layerDock{nullptr};

    QMenu *m_recentMenu{nullptr};
    QToolBar *m_fileToolBar{nullptr};
    QToolBar *m_editToolBar{nullptr};
    QToolBar *m_viewToolBar{nullptr};
    QAction *m_saveAction{nullptr};
    QAction *m_closeAction{nullptr};
    QAction *m_undoAction{nullptr};
    QAction *m_redoAction{nullptr};
    QAction *m_typeAnnotationAction{nullptr};
    QAction *m_instanceAnnotationAction{nullptr};
    QAction *m_showAllAction{nullptr};
    QAction *m_realtimeZoomAction{nullptr};
    QAction *m_windowZoomAction{nullptr};
    QAction *m_panAction{nullptr};
    QLabel *m_pathLabel{nullptr};
    QLabel *m_entityCountLabel{nullptr};
    QLabel *m_typeCountLabel{nullptr};
    QLabel *m_instanceCountLabel{nullptr};
    QLabel *m_coordinateLabel{nullptr};
};

} // namespace totcad
