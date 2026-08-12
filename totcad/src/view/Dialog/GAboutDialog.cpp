#include "View/Dialog/GAboutDialog.hpp"
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>
namespace totcad {
GAboutDialog::GAboutDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("关于"));
    setModal(true);
    auto *layout = new QVBoxLayout(this);
    auto *title = new QLabel(QStringLiteral("<h2>矢量图 AI 标注工具</h2>"), this);
    title->setAlignment(Qt::AlignCenter);
    auto *description = new QLabel(tr("用于浏览和标注 DXF 矢量图。\nTOT26 C++ 大作业"), this);
    description->setAlignment(Qt::AlignCenter);
    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    layout->addWidget(title);
    layout->addWidget(description);
    layout->addWidget(buttons);
    resize(360, 180);
}
} // namespace totcad
