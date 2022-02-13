# QtAutoSettingsDialog

Automatically generates a dialog where you can change the values of an applications QSettings instance.

The dialog will guess the type of the values and then generate a form.
For example it will use a checkbox to represent a boolean setting.

At the moment these types are supported:

|Type|Widget used|
|---|---|
|bool|QCheckBox|
|int|QSpinBox|