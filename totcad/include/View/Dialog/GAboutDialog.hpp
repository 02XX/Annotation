#pragma once
#include <QDialog>
namespace totcad {
class GAboutDialog final : public QDialog {
    Q_OBJECT
public: explicit GAboutDialog(QWidget *parent = nullptr);
};
} // namespace totcad
