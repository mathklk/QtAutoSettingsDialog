#ifndef WINDOW_SETTINGS_H
#define WINDOW_SETTINGS_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QObject>
#include <QSettings>

class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget* parent = nullptr);

    enum class Type {unknown, boolean, integer};

    void accept(void) override;

private slots:
    void _slotSomethingChanged(int);
    void _slotSave(void);

private:
    struct Setting
    {
        QString key;
        QVariant defaultValue;
        Type type;
        QWidget* widget;
    };

    void _addFields(void);
    Type _guessType(const QString& settingsKey) const;
    void _save(void);

    QDialogButtonBox* _buttonBox = nullptr;

    QSettings _settings;
    QVector<Setting> _settingList;

};

#endif // WINDOW_SETTINGS_H
