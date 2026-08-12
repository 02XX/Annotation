#pragma once

#include <QObject>

class QUndoStack;

namespace totcad {

class GDXFModel;
class GAnnotationModel;

class GFileController final : public QObject
{
    Q_OBJECT
public:
    GFileController(GDXFModel *drawing,
                    GAnnotationModel *annotations,
                    QUndoStack *undoStack,
                    QObject *parent = nullptr);

    bool open(const QString &filePath, QString *errorMessage = nullptr);
    bool save(QString *errorMessage = nullptr);
    void close();
    bool hasDrawing() const;
    bool isModified() const;
    QString annotationFilePath() const;

signals:
    void drawingOpened(const QString &filePath);
    void drawingClosed();
    void annotationsSaved(const QString &filePath);

private:
    GDXFModel *m_drawing;
    GAnnotationModel *m_annotations;
    QUndoStack *m_undoStack;
};

} // namespace totcad
