/******************************************************************************\
 * Copyright (c) 2004-2020
 *
 * Author(s):
 *  Volker Fischer
 *
 ******************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
\******************************************************************************/

#include "clientsettingsdlg.h"


/* Implementation *************************************************************/
CClientSettingsDlg::CClientSettingsDlg ( CClient* pNCliP, QWidget* parent,
    Qt::WindowFlags f ) : QDialog ( parent, f ), pClient ( pNCliP )
{
    setupUi ( this );


    // Add help text to controls -----------------------------------------------
    // jitter buffer
    QString strJitterBufferSize = "<b>" + tr ( "Jitter Buffer Size" ) + ":</b> " + tr (
        "The jitter buffer compensates for network and sound card timing jitters. The "
        "size of this jitter buffer has therefore influence on the quality of "
        "the audio stream (how many dropouts occur) and the overall delay "
        "(the longer the buffer, the higher the delay)." ) + "<br>" + tr (
        "The jitter buffer size can be manually chosen for the local client "
        "and the remote server. For the local jitter buffer, dropouts in the "
        "audio stream are indicated by the light on the bottom "
        "of the jitter buffer size faders. If the light turns to red, a buffer "
        "overrun/underrun took place and the audio stream is interrupted." ) + "<br>" + tr (
        "The jitter buffer setting is therefore a trade-off between audio "
        "quality and overall delay." ) + "<br>" + tr (
        "An auto setting of the jitter buffer size setting is available. If "
        "the check Auto is enabled, the jitter buffers of the local client and "
        "the remote server are set automatically "
        "based on measurements of the network and sound card timing jitter. If "
        "the Auto check is enabled, the jitter buffer size faders are "
        "disabled (they cannot be moved with the mouse)." );

    QString strJitterBufferSizeTT = tr ( "In case the auto setting of the "
        "jitter buffer is enabled, the network buffers of the local client and "
        "the remote server are set to a conservative "
        "value to minimize the audio dropout probability. To tweak the "
        "audio delay/latency it is recommended to disable the auto setting "
        "functionality and to lower the jitter buffer size manually by "
        "using the sliders until your personal acceptable limit of the amount "
        "of dropouts is reached. The LED indicator will visualize the audio "
        "dropouts of the local jitter buffer by a red light." ) +
        TOOLTIP_COM_END_TEXT;

    lblNetBuf->setWhatsThis            ( strJitterBufferSize );
    lblNetBuf->setToolTip              ( strJitterBufferSizeTT );
    grbJitterBuffer->setWhatsThis      ( strJitterBufferSize );
    grbJitterBuffer->setToolTip        ( strJitterBufferSizeTT );
    sldNetBuf->setWhatsThis            ( strJitterBufferSize );
    sldNetBuf->setAccessibleName       ( tr ( "Local jitter buffer slider control" ) );
    sldNetBuf->setToolTip              ( strJitterBufferSizeTT );
    sldNetBufServer->setWhatsThis      ( strJitterBufferSize );
    sldNetBufServer->setAccessibleName ( tr ( "Server jitter buffer slider control" ) );
    sldNetBufServer->setToolTip        ( strJitterBufferSizeTT );
    chbAutoJitBuf->setAccessibleName   ( tr ( "Auto jitter buffer switch" ) );
    chbAutoJitBuf->setToolTip          ( strJitterBufferSizeTT );
    ledNetw->setAccessibleName         ( tr ( "Jitter buffer status LED indicator" ) );
    ledNetw->setToolTip                ( strJitterBufferSizeTT );

    // sound card device
    cbxSoundcard->setWhatsThis ( "<b>" + tr ( "Sound Card Device" ) + ":</b> " +
        tr ( "The ASIO driver (sound card) can be selected using " ) + APP_NAME +
        tr ( " under the Windows operating system. Under MacOS/Linux, no sound "
        "card selection is possible. If the selected ASIO driver is not valid "
        "an error message is shown and the previous valid driver is selected." ) + "<br>" + tr (
        "If the driver is selected during an active connection, the connection "
        "is stopped, the driver is changed and the connection is started again "
        "automatically." ) );

    cbxSoundcard->setAccessibleName ( tr ( "Sound card device selector combo box" ) );

#ifdef _WIN32
    // set Windows specific tool tip
    cbxSoundcard->setToolTip ( tr ( "In case the ASIO4ALL driver is used, "
        "please note that this driver usually introduces approx. 10-30 ms of "
        "additional audio delay. Using a sound card with a native ASIO driver "
        "is therefore recommended." ) + "<br>" + tr ( "If you are using the kX ASIO "
        "driver, make sure to connect the ASIO inputs in the kX DSP settings "
        "panel." ) + TOOLTIP_COM_END_TEXT );
#endif

    // sound card input/output channel mapping
    QString strSndCrdChanMapp = "<b>" + tr ( "Sound Card Channel Mapping" ) + ":</b> " +
        tr ( "In case the selected sound card device offers more than one "
        "input or output channel, the Input Channel Mapping and Output "
        "Channel Mapping settings are visible." ) + "<br>" + tr (
        "For each " ) + APP_NAME + tr ( " input/output channel (Left and "
        "Right channel) a different actual sound card channel can be "
        "selected." );

    lblInChannelMapping->setWhatsThis ( strSndCrdChanMapp );
    lblOutChannelMapping->setWhatsThis ( strSndCrdChanMapp );
    cbxLInChan->setWhatsThis ( strSndCrdChanMapp );
    cbxLInChan->setAccessibleName ( tr ( "Left input channel selection combo box" ) );
    cbxRInChan->setWhatsThis ( strSndCrdChanMapp );
    cbxRInChan->setAccessibleName ( tr ( "Right input channel selection combo box" ) );
    cbxLOutChan->setWhatsThis ( strSndCrdChanMapp );
    cbxLOutChan->setAccessibleName ( tr ( "Left output channel selection combo box" ) );
    cbxROutChan->setWhatsThis ( strSndCrdChanMapp );
    cbxROutChan->setAccessibleName ( tr ( "Right output channel selection combo box" ) );

    // enable OPUS64
    chbEnableOPUS64->setWhatsThis ( "<b>" + tr ( "Enable Small Network Buffers" ) + ":</b> " + tr (
        "If enabled, the support for very small network audio packets is activated. Very small "
        "network packets are only actually used if the sound card buffer delay is smaller than " ) +
        QString().setNum ( DOUBLE_SYSTEM_FRAME_SIZE_SAMPLES ) + tr ( " samples. The "
        "smaller the network buffers, the smaller the audio latency. But at the same time "
        "the network load increases and the probability of audio dropouts also increases." ) );

    chbEnableOPUS64->setAccessibleName ( tr ( "Enable small network buffers check box" ) );

    // sound card buffer delay
    QString strSndCrdBufDelay = "<b>" + tr ( "Sound Card Buffer Delay" ) + ":</b> " +
        tr ( "The buffer delay setting is a fundamental setting of the " ) +
        APP_NAME + tr ( " software. This setting has influence on many "
        "connection properties." ) + "<br>" + tr (
        "Three buffer sizes are supported" ) +
        ":<ul>"
        "<li>" + tr ( "64 samples: This is the preferred setting since it gives lowest "
        "latency but does not work with all sound cards." ) + "</li>"
        "<li>" + tr ( "128 samples: This setting should work on most of the available "
        "sound cards." ) + "</li>"
        "<li>" + tr ( "256 samples: This setting should only be used if only a very slow "
        "computer or a slow internet connection is available." ) + "</li>"
        "</ul>" + tr (
        "Some sound card driver do not allow the buffer delay to be changed "
        "from within the " ) + APP_NAME +
        tr ( " software. In this case the buffer delay setting "
        "is disabled. To change the actual buffer delay, this "
        "setting has to be changed in the sound card driver. On Windows, press "
        "the ASIO Setup button to open the driver settings panel. On Linux, "
        "use the Jack configuration tool to change the buffer size." ) + "<br>" + tr (
        "If no buffer size is selected and all settings are disabled, an "
        "unsupported buffer size is used by the driver. The " ) + APP_NAME +
        tr ( " software will still work with this setting but with restricted "
        "performance." ) + "<br>" + tr (
        "The actual buffer delay has influence on the connection status, the "
        "current upload rate and the overall delay. The lower the buffer size, "
        "the higher the probability of red light in the status indicator (drop "
        "outs) and the higher the upload rate and the lower the overall "
        "delay." ) + "<br>" + tr (
        "The buffer setting is therefore a trade-off between audio "
        "quality and overall delay." );

    QString strSndCrdBufDelayTT = tr ( "If the buffer delay settings are "
        "disabled, it is prohibited by the audio driver to modify this "
        "setting from within the " ) + APP_NAME +
        tr ( " software. On Windows, press the ASIO Setup button to open the "
        "driver settings panel. On Linux, use the Jack configuration tool to "
        "change the buffer size." ) + TOOLTIP_COM_END_TEXT;

    rbtBufferDelayPreferred->setWhatsThis ( strSndCrdBufDelay );
    rbtBufferDelayPreferred->setAccessibleName ( tr ( "128 samples setting radio button" ) );
    rbtBufferDelayPreferred->setToolTip ( strSndCrdBufDelayTT );
    rbtBufferDelayDefault->setWhatsThis ( strSndCrdBufDelay );
    rbtBufferDelayDefault->setAccessibleName ( tr ( "256 samples setting radio button" ) );
    rbtBufferDelayDefault->setToolTip ( strSndCrdBufDelayTT );
    rbtBufferDelaySafe->setWhatsThis ( strSndCrdBufDelay );
    rbtBufferDelaySafe->setAccessibleName ( tr ( "512 samples setting radio button" ) );
    rbtBufferDelaySafe->setToolTip ( strSndCrdBufDelayTT );
    butDriverSetup->setWhatsThis ( strSndCrdBufDelay );
    butDriverSetup->setAccessibleName ( tr ( "ASIO setup push button" ) );
    butDriverSetup->setToolTip ( strSndCrdBufDelayTT );

    // fancy skin
    chbGUIDesignFancy->setWhatsThis ( "<b>" + tr ( "Fancy Skin" ) + ":</b> " + tr (
        "If enabled, a fancy skin will be applied to the main window." ) );

    chbGUIDesignFancy->setAccessibleName ( tr ( "Fancy skin check box" ) );

    // display channel levels
    chbDisplayChannelLevels->setWhatsThis ( "<b>" + tr ( "Display Channel Levels" ) + ":</b> " +
        tr ( "If enabled, each client channel will display a pre-fader level bar." ) );

    chbDisplayChannelLevels->setAccessibleName ( tr ( "Display channel levels check box" ) );

    // audio channels
    QString strAudioChannels = "<b>" + tr ( "Audio Channels" ) + ":</b> " + tr (
        "Select the number of audio channels to be used. There are three "
        "modes available. The mono and stereo modes use one and two "
        "audio channels respectively. In the mono-in/stereo-out mode "
        "the audio signal which is sent to the server is mono but the "
        "return signal is stereo. This is useful for the case that the "
        "sound card puts the instrument on one input channel and the "
        "microphone on the other channel. In that case the two input signals "
        "can be mixed to one mono channel but the server mix can be heard in "
        "stereo." ) + "<br>" + tr (
        "Enabling the stereo streaming mode will increase the "
        "stream data rate. Make sure that the current upload rate does not "
        "exceed the available bandwidth of your internet connection." ) + "<br>" + tr (
        "In case of the stereo streaming mode, no audio channel selection "
        "for the reverberation effect will be available on the main window "
        "since the effect is applied on both channels in this case." );

    lblAudioChannels->setWhatsThis ( strAudioChannels );
    cbxAudioChannels->setWhatsThis ( strAudioChannels );
    cbxAudioChannels->setAccessibleName ( tr ( "Audio channels combo box" ) );

    // audio quality
    QString strAudioQuality = "<b>" + tr ( "Audio Quality" ) + ":</b> " + tr (
        "Select the desired audio quality. A low, normal or high audio "
        "quality can be selected. The higher the audio quality, the higher "
        "the audio stream data rate. Make sure that the current "
        "upload rate does not exceed the available bandwidth of your "
        "internet connection." );

    lblAudioQuality->setWhatsThis ( strAudioQuality );
    cbxAudioQuality->setWhatsThis ( strAudioQuality );
    cbxAudioQuality->setAccessibleName ( tr ( "Audio quality combo box" ) );

    // new client fader level
    QString strNewClientLevel = "<b>" + tr ( "New Client Level" ) + ":</b> " +
        tr ( "The new client level setting defines the fader level of a new "
        "connected client in percent. I.e. if a new client connects "
        "to the current server, it will get the specified initial "
        "fader level if no other fader level of a previous connection "
        "of that client was already stored." );

    lblNewClientLevel->setWhatsThis ( strNewClientLevel );
    edtNewClientLevel->setWhatsThis ( strNewClientLevel );
    edtNewClientLevel->setAccessibleName ( tr ( "New client level edit box" ) );

    // central server address
    QString strCentrServAddr = "<b>" + tr ( "Central Server Address" ) + ":</b> " +
        tr ( "The central server address is the IP address or URL of the central server "
        "at which the server list of the connection dialog is managed. With the "
        "central server address type either the local region can be selected of "
        "the default central servers or a manual address can be specified." );

    lblCentralServerAddress->setWhatsThis ( strCentrServAddr );
    cbxCentServAddrType->setWhatsThis ( strCentrServAddr );
    edtCentralServerAddress->setWhatsThis ( strCentrServAddr );

    cbxCentServAddrType->setAccessibleName ( tr ( "Default central server type combo box" ) );
    edtCentralServerAddress->setAccessibleName ( tr ( "Central server address line edit" ) );

    // current connection status parameter
    QString strConnStats = "<b>" + tr (  "Current Connection Status "
        "Parameter" ) + ":</b> " + tr ( "The ping time is the time required for the audio "
        "stream to travel from the client to the server and backwards. This "
        "delay is introduced by the network. This delay should be as low as "
        "20-30 ms. If this delay is higher (e.g., 50-60 ms), your distance to "
        "the server is too large or your internet connection is not "
        "sufficient." ) + "<br>" + tr (
        "The overall delay is calculated from the current ping time and the "
        "delay which is introduced by the current buffer settings." ) + "<br>" + tr (
        "The upstream rate depends on the current audio packet size and the "
        "audio compression setting. Make sure that the upstream rate is not "
        "higher than the available rate (check the upstream capabilities of "
        "your internet connection by, e.g., using speedtest.net)." );

    lblPingTime->setWhatsThis          ( strConnStats );
    lblPingTimeValue->setWhatsThis     ( strConnStats );
    lblOverallDelay->setWhatsThis      ( strConnStats );
    lblOverallDelayValue->setWhatsThis ( strConnStats );
    lblUpstream->setWhatsThis          ( strConnStats );
    lblUpstreamValue->setWhatsThis     ( strConnStats );
    ledOverallDelay->setWhatsThis      ( strConnStats );
    ledOverallDelay->setToolTip ( tr ( "If this LED indicator turns red, "
        "you will not have much fun using the " ) + APP_NAME +
        tr ( " software." ) + TOOLTIP_COM_END_TEXT );


    // init driver button
#ifdef _WIN32
    butDriverSetup->setText ( tr ( "ASIO Setup" ) );
#else
    // no use for this button for MacOS/Linux right now -> hide it
    butDriverSetup->hide();
#endif

    // init delay and other information controls
    ledNetw->Reset();
    ledOverallDelay->Reset();
    lblPingTimeValue->setText     ( "---" );
    lblOverallDelayValue->setText ( "---" );
    lblUpstreamValue->setText     ( "---" );
    edtNewClientLevel->setValidator ( new QIntValidator ( 0, 100, this ) ); // % range from 0-100


    // init slider controls ---
    // network buffer sliders
    sldNetBuf->setRange       ( MIN_NET_BUF_SIZE_NUM_BL, MAX_NET_BUF_SIZE_NUM_BL );
    sldNetBufServer->setRange ( MIN_NET_BUF_SIZE_NUM_BL, MAX_NET_BUF_SIZE_NUM_BL );
    UpdateJitterBufferFrame();

    // init combo box containing all available sound cards in the system
    cbxSoundcard->clear();
    for ( int iSndDevIdx = 0; iSndDevIdx < pClient->GetSndCrdNumDev(); iSndDevIdx++ )
    {
        cbxSoundcard->addItem ( pClient->GetSndCrdDeviceName ( iSndDevIdx ) );
    }
    cbxSoundcard->setCurrentIndex ( pClient->GetSndCrdDev() );

    // init sound card channel selection frame
    UpdateSoundChannelSelectionFrame();

    // fancy GUI design check box
    if ( pClient->GetGUIDesign() == GD_STANDARD )
    {
        chbGUIDesignFancy->setCheckState ( Qt::Unchecked );
    }
    else
    {
        chbGUIDesignFancy->setCheckState ( Qt::Checked );
    }

    // Display Channel Levels check box
    chbDisplayChannelLevels->setCheckState ( pClient->GetDisplayChannelLevels() ? Qt::Checked : Qt::Unchecked );

    // "Audio Channels" combo box
    cbxAudioChannels->clear();
    cbxAudioChannels->addItem ( tr ( "Mono" ) );               // CC_MONO
    cbxAudioChannels->addItem ( tr ( "Mono-in/Stereo-out" ) ); // CC_MONO_IN_STEREO_OUT
    cbxAudioChannels->addItem ( tr ( "Stereo" ) );             // CC_STEREO
    cbxAudioChannels->setCurrentIndex ( static_cast<int> ( pClient->GetAudioChannels() ) );

    // "Audio Quality" combo box
    cbxAudioQuality->clear();
    cbxAudioQuality->addItem ( tr ( "Low" ) );    // AQ_LOW
    cbxAudioQuality->addItem ( tr ( "Normal" ) ); // AQ_NORMAL
    cbxAudioQuality->addItem ( tr ( "High" ) );   // AQ_HIGH
    cbxAudioQuality->setCurrentIndex ( static_cast<int> ( pClient->GetAudioQuality() ) );

    // central server address type combo box
    cbxCentServAddrType->clear();
    cbxCentServAddrType->addItem ( tr ( "Manual" ) );                  // AT_MANUAL
    cbxCentServAddrType->addItem ( tr ( "Default" ) );                 // AT_DEFAULT
    cbxCentServAddrType->addItem ( tr ( "Default (North America)" ) ); // AT_NORTH_AMERICA
    cbxCentServAddrType->setCurrentIndex ( static_cast<int> ( pClient->GetCentralServerAddressType() ) );
    UpdateCentralServerDependency();

    // update new client fader level edit box
    edtNewClientLevel->setText ( QString::number ( pClient->iNewClientFaderLevel ) );

    // update enable small network buffers check box
    chbEnableOPUS64->setCheckState ( pClient->GetEnableOPUS64() ? Qt::Checked : Qt::Unchecked );

    // set text for sound card buffer delay radio buttons
    rbtBufferDelayPreferred->setText ( GenSndCrdBufferDelayString (
        FRAME_SIZE_FACTOR_PREFERRED * SYSTEM_FRAME_SIZE_SAMPLES ) );

    rbtBufferDelayDefault->setText ( GenSndCrdBufferDelayString (
        FRAME_SIZE_FACTOR_DEFAULT * SYSTEM_FRAME_SIZE_SAMPLES,
        ", " + tr ( "preferred" ) ) );

    rbtBufferDelaySafe->setText ( GenSndCrdBufferDelayString (
        FRAME_SIZE_FACTOR_SAFE * SYSTEM_FRAME_SIZE_SAMPLES ) );

    // sound card buffer delay inits
    SndCrdBufferDelayButtonGroup.addButton ( rbtBufferDelayPreferred );
    SndCrdBufferDelayButtonGroup.addButton ( rbtBufferDelayDefault );
    SndCrdBufferDelayButtonGroup.addButton ( rbtBufferDelaySafe );

    UpdateSoundCardFrame();


    // Connections -------------------------------------------------------------
    // timers
    QObject::connect ( &TimerStatus, SIGNAL ( timeout() ),
        this, SLOT ( OnTimerStatus() ) );

    // slider controls
    QObject::connect ( sldNetBuf, SIGNAL ( valueChanged ( int ) ),
        this, SLOT ( OnNetBufValueChanged ( int ) ) );

    QObject::connect ( sldNetBufServer, SIGNAL ( valueChanged ( int ) ),
        this, SLOT ( OnNetBufServerValueChanged ( int ) ) );

    // check boxes
    QObject::connect ( chbGUIDesignFancy, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnGUIDesignFancyStateChanged ( int ) ) );

    QObject::connect ( chbDisplayChannelLevels, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnDisplayChannelLevelsStateChanged ( int ) ) );

    QObject::connect ( chbAutoJitBuf, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnAutoJitBufStateChanged ( int ) ) );

    QObject::connect ( chbEnableOPUS64, SIGNAL ( stateChanged ( int ) ),
        this, SLOT ( OnEnableOPUS64StateChanged ( int ) ) );

    // line edits
    QObject::connect ( edtCentralServerAddress, SIGNAL ( editingFinished() ),
        this, SLOT ( OnCentralServerAddressEditingFinished() ) );

    QObject::connect ( edtNewClientLevel, SIGNAL ( editingFinished() ),
        this, SLOT ( OnNewClientLevelEditingFinished() ) );

    // combo boxes
    QObject::connect ( cbxSoundcard, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnSoundcardActivated ( int ) ) );

    QObject::connect ( cbxLInChan, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnLInChanActivated ( int ) ) );

    QObject::connect ( cbxRInChan, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnRInChanActivated ( int ) ) );

    QObject::connect ( cbxLOutChan, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnLOutChanActivated ( int ) ) );

    QObject::connect ( cbxROutChan, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnROutChanActivated ( int ) ) );

    QObject::connect ( cbxAudioChannels, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnAudioChannelsActivated ( int ) ) );

    QObject::connect ( cbxAudioQuality, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnAudioQualityActivated ( int ) ) );

    QObject::connect ( cbxCentServAddrType, SIGNAL ( activated ( int ) ),
        this, SLOT ( OnCentServAddrTypeActivated ( int ) ) );

    // buttons
    QObject::connect ( butDriverSetup, SIGNAL ( clicked() ),
        this, SLOT ( OnDriverSetupClicked() ) );

    // misc
    QObject::connect ( &SndCrdBufferDelayButtonGroup,
        SIGNAL ( buttonClicked ( QAbstractButton* ) ), this,
        SLOT ( OnSndCrdBufferDelayButtonGroupClicked ( QAbstractButton* ) ) );


    // Timers ------------------------------------------------------------------
    // start timer for status bar
    TimerStatus.start ( DISPLAY_UPDATE_TIME );
}

void CClientSettingsDlg::UpdateJitterBufferFrame()
{
    // update slider value and text
    const int iCurNumNetBuf = pClient->GetSockBufNumFrames();
    sldNetBuf->setValue ( iCurNumNetBuf );
    lblNetBuf->setText ( tr ( "Size: " ) + QString().setNum ( iCurNumNetBuf ) );

    const int iCurNumNetBufServer = pClient->GetServerSockBufNumFrames();
    sldNetBufServer->setValue ( iCurNumNetBufServer );
    lblNetBufServer->setText ( tr ( "Size: " ) + QString().setNum ( iCurNumNetBufServer ) );

    // if auto setting is enabled, disable slider control
    const bool bIsAutoSockBufSize = pClient->GetDoAutoSockBufSize();

    chbAutoJitBuf->setChecked        (  bIsAutoSockBufSize );
    sldNetBuf->setEnabled            ( !bIsAutoSockBufSize );
    lblNetBuf->setEnabled            ( !bIsAutoSockBufSize );
    lblNetBufLabel->setEnabled       ( !bIsAutoSockBufSize );
    sldNetBufServer->setEnabled      ( !bIsAutoSockBufSize );
    lblNetBufServer->setEnabled      ( !bIsAutoSockBufSize );
    lblNetBufServerLabel->setEnabled ( !bIsAutoSockBufSize );
}

QString CClientSettingsDlg::GenSndCrdBufferDelayString ( const int     iFrameSize,
                                                         const QString strAddText )
{
    // use two times the buffer delay for the entire delay since
    // we have input and output
    return QString().setNum ( static_cast<double> ( iFrameSize ) * 2 *
        1000 / SYSTEM_SAMPLE_RATE_HZ, 'f', 2 ) + " ms (" +
        QString().setNum ( iFrameSize ) + strAddText + ")";
}

void CClientSettingsDlg::UpdateSoundCardFrame()
{
    // get current actual buffer size value
    const int iCurActualBufSize = pClient->GetSndCrdActualMonoBlSize();

    // check which predefined size is used (it is possible that none is used)
    const bool bPreferredChecked = ( iCurActualBufSize == SYSTEM_FRAME_SIZE_SAMPLES * FRAME_SIZE_FACTOR_PREFERRED );
    const bool bDefaultChecked   = ( iCurActualBufSize == SYSTEM_FRAME_SIZE_SAMPLES * FRAME_SIZE_FACTOR_DEFAULT );
    const bool bSafeChecked      = ( iCurActualBufSize == SYSTEM_FRAME_SIZE_SAMPLES * FRAME_SIZE_FACTOR_SAFE );

    // Set radio buttons according to current value (To make it possible
    // to have all radio buttons unchecked, we have to disable the
    // exclusive check for the radio button group. We require all radio
    // buttons to be unchecked in the case when the sound card does not
    // support any of the buffer sizes and therefore all radio buttons
    // are disabeld and unchecked.).
    SndCrdBufferDelayButtonGroup.setExclusive ( false );
    rbtBufferDelayPreferred->setChecked ( bPreferredChecked );
    rbtBufferDelayDefault->setChecked   ( bDefaultChecked );
    rbtBufferDelaySafe->setChecked      ( bSafeChecked );
    SndCrdBufferDelayButtonGroup.setExclusive ( true );

    // disable radio buttons which are not supported by audio interface
    rbtBufferDelayPreferred->setEnabled ( pClient->GetFraSiFactPrefSupported() );
    rbtBufferDelayDefault->setEnabled   ( pClient->GetFraSiFactDefSupported() );
    rbtBufferDelaySafe->setEnabled      ( pClient->GetFraSiFactSafeSupported() );

    // If any of our predefined sizes is chosen, use the regular group box
    // title text. If not, show the actual buffer size. Otherwise the user
    // would not know which buffer size is actually used.
    if ( bPreferredChecked || bDefaultChecked || bSafeChecked )
    {
        // default title text
        grbSoundCrdBufDelay->setTitle ( tr ( "Buffer Delay" ) );
    }
    else
    {
        // special title text with buffer size information added
        grbSoundCrdBufDelay->setTitle ( tr ( "Buffer Delay: " ) +
            GenSndCrdBufferDelayString ( iCurActualBufSize ) );
    }
}

void CClientSettingsDlg::UpdateSoundChannelSelectionFrame()
{
#if defined ( _WIN32 ) || defined ( __APPLE__ ) || defined ( __MACOSX )
    int iSndChanIdx;

    // Definition: The channel selection frame shall only be visible,
    // if more than two input or output channels are available
    const int iNumInChannels  = pClient->GetSndCrdNumInputChannels();
    const int iNumOutChannels = pClient->GetSndCrdNumOutputChannels();

    if ( ( iNumInChannels <= 2 ) && ( iNumOutChannels <= 2 ) )
    {
        // as defined, make settings invisible
        FrameSoundcardChannelSelection->setVisible ( false );
    }
    else
    {
        // update combo boxes
        FrameSoundcardChannelSelection->setVisible ( true );

        // input
        cbxLInChan->clear();
        cbxRInChan->clear();
        for ( iSndChanIdx = 0; iSndChanIdx < pClient->GetSndCrdNumInputChannels(); iSndChanIdx++ )
        {
            cbxLInChan->addItem ( pClient->GetSndCrdInputChannelName ( iSndChanIdx ) );
            cbxRInChan->addItem ( pClient->GetSndCrdInputChannelName ( iSndChanIdx ) );
        }
        if ( pClient->GetSndCrdNumInputChannels() > 0 )
        {
            cbxLInChan->setCurrentIndex ( pClient->GetSndCrdLeftInputChannel() );
            cbxRInChan->setCurrentIndex ( pClient->GetSndCrdRightInputChannel() );
        }

        // output
        cbxLOutChan->clear();
        cbxROutChan->clear();
        for ( iSndChanIdx = 0; iSndChanIdx < pClient->GetSndCrdNumOutputChannels(); iSndChanIdx++ )
        {
            cbxLOutChan->addItem ( pClient->GetSndCrdOutputChannelName ( iSndChanIdx ) );
            cbxROutChan->addItem ( pClient->GetSndCrdOutputChannelName ( iSndChanIdx ) );
        }
        if ( pClient->GetSndCrdNumOutputChannels() > 0 )
        {
            cbxLOutChan->setCurrentIndex ( pClient->GetSndCrdLeftOutputChannel() );
            cbxROutChan->setCurrentIndex ( pClient->GetSndCrdRightOutputChannel() );
        }
    }
#else
    // for other OS, no sound card channel selection is supported
    FrameSoundcardChannelSelection->setVisible ( false );
#endif
}

void CClientSettingsDlg::UpdateCentralServerDependency()
{
    const bool bCurUseDefCentServAddr = ( pClient->GetCentralServerAddressType() != AT_MANUAL );

    // make sure the line edit does not fire signals when we update the text
    edtCentralServerAddress->blockSignals ( true );
    {
        if ( bCurUseDefCentServAddr )
        {
            // if the default central server is used, just show a text of the
            // server name
            edtCentralServerAddress->setText ( DEFAULT_SERVER_NAME );
        }
        else
        {
            // show the current user defined server address
            edtCentralServerAddress->setText (
                pClient->GetServerListCentralServerAddress() );
        }
    }
    edtCentralServerAddress->blockSignals ( false );

    // the line edit of the central server address is only enabled, if not the
    // default address is used
    edtCentralServerAddress->setEnabled ( !bCurUseDefCentServAddr );
}

void CClientSettingsDlg::OnDriverSetupClicked()
{
    pClient->OpenSndCrdDriverSetup();
}

void CClientSettingsDlg::OnNetBufValueChanged ( int value )
{
    pClient->SetSockBufNumFrames ( value, true );
    UpdateJitterBufferFrame();
}

void CClientSettingsDlg::OnNetBufServerValueChanged ( int value )
{
    pClient->SetServerSockBufNumFrames ( value );
    UpdateJitterBufferFrame();
}

void CClientSettingsDlg::OnSoundcardActivated ( int iSndDevIdx )
{
    const QString strError = pClient->SetSndCrdDev ( iSndDevIdx );

    if ( !strError.isEmpty() )
    {
        QMessageBox::critical ( this, APP_NAME,
            QString ( tr ( "The selected audio device could not be used "
            "because of the following error: " ) ) + strError +
            QString ( tr ( " The previous driver will be selected." ) ),
            tr ( "Ok" ), nullptr );

        // recover old selection
        cbxSoundcard->setCurrentIndex ( pClient->GetSndCrdDev() );
    }
    UpdateSoundChannelSelectionFrame();
    UpdateDisplay();
}

void CClientSettingsDlg::OnLInChanActivated ( int iChanIdx )
{
    pClient->SetSndCrdLeftInputChannel ( iChanIdx );
    UpdateSoundChannelSelectionFrame();
}

void CClientSettingsDlg::OnRInChanActivated ( int iChanIdx )
{
    pClient->SetSndCrdRightInputChannel ( iChanIdx );
    UpdateSoundChannelSelectionFrame();
}

void CClientSettingsDlg::OnLOutChanActivated ( int iChanIdx )
{
    pClient->SetSndCrdLeftOutputChannel ( iChanIdx );
    UpdateSoundChannelSelectionFrame();
}

void CClientSettingsDlg::OnROutChanActivated ( int iChanIdx )
{
    pClient->SetSndCrdRightOutputChannel ( iChanIdx );
    UpdateSoundChannelSelectionFrame();
}

void CClientSettingsDlg::OnAudioChannelsActivated ( int iChanIdx )
{
    pClient->SetAudioChannels ( static_cast<EAudChanConf> ( iChanIdx ) );
    emit AudioChannelsChanged();
    UpdateDisplay(); // upload rate will be changed
}

void CClientSettingsDlg::OnAudioQualityActivated ( int iQualityIdx )
{
    pClient->SetAudioQuality ( static_cast<EAudioQuality> ( iQualityIdx ) );
    UpdateDisplay(); // upload rate will be changed
}

void CClientSettingsDlg::OnCentServAddrTypeActivated ( int iTypeIdx )
{
    // apply new setting to the client
    pClient->SetCentralServerAddressType ( static_cast<ECSAddType> ( iTypeIdx ) );

    // update GUI dependencies
    UpdateCentralServerDependency();
}

void CClientSettingsDlg::OnAutoJitBufStateChanged ( int value )
{
    pClient->SetDoAutoSockBufSize ( value == Qt::Checked );
    UpdateJitterBufferFrame();
}

void CClientSettingsDlg::OnEnableOPUS64StateChanged ( int value )
{
    pClient->SetEnableOPUS64 ( value == Qt::Checked );
    UpdateDisplay();
}

void CClientSettingsDlg::OnGUIDesignFancyStateChanged ( int value )
{
    if ( value == Qt::Unchecked )
    {
        pClient->SetGUIDesign ( GD_STANDARD );
    }
    else
    {
        pClient->SetGUIDesign ( GD_ORIGINAL );
    }
    emit GUIDesignChanged();
    UpdateDisplay();
}

void CClientSettingsDlg::OnDisplayChannelLevelsStateChanged ( int value )
{
    pClient->SetDisplayChannelLevels ( value != Qt::Unchecked );
    emit DisplayChannelLevelsChanged();
}

void CClientSettingsDlg::OnCentralServerAddressEditingFinished()
{
    // store new setting in the client
    pClient->SetServerListCentralServerAddress (
        edtCentralServerAddress->text() );
}

void CClientSettingsDlg::OnNewClientLevelEditingFinished()
{
    // store new setting in the client
    pClient->iNewClientFaderLevel =
        edtNewClientLevel->text().toInt();

    // inform that the level has changed and the mixer board settings must
    // be updated
    emit NewClientLevelChanged();
}

void CClientSettingsDlg::OnSndCrdBufferDelayButtonGroupClicked ( QAbstractButton* button )
{
    if ( button == rbtBufferDelayPreferred )
    {
        pClient->SetSndCrdPrefFrameSizeFactor ( FRAME_SIZE_FACTOR_PREFERRED );
    }

    if ( button == rbtBufferDelayDefault )
    {
        pClient->SetSndCrdPrefFrameSizeFactor ( FRAME_SIZE_FACTOR_DEFAULT );
    }

    if ( button == rbtBufferDelaySafe )
    {
        pClient->SetSndCrdPrefFrameSizeFactor ( FRAME_SIZE_FACTOR_SAFE );
    }

    UpdateDisplay();
}

void CClientSettingsDlg::SetPingTimeResult ( const int                         iPingTime,
                                             const int                         iOverallDelayMs,
                                             const CMultiColorLED::ELightColor eOverallDelayLEDColor )
{
    // apply values to GUI labels, take special care if ping time exceeds
    // a certain value
    if ( iPingTime > 500 )
    {
        const QString sErrorText =
            "<font color=""red""><b>&#62;500 ms</b></font>";

        lblPingTimeValue->setText     ( sErrorText );
        lblOverallDelayValue->setText ( sErrorText );
    }
    else
    {
        lblPingTimeValue->setText ( QString().setNum ( iPingTime ) + " ms" );
        lblOverallDelayValue->setText (
            QString().setNum ( iOverallDelayMs ) + " ms" );
    }

    // set current LED status
    ledOverallDelay->SetLight ( eOverallDelayLEDColor );
}

void CClientSettingsDlg::UpdateDisplay()
{
    // update slider controls (settings might have been changed)
    UpdateJitterBufferFrame();
    UpdateSoundCardFrame();

    if ( !pClient->IsRunning() )
    {
        // clear text labels with client parameters
        lblPingTimeValue->setText     ( "---" );
        lblOverallDelayValue->setText ( "---" );
        lblUpstreamValue->setText     ( "---" );
    }
    else
    {
        // update upstream rate information label (only if client is running)
        lblUpstreamValue->setText (
            QString().setNum ( pClient->GetUploadRateKbps() ) + " kbps" );
    }
}
