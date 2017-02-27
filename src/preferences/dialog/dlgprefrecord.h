/**
* @file encoderflacsettings.cpp
* @author John Sully
* @author Josep Maria Antolín
* @date Feb 27 2017
* @brief storage of setting for flac encoder
*/

#ifndef DLGPREFRECORD_H
#define DLGPREFRECORD_H

#include <QRadioButton>
#include <QWidget>

#include "preferences/dialog/ui_dlgprefrecorddlg.h"
#include "preferences/usersettings.h"
#include "preferences/dlgpreferencepage.h"
#include "encoder/encoder.h"

class ControlObject;
class ControlProxy;

class DlgPrefRecord : public DlgPreferencePage, public Ui::DlgPrefRecordDlg  {
    Q_OBJECT
  public:
    DlgPrefRecord(QWidget *parent, UserSettingsPointer _config);
    virtual ~DlgPrefRecord();

  public slots:
    // Apply changes to widget
    void slotApply();
    void slotUpdate();
    void slotResetToDefaults();

    // Dialog to browse for recordings directory
    void slotBrowseRecordingsDir();

    void slotFormatChanged();
    void slotSliderQuality();
    void slotSliderCompression();
    void slotGroupChanged();

  signals:
    void apply(const QString &);

  private:
    void setupEncoderUI(Encoder::Format selformat);
    void loadMetaData();
    void updateTextQuality();
    void updateTextCompression();
    void saveRecordingFolder();
    void saveMetaData();
    void saveEncoding();
    void saveUseCueFile();
    void saveSplitSize();

    // Pointer to config object
    UserSettingsPointer m_pConfig;
    Encoder::Format m_selFormat;
    QList<QRadioButton*> m_formatButtonss;
    QList<QWidget*> m_optionWidgets;
};

#endif
