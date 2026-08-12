#include "Controller/GFileController.hpp"

#include "IO/Annotation/GAnnotationSerializer.hpp"
#include "IO/DXF/GDXFLoader.hpp"
#include "IO/GRecentFileManager.hpp"
#include "Model/GAnnotationModel.hpp"
#include "Model/GDXFModel.hpp"

#include <QFileInfo>
#include <QUndoStack>

#include <string>
#include <utility>

namespace totcad {

GFileController::GFileController(GDXFModel *drawing,
                                 GAnnotationModel *annotations,
                                 QUndoStack *undoStack,
                                 QObject *parent)
    : QObject(parent),
      m_drawing(drawing),
      m_annotations(annotations),
      m_undoStack(undoStack)
{
    connect(m_undoStack, &QUndoStack::cleanChanged, this, [this](bool clean) {
        if (clean)
            m_annotations->setClean();
    });
}

bool GFileController::open(const QString &filePath, QString *errorMessage)
{
    GDXFModel parsedDrawing;
    std::string dxfError;
    if (!GDXFLoader{}.load(filePath.toUtf8().toStdString(), parsedDrawing, &dxfError)) {
        if (errorMessage)
            *errorMessage = QString::fromStdString(dxfError);
        return false;
    }

    const QFileInfo dxfInfo(filePath);
    const QString annotationPath = dxfInfo.absolutePath() + QLatin1Char('/')
                                   + dxfInfo.completeBaseName() + QStringLiteral(".json");
    GAnnotationModel parsedAnnotations;
    if (QFileInfo::exists(annotationPath)
        && !GAnnotationSerializer{}.load(annotationPath, parsedAnnotations, errorMessage)) {
        return false;
    }
    m_drawing->replaceWith(std::move(parsedDrawing));
    m_annotations->restore(parsedAnnotations.snapshot(), false);
    m_annotations->setClean();
    m_undoStack->clear();
    m_undoStack->setClean();
    GRecentFileManager{}.addFile(filePath);
    emit drawingOpened(filePath);
    return true;
}

bool GFileController::save(QString *errorMessage)
{
    if (!hasDrawing()) {
        if (errorMessage)
            *errorMessage = tr("当前没有打开的 DXF 文件");
        return false;
    }
    if (!GAnnotationSerializer{}.save(annotationFilePath(), *m_annotations, errorMessage))
        return false;
    m_annotations->setClean();
    m_undoStack->setClean();
    emit annotationsSaved(annotationFilePath());
    return true;
}

void GFileController::close()
{
    m_drawing->clear();
    m_annotations->clear();
    m_undoStack->clear();
    emit drawingClosed();
}

bool GFileController::hasDrawing() const
{
    return !m_drawing->sourcePath().isEmpty();
}

bool GFileController::isModified() const
{
    return hasDrawing() && (!m_undoStack->isClean() || m_annotations->isDirty());
}

QString GFileController::annotationFilePath() const
{
    const QFileInfo info(m_drawing->sourcePath());
    return info.absolutePath() + QLatin1Char('/') + info.completeBaseName() + QStringLiteral(".json");
}

} // namespace totcad
