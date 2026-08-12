#pragma once

#include <QObject>

class QUndoStack;

namespace totcad {

class GCADDocument;
class GAnnotationDocument;

class GDocumentController final : public QObject
{
    Q_OBJECT
public:
    GDocumentController(GCADDocument *cadDocument,
                        GAnnotationDocument *annotationDocument,
                        QUndoStack *undoStack,
                        QObject *parent = nullptr);

    bool open(const QString &filePath, QString *errorMessage = nullptr);
    bool save(QString *errorMessage = nullptr);
    void close();
    bool hasDocument() const;
    bool isModified() const;
    QString annotationFilePath() const;

signals:
    void documentOpened(const QString &filePath);
    void documentClosed();
    void documentSaved(const QString &filePath);

private:
    GCADDocument *m_cadDocument;
    GAnnotationDocument *m_annotationDocument;
    QUndoStack *m_undoStack;
};

} // namespace totcad
