#include <QDebug>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLine>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QCheckBox>

#include "window_settings.h"

SettingsDialog::SettingsDialog(QWidget* parent):
    QDialog(parent)
{
    /*
     * Assume that the QSettings instance has already been initialized with organization- and application-name.
     * The actual settings entries have to be created beforehand.
     */ 

    setStyleSheet("font-size:16px;");
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Einstellungen");

    _addFields();
}


void SettingsDialog::_addFields()
{
    const QStringList keys = _settings.allKeys();
    for (const QString& key : keys) {
        Setting setting;
        setting.key = key;
        setting.defaultValue = _settings.value(key);
        setting.type = _guessType(key);
        if (setting.type == Type::integer) {
            QSpinBox* widget = new QSpinBox();
            widget->setMaximum(10'000);
            widget->setMinimum(-10'000);
            widget->setValue(setting.defaultValue.toInt());
            setting.widget = widget;
        } else if (setting.type == Type::boolean) {
            QCheckBox* widget = new QCheckBox();
            widget->setChecked(setting.defaultValue.toBool());
            setting.widget = widget;
        }
        _settingList.append(setting);
    }

    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Apply | QDialogButtonBox::Close | QDialogButtonBox::Ok, this);
    const QPushButton* applyButton =_buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, &QPushButton::pressed, this, &SettingsDialog::_slotSave);
    connect(_buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QFrame* hLine = new QFrame();
    hLine->setFrameShape(QFrame::HLine);
    hLine->setFrameShadow(QFrame::Sunken);

    QFormLayout* form = new QFormLayout;
    for (const Setting& s : _settingList) {
        form->addRow(s.key, s.widget);
        if (s.type == Type::boolean)
            connect(s.widget, SIGNAL(stateChanged(int)), this, SLOT(_slotSomethingChanged(int)));
        else if (s.type == Type::integer)
            connect(s.widget, SIGNAL(valueChanged(int)), this, SLOT(_slotSomethingChanged(int)));
    }
    form->addRow(hLine);
    form->addRow(_buttonBox);
    _buttonBox->show();
    setLayout(form);
}

void SettingsDialog::accept()
{
    _save();
    QDialog::accept();
}

void SettingsDialog::_save()
{
    for (Setting& s : _settingList) {
        if (s.type == Type::integer) {
            const int newValue = static_cast<QSpinBox*>(s.widget)->value();
            s.defaultValue = QVariant(newValue);
            _settings.setValue(s.key, newValue);
        } else if (s.type == Type::boolean) {
            const bool newValue = static_cast<QCheckBox*>(s.widget)->isChecked();
            s.defaultValue = QVariant(newValue);
            _settings.setValue(s.key, newValue);
        }
    }
}

SettingsDialog::Type SettingsDialog::_guessType(const QString& key) const {
    const QString value = _settings.value(key).toString();

    qDebug() << value;

    if (value == "true" or value == "false") {
        return Type::boolean;
    }
    bool ok;
    value.toInt(&ok);
    if (ok) {
        return Type::integer;
    }

    return Type::unknown;
}

void SettingsDialog::_slotSomethingChanged(int)
{
    if (nullptr == _buttonBox) {
        return;
    }
    QPushButton* applyButton = _buttonBox->button(QDialogButtonBox::StandardButton::Apply);
    if (nullptr == applyButton) {
        return;
    }
    applyButton->setEnabled(true);
}

void SettingsDialog::_slotSave(void)
{
    if (nullptr == _buttonBox) {
        return;
    }
    QPushButton* applyButton = _buttonBox->button(QDialogButtonBox::StandardButton::Apply);
    if (nullptr == applyButton) {
        return;
    }
    _save();
    applyButton->setEnabled(false);
}
