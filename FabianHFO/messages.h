/** @file messages.h
 *   @brief File for documentation purpose only.
 *   @date 2016-10-20
 *
 * @mainpage fabian HFO documentation
 *
 *
 * @section ver Version history and changelog
 *
 * @subsection ver1000 Version 1.0.0.0
 * @date 2016-10-20
 * @verbatim
	- Systemlog messages:
	#HFO:0001		ERROR GetM_StatusByte
	#HFO:0002		ERROR GetM_CHECKSUM
	#HFO:0003		ERROR GetM_INSP_FLOW
	#HFO:0004		ERROR GetM_EXP_FLOW
	#HFO:0005		ERROR GetM_DEMAND_FLOW
	#HFO:0006		ERROR GetM_CAL_PRESS_SCALE
	#HFO:0007		ERROR GetM_CAL_PRESS_OFFSET
	#HFO:0008		ERROR GetM_VERSION
	#HFO:0009		ERROR - OpenCom CInterfaceSerial
	#HFO:0010		ERROR checking COM values, VentMode: %d Cmnd: %s
	#HFO:0011		ERROR 3x checking COM values, VentMode: %d Cmnd: %s, send again
	#HFO:0012		ReceiveSerialData: m_comPort.m_hCommPort == INVALID_HANDLE_VALUE
	#HFO:0013		ERROR5 SendSerialData, Msg:
	#HFO:0014		ERROR4 SendSerialData
	#HFO:0015		ERROR CDiagramm::DrawGrid GridSpacing<=0
	#HFO:0016		ERROR CParaBtn::WriteCurrentValue() IDC_BTN_PARA_ITIME RM_SET_ItoBPM %d
	#HFO:0017		ERROR CParaBtn::WriteCurrentValue()2 IDC_BTN_PARA_ITIME RM_SET_ItoBPM %d
	#HFO:0018		ERROR CParaBtn::WriteCurrentValue() IDC_BTN_PARA_ITIME RM_SET_ItoE %d
	#HFO:0019		ERROR CParaBtn::WriteCurrentValue()2 IDC_BTN_PARA_ITIME RM_SET_ItoE %d
	#HFO:0020		ERROR CParaBtn::WriteCurrentValue() IDC_BTN_PARA_ETIME %d
	#HFO:0021		ERROR CParaBtn::WriteCurrentValue()2 IDC_BTN_PARA_ETIME %d
	#HFO:0022		ERROR CParaBtn::WriteCurrentValue() IDC_BTN_PARA_BPM %d
	#HFO:0023		ERROR CParaBtn::WriteCurrentValue() IDC_BTN_PARA_BPM %d
	#HFO:0024		ERROR SetAlarmlimitDataMVmax value: %d
	#HFO:0025		ERROR SetAlimitSPO2maxIPPV value: %d
	#HFO:0026		ERROR SetAlimitPulseRatemaxIPPV value: %d
	#HFO:0027		ERROR SetAlimitETCO2maxIPPV value: %d
	#HFO:0028		ERROR SetAlarmlimitDataMVmin value: %d
	#HFO:0029		ERROR SetAlarmlimitDataSPO2min value: %d
	#HFO:0030		ERROR SetAlarmlimitDataPulseRatemin value: %d
	#HFO:0031		ERROR SetAlarmlimitDataSPO2_PImin value: %d
	#HFO:0032		ERROR SetAlarmlimitDataETCO2min value: %d
	#HFO:0033		ERROR SetAlarmlimitDataPmax value: %d
	#HFO:0034		ERROR SetAlarmlimitDataPEEPmin value: %d
	#HFO:0035		ERROR SetAlarmlimitDataBPMmax value: %d
	#HFO:0036		ERROR SetAlarmlimitDataLeakmax value: %d
	#HFO:0037		ERROR SetAlarmlimitDataApnoe value: %d
	#HFO:0038		ERROR SetAlarmlimitDataMVmaxHF value: %d MaxRange: %d MinRange %d
	#HFO:0039		ERROR SetAlimitPulseRatemaxHF value: %d
	#HFO:0040		ERROR SetAlimitSPO2maxHF value: %d
	#HFO:0041		ERROR SetAlimitETCO2maxHF value: %d
	#HFO:0042		ERROR SetAlarmlimitDataDCO2maxHF value: %d
	#HFO:0043		ERROR SetAlarmlimitDataMVminHF value: %d
	#HFO:0044		ERROR SetAlarmlimitDataSPO2_PIminHF value: %d
	#HFO:0045		ERROR SetAlarmlimitDataPulseRateminHF value: %d
	#HFO:0046		ERROR SetAlarmlimitDataSPO2minHF value: %d
	#HFO:0047		ERROR SetAlarmlimitDataETCO2minHF value: %d
	#HFO:0048		ERROR SetAlarmlimitDataDCO2minHF value: %d
	#HFO:0049		ERROR SetAlarmlimitDataPmaxHF value: %d
	#HFO:0050		ERROR SetAlarmlimitDataBPMmaxHF value: %d
	#HFO:0051		ERROR SetAlarmlimitDataLeakmaxHF value: %d
	#HFO:0052		ERROR SetAlarmlimitDataApnoeHF value: %d
	#HFO:0053		ERROR SetAlarmlimitDataPEEPminCPAP value: %d
	#HFO:0054		ERROR SetAlarmlimitDataPmaxCPAP value: %d
	#HFO:0055		ERROR no xml-language-file available
	#HFO:0056		ERROR CDataHandler::Init() TREND fail: bRes==false:
	#HFO:0057		ERROR CDataHandler Trends bDeserialize==false
	#HFO:0058		ERROR CDataHandler Trends Filenums different PINSP:%d PMEAN:%d FIO2:%d VTE:%d COMPLIANCE:%d CO2HFO2:%d MV:%d HFAMP:%d
	#HFO:0059		ERROR: CDataHandler::CheckLicensing() iLenUniqueID>16
	#HFO:0060		ERROR: CDataHandler::CheckLicensing() iLenUniqueID>16
	#HFO:0061		ERROR: CDataHandler::loadConfig() VM_DUOPAP isNebulizerOn==false
	#HFO:0062		ERROR: CDataHandler::loadConfig() VM_DUOPAP||VM_NCPAP && m_eFlowSensorState!=FLOWSENSOR_OFF
	#HFO:0063		ERROR: CDataHandler::loadConfig() VM_NCPAP isNebulizerOn==false
	#HFO:0064		ERROR: CDataHandler::loadConfig() VM_DUOPAP||VM_NCPAP && m_eFlowSensorState!=FLOWSENSOR_OFF
	#HFO:0065		ERROR: CDataHandler::loadConfig() VM_THERAPIE isNebulizerOn==false
	#HFO:0066		ERROR: CDataHandler::LoadConfig() THERAPIE && m_eFlowSensorState!=FLOWSENSOR_OFF
	#HFO:0067		ERROR: Start with Ventmode
	#HFO:0068		ERROR: CDataHandler::Start() Read_CHECKSUM==-1
	#HFO:0069		ERROR: CDataHandler::Start() Read_VERSION1==0xFF
	#HFO:0070		ERROR: CDataHandler::Start() Read_VERSION1==0
	#HFO:0071		ERROR: CDataHandler::Start() Read_VERSION1==0xFF
	#HFO:0072		ERROR: CDataHandler::Start() Read_VERSION1==0
	#HFO:0073		ERROR: CDataHandler::SetFlowSensorState() fail
	#HFO:0074		ERROR CDataHandler::CheckIErelation() RM_SET_ItoBPM iITime%d iBPM%d iETime%d 
	#HFO:0075		ERROR CDataHandler::CheckIErelation() RM_SET_ItoE iITime%d iBPM%d iETime%d
	#HFO:0076		ERROR CDataHandler::CalculateETime_ITimeChanged() %d
	#HFO:0077		ERROR CDataHandler::CalculateBPM_ITimeChanged() %d
	#HFO:0078		ERROR CDataHandler::CalculateETime_BPMChanged() %d
	#HFO:0079		ERROR CDataHandler::CalculateBPM_ETimeChanged() %d
	#HFO:0080		ERROR CalculateAutoAlarmlimitMVmax value=0
	#HFO:0081		ERROR CalculateAutoAlarmlimitPmax HFO: Pmax %d, Ppeak %d, Auto %d, RatioIE %d
	#HFO:0082		Platform doesn't support CONTRASTCOMMAND
	#HFO:0083		SerializeAllTrends CheckAkku
	#HFO:0084		SerializeAllTrends CheckAkku2
	#HFO:0085		TREND: DeleteTrendData1 Time
	#HFO:0086		TREND: DeleteTrendData2 Time
	#HFO:0087		TREND: DeleteTrendData3 Time
	#HFO:0088		TREND: DeleteTrendData4 Time
	#HFO:0089		TREND: DeleteTrendData5 Time
	#HFO:0090		TREND: DeleteTrendData6 Time
	#HFO:0091		TREND: DeleteTrendData7 Time
	#HFO:0092		TREND: DeleteTrendData8 Time
	#HFO:0093		ERROR open file SerializeTrend:
	#HFO:0094		ERROR open file DeserializeTrend:
	#HFO:0095		ERROR open file DeserializeTempTrend:
	#HFO:0096		ERROR Trend MkDir
	#HFO:0097		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_GASTEMPERATURE host: %d, slave: %d
	#HFO:0098		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_ETCO2_TIMEPERIOD host: %d, slave: %d
	#HFO:0099		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_NOBREATHDETECTED_TIMEOUT host: %d, slave: %d
	#HFO:0100		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_CURRENT_CO2UNITS host: %d, slave: %d
	#HFO:0101		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_SLEEPMODE host: %d, slave: %d
	#HFO:0102		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_ZEROGASTYPE host: %d, slave: %d
	#HFO:0103		ERROR Capnostat: ParseCO2Command CO2_CAPNOSTAT_ISB_GAS_COMPENSATIONS host: %d, slave: %d
	#HFO:0104		ERROR StopETCO2Thread WaitForSingleObject(m_pcwtETCO2SendThread->m_hThread,1000)
	#HFO:0105		ERROR StopCO2Thread WaitForSingleObject(m_pcwtETCO2InitThread->m_hThread,1000)
	#HFO:0106		ERROR StopCO2checkThread WaitForSingleObject(m_pcwtETCO2checkThread->m_hThread,1000)
	#HFO:0107		ERROR CInterfaceI2C::Init() - INVALID_HANDLE_VALUE
	#HFO:0108		ERROR CInterfaceI2C::ScanForDevice - INVALID_HANDLE_VALUE
	#HFO:0109		ERROR CInterfaceI2C::ScanForDevice Found no device
	#HFO:0110		ERROR StopI2CThread WaitForSingleObject(m_pcwtI2CThread->m_hThread,1000)
	#HFO:0111		ERROR CInterfaceCO2_MICROPOD: %s (error code %d)
	#HFO:0112		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_BTPS host: %d, slave: %d
	#HFO:0113		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_PEAKPICK host: %d, slave: %d
	#HFO:0114		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_DISTINGBREATHMSG host: %d, slave: %d
	#HFO:0115		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_CO2UNITS host: %d, slave: %d
	#HFO:0116		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_PERIODICBREATHMSG host: %d, slave: %d
	#HFO:0117		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_WAVEMEASINTERVAL host: %d, slave: %d
	#HFO:0118		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_AUTOSTANDBY host: %d, slave: %d
	#HFO:0119		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_PATIENTMODE host: %d, slave: %d
	#HFO:0120		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_CO2RANGE host: %d, slave: %d
	#HFO:0121		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_MEASUREMODE host: %d, slave: %d
	#HFO:0122		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTCO2NUM host: %d, slave: %d
	#HFO:0123		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_MODULECONFIG_PERIODICMESSAGE_CONTPERCSPONT host: %d, slave: %d
	#HFO:0124		ERROR MicroPod: ParseCO2Command CO2_MICROPOD_SLAVECMD_CUROPMODE host: %d, slave: %d
	#HFO:0125		ERROR CInterfaceSPI::Init - CreateFile(SPI1:)
	#HFO:0126		SPI: Error m_hSPIFile==INVALID_HANDLE_VALUE
	#HFO:0127		DIO: Error1 while write pin data
	#HFO:0128		DIO: Error2 while write pin data
	#HFO:0129		SPI: Error while send data IOCTL_NSPI_SEND
	#HFO:0130		SPI: Error while receive data IOCTL_NSPI_RECEIVE
	#HFO:0131		ERROR range: M_AVG_P_Peak
	#HFO:0132		ERROR range: M_AVG_P_Mean
	#HFO:0133		ERROR range: M_AVG_PEEP
	#HFO:0134		ERROR range: M_AVG_T_insp_PSV
	#HFO:0135		ERROR range: M_AVG_Dyn_Compl
	#HFO:0136		ERROR range: M_AVG_C20/C
	#HFO:0137		ERROR range: M_AVG_Resistance
	#HFO:0138		ERROR range: M_AVG_Min_Vol
	#HFO:0139		ERROR range: M_AVG_%Anteil_Resp
	#HFO:0140		ERROR range: M_AVG_TVE
	#HFO:0141		ERROR range: M_AVG_TVE_Respirator
	#HFO:0142		ERROR range: M_AVG_TVE_Patient
	#HFO:0143		ERROR range: M_AVG_Leak
	#HFO:0144		ERROR range: M_AVG_Freq
	#HFO:0145		ERROR range: M_AVG_Trig_Value
	#HFO:0146		ERROR range: M_AVG_TVI
	#HFO:0147		ERROR range: M_AVG_HFO_Amp
	#HFO:0148		ERROR range: M_AVG_TVE_HFO
	#HFO:0149		ERROR range: M_AVG_DCO2
	#HFO:0150		ERROR range: M_AVG_HFO_Freq
	#HFO:0151		ERROR range: M_AVG_VOL_CORR
	#HFO:0152		ERROR range: M_ATZ_P_Peak
	#HFO:0153		ERROR range: M_ATZ_P_Mean
	#HFO:0154		ERROR range: M_ATZ_PEEP
	#HFO:0155		ERROR range: M_ATZ_T_insp_PSV
	#HFO:0156		ERROR range: M_ATZ_Dyn_Compl
	#HFO:0157		ERROR range: M_ATZ_C20/C
	#HFO:0158		ERROR range: M_ATZ_Resistance
	#HFO:0159		ERROR range: M_ATZ_Min_Vol
	#HFO:0160		ERROR range: M_ATZ_%Anteil_Resp
	#HFO:0161		ERROR range: M_ATZ_TVE
	#HFO:0162		ERROR range: M_ATZ_TVE_Respirator
	#HFO:0163		ERROR range: M_ATZ_TVE_Patient
	#HFO:0164		ERROR range: M_ATZ_Leak
	#HFO:0165		ERROR range: M_ATZ_Freq
	#HFO:0166		ERROR range: M_ATZ_Trig_Value
	#HFO:0167		ERROR range: M_ATZ_TVI
	#HFO:0168		ERROR range: M_ATZ_HFO_Amp
	#HFO:0169		ERROR range: M_ATZ_TVE_HFO
	#HFO:0170		ERROR range: M_ATZ_DCO2
	#HFO:0171		ERROR range: M_ATZ_HFO_Freq
	#HFO:0172		SPI: Error while Read_P_PROX_ADC
	#HFO:0173		SPI: Error while Read_DRM_1_ADC
	#HFO:0174		SPI: Error while Read_DRM_2_ADC
	#HFO:0175		SPI: Error while Read_DRM_3_ADC
	#HFO:0176		SPI: Error while Read_Batt_stat	
	#HFO:0177		SPI: Error while Read_OXY1_ADC
	#HFO:0178		SPI: Error while Read_OXY2_ADC
	#HFO:0179		SPI: Error while Read_STATUS
	#HFO:0180		SPI: Error while Read_STATUS IsSPIenabled()==false
	#HFO:0181		SPI: Error while Read_STATUS INVALID_HANDLE_VALUE
	#HFO:0182		SPI: Error while Read_ATZ_P_Peak
	#HFO:0183		SPI: Error while Read_CHECKSUM
	#HFO:0184		SPI: Error while Read_VERSION1
	#HFO:0185		SPI: Error while Read_VERSION2
	#HFO:0186		SPI: Error while Read_VCC_24_BLEND_ADC
	#HFO:0187		SPI: Error while Read_VCC_5_ADC
	#HFO:0188		SPI: Error while Read_VENT_MODE
	#HFO:0189		SPI: Error while Read_CAL_PRESS_SCALE
	#HFO:0190		SPI: Error while Read_CAL_PRESS_OFFSET
	#HFO:0191		SPI: Error while Read_KOMMANDO
	#HFO:0192		SPI: Error while Read_FLOWSENS_CMND
	#HFO:0193		SPI: Error while Read_STATUS_PROXSENSOR
	#HFO:0194		SPI: Error while Read_STATUS_FLOWSENSOR
	#HFO:0195		SPI: Error while Read_DAC_FLOW_INSP
	#HFO:0196		SPI: Error while Read_DAC_FLOW_EXH
	#HFO:0197		SPI: Error while Read_FLOW_INSP_ADC
	#HFO:0198		SPI: Error while Read_FLOW_EXH_ADC
	#HFO:0199		SPI: Error while Read_SYST_OFFSET_FLOW_INSP
	#HFO:0200		SPI: Error while Read_SYST_OFFSET_FLOW_EXH
	#HFO:0201		SPI: Error while Read_SYST_SCALE_FLOW_INSP
	#HFO:0202		SPI: Error while Read_SYST_SCALE_FLOW_EXH
	#HFO:0203		SPI: Error while Read_FLOWCAL_DEV_A
	#HFO:0204		SPI: Error while Read_FLOWCAL_DEV_B
	#HFO:0205		SPI: Error while Read_OXY_ACTUAL
	#HFO:0206		SPI: Error while Read_OXY_ACTUAL
	#HFO:0207		SPI: Error while Read_OXY_DEFAULT
	#HFO:0208		SPI: Error while Read_STATUS_OXY_SENSOR
	#HFO:0209		SPI: Error while Read_FLOW_CORFACTOR
	#HFO:0210		ERROR StopSPO2Thread WaitForSingleObject(m_pcwtSPO2SendThread->m_hThread,1000)
	#HFO:0211		ERROR StopSPO2Thread WaitForSingleObject(m_pcwtSPO2InitThread->m_hThread,1000)
	#HFO:0212		ERROR StopSPO2checkThread WaitForSingleObject(m_pcwtSPO2checkThread->m_hThread,1000)
	#HFO:0213		ERROR StopTerminalThread WaitForSingleObject(m_pcwtTerminalSendThread->m_hThread,1000)
	#HFO:0214		ERROR: no UNICODE language file:
	#HFO:0215		ERROR: default no UNICODE language file:
	#HFO:0216		ERROR can not delete  %s
	#HFO:0217		ReinitSERIAL GetM_CHECKSUM success
	#HFO:0218		ReinitSERIAL GetM_CHECKSUM fail
	#HFO:0219		ReinitSERIAL fail
	#HFO:0220		I2CWatchdog ERROR: MEMORY
	#HFO:0221		I2CWatchdog ERROR: TREND
	#HFO:0222		ERROR WATCHDOGTIMER
	#HFO:0223		ERROR: There are %*ld free %sbytes of physical memory
	#HFO:0224		ERROR CDataHandler::DeleteTrendData() fail
	#HFO:0225		ERROR: UniqueID try NetworkAddress
	#HFO:0226		ERROR: UniqueID NetworkAddress failed
	#HFO:0227		SPI-ERROR ERRC_SPI_INIT
	#HFO:0228		Trigger-ERROR EV_BN_MENU_IPPV 1"
	#HFO:0229		Trigger-ERROR EV_CONTROL_FLOWSENSORSTATE VM_PSV VentModeChanged()
	#HFO:0230		Trigger-ERROR EV_CONTROL_FLOWSENSORSTATE VM_SIMVPSV VentModeChanged()
	#HFO:0231		Trigger-ERROR EV_CONTROL_FLOWSENSORSTATE VM_SIMV VentModeChanged()
	#HFO:0232		Trigger-ERROR EV_CONTROL_FLOWSENSORSTATE VM_SIPPV VentModeChanged()
	#HFO:0233		Trigger-ERROR EV_CONTROL_SETLASTVENTMODE
	#HFO:0234		ERROR EV_PARABN_VGARANT_AUTO_OFF
	#HFO:0235		Trigger-ERROR VentModeChanged(VM_SERVICE)
	#HFO:0236		EV_BN_MATRIX_STARTSTOP_DOWN GetViewState()==VS_STARTUP || GetViewState()==VS_PATDATA
	#HFO:0237		EV_BN_MATRIX_STARTSTOP_UP GetViewState()==VS_STARTUP || GetViewState()==VS_PATDATA
	#HFO:0238		Trigger-ERROR EV_BN_MENU_IPPV 2
	#HFO:0239		Trigger-ERROR EV_BN_MENU_PSV 2
	#HFO:0240		Trigger-ERROR EV_BN_MENU_SIMV 2
	#HFO:0241		Trigger-ERROR EV_BN_MENU_SIMVPSV 2
	#HFO:0242		Trigger-ERROR EV_BN_MENU_SIPPV 2
	#HFO:0243		Trigger-ERROR EV_BN_MENU_HFO 2
	#HFO:0244		Trigger-ERROR EV_BN_MENU_CPAP 2
	#HFO:0245		Trigger-ERROR EV_BN_MENU_NCPAP 2
	#HFO:0246		Trigger-ERROR EV_BN_MENU_NCPAP 2
	#HFO:0247		Trigger-ERROR EV_BN_MENU_NCPAP 2
	#HFO:0248		Trigger-ERROR EV_BN_MENU_NCPAP 2
	#HFO:0249		Trigger-ERROR EV_BN_MENU_DUOPAP 2
	#HFO:0250		Trigger-ERROR EV_BN_MENU_NCPAP 2
	#HFO:0251		ERROR CMVModel::ChangeToPrevMode()
	#HFO:0252		CDataHandler::ParseFlowsensorAlarmStateBytes() BIT0==1 && FLOWSENSOR_ON
	#HFO:0253		CDataHandler::ParseFlowsensorAlarmStateBytes() BIT0==0 && FLOWSENSOR_MANOFF
	#HFO:0254		CDataHandler::ParseFlowsensorAlarmStateBytes() BIT0==0 && FLOWSENSOR_OFF
	#HFO:0255		ERROR: SPI SetSPIavailability false
	#HFO:0256		CMVModel::SetI2Cerror adresse %d
	#HFO:0257		ERROR CParaDataPSV::Init1() RM_SET_ItoBPM m_iITime%d m_iBPM%d m_iETime%d
	#HFO:0258		ERROR CParaDataPSV::Init3() RM_SET_ItoBPM m_iITime%d m_iBPM%d m_iETime%d
	#HFO:0259		ERROR CParaDataPSV::Init4() RM_SET_ItoE m_iITime%d m_iBPM%d m_iETime%d
	#HFO:0260		ERROR CParaDataPSV::Init()6 RM_SET_ItoE m_iITime%d m_iBPM%d m_iETime%d
	#HFO:0261		ERROR PIF -> SendData
	#HFO:0262		ERROR PIF -> SendData SHUTDOWN
	#HFO:0263		ERROR: could not copy file
	#HFO:0264		ERROR EV_PINSP_DECREASE
	#HFO:0265		ERROR EV_VGARANT_OFF
	#HFO:0266		ERROR EV_VGARANT_PRESET_OFF
	#HFO:0267		ERROR EV_PARABN_VGARANT_PRESET_SELECTED_OFF
	#HFO:0268		ERROR COM -> Startup: GetM_StatusByte
	#HFO:0269		ERROR PIF -> Startup: SendData
	#HFO:0270		ERROR DIO -> Startup: ClearPin
	#HFO:0271		ERROR SPI -> Startup: Read_STATUS1
	#HFO:0272		ERROR SPI -> Startup: Read_STATUS2
	#HFO:0273		ERROR StopLoadThread WaitForSingleObject(m_pcwtLoadThread->m_hThread,3000)
	#HFO:0274		ERROR StopCO2DataThread WaitForSingleObject(m_pcwtCO2DataThread->m_hThread,1000)
	#HFO:0275		ERROR StopSPO2DataThread WaitForSingleObject(m_pcwtSPO2DataThread->m_hThread,1000)
	#HFO:0276		ERROR CWndMenuVentState::ShowMenuBtn() VM_HFO -> VM_IPPV
	#HFO:0277		ERROR CWndMenuVentState::ShowMenuBtn() VM_NCPAP -> VM_CPAP
	#HFO:0278		ERROR CWndMenuVentState::ShowMenuBtn() VM_DUOPAP -> VM_CPAP
	#HFO:0279		ERROR CWndMenuVentState::ShowMenuBtn() VM_THERAPIE -> VM_CPAP
	#HFO:0280		ERROR: could not copy file
	#HFO:0281		CWndServiceInterfaces::OnBnClickedExitApp() Service mode ExitApp()
	#HFO:0282		ERROR: CWndServiceLicensing::Init() iLenUniqueID>16
	#HFO:0283		Service mode CWndServiceSettings::OnBnClickedEraseFRAM()
	#HFO:0284		Service mode CWndServiceSystem::OnBnClickedInstaller()
	#HFO:0285		Service mode OnBnClickedAlarm() set off
	#HFO:0286		Service mode OnBnClickedAlarm() set on
	#HFO:0287		ERROR: INT CDataHandler::getBlockMessureDataAVG(eNumericType type)
	#HFO:0288		ERROR: INT CDataHandler::getBlockMessureDataBTB(eNumericType type)
	#HFO:0289		different version of Hospital settings Configuration: old version / new version
	#HFO:0290		Invalid timesetting	
	#HFO:0291		ERROR CMVModel::SetSERIALavailability false
	#HFO:0292		ERROR SetAlimitFICO2maxHF value: %d
	#HFO:0293		ERROR SetAlimitFICO2minHF value: %d
	#HFO:0294		ERROR CParaDataPSV::Init1() IPPV RM_SET_ItoBPM m_iITime%d m_iBPM%d m_iETime%d
	#HFO:0295		ERROR CMVModel::SetSERIALavailability false
	#HFO:0296		ERROR CParaDataPSV::Init4() TRIGGER RM_SET_ItoBPM m_iITime%d m_iBPM%d m_iETime%d
	#HFO:0297		Overruns TickCount
	#HFO:0298		ERROR COM: int iERRORcode
	#HFO:0299		ERROR switch to PEDIATRIC: could not chanage to IPPV
	#HFO:0300		ERROR FOT setParaFOTCONV (iSteps-1)/2>0
	#HFO:0301		ERROR FOT setParaFOTHFO (iSteps-1)/2>0
	#HFO:0302		ERROR Thread Check Routine [#thread]
	#HFO:0303		ERROR getTickCount64
	
	#HFO:T400		TERMINAL - SET IPPV
	#HFO:T401		TERMINAL - SET SIPPV
	#HFO:T402		TERMINAL - SET SIMV
	#HFO:T403		TERMINAL - SET SIMVPSV
	#HFO:T404		TERMINAL - SET PSV
	#HFO:T405		TERMINAL - SET CPAP
	#HFO:T406		TERMINAL - SET NCPAP
	#HFO:T407		TERMINAL - SET DUOPAP
	#HFO:T408		TERMINAL - SET HFO
	#HFO:T409		TERMINAL - SET THERAPY

	- Systemlog threading:
	#THR:001	CAlarmHandler::DoAlarmTimerFunctions
	#THR:002	CInterfaceCOMInbiolab::ReceiveSerialData	
	#THR:003	CInterfaceCOMport::SendCOMData
	#THR:004	CInterfaceCOMport::COMInit
	#THR:005	CInterfaceCOMport::COMcheck
	#THR:006	CInterfaceCO2::SendETCO2Data
	#THR:007	CInterfaceCO2::ETCO2Init
	#THR:008	CInterfaceCO2::ETCO2check
	#THR:009	CInterfaceI2C::I2CData
	#THR:010	CInterfaceSerial::ReceiveSerialData
	#THR:011	CInterfaceSerial::SendSerialData
	#THR:012	CInterfaceSerial::CheckSerialData
	#THR:013	CInterfaceSPI::SPIMonitorData
	#THR:014	CInterfaceSPI::SPICommunicationData
	#THR:015	CInterfaceSPO2::SendSPO2Data
	#THR:016	CInterfaceSPO2::SPO2Init
	#THR:017	CInterfaceSPO2::SPO2check
	#THR:018	CMainFrame::SetI2CWatchdog
	#THR:019	CMainFrame::CheckOxyCal
	#THR:020	CMainFrame::DoTimerFunctions
	#THR:021	CMainFrame::SaveTrendUSB
	#THR:022	CMainFrame::DelTrendData
	#THR:023	CMVModel::MonitorData
	#THR:024	CMVModel::AlarmData
	#THR:025	CMVModel::FlowsensorData
	#THR:026	CMVModel::Exspirationstart
	#THR:027	CSubViewCalFlowSensor::CheckFlowSensor
	#THR:028	CSubViewCalOxySensor::CheckOxySensor
	#THR:029	CSubViewLanguage::LoadLanguage
	#THR:030	CSubViewTools::WaitTrendUSB
	#THR:031	CViewDiagramm::DrawDiagramm
	#THR:032	CWndDataCLFIO2::CLFIO2Data
	#THR:033	CWndDataCO2::CO2Data
	#THR:034	CWndDataEMG::EMGData
	#THR:035	CWndDataSPO2::SPO2Data
	#THR:036	CWndNumAvDUOPAP::DoFlow
	#THR:037	CWndNumAvNCPAP::DoFlow
	#THR:038	CWndNumAvTHERAPIE::DoFlow
	- Systemlog button-clicks:
	"$M1$": MATRIX_ALARMSILENT
	"$M2$": MATRIX_GRAPH
	"$M3$": MATRIX_ONOFF
	"$M4$": MATRIX_STARTSTOP
	"$M5$": MATRIX_MANBREATH
	"$M6$": MATRIX_ALIMITS
	"$M7$": MATRIX_O2FLUSH
	"$M8$": MATRIX_HOME/MATRIX_MODE
	"$M9$": MATRIX_MENU
	"$M10$": MATRIX_NEBULISER
	"$M20$": BN_IPPV
	"$M21$": BN_SIPPV
	"$M22$": BN_SIMV
	"$M23$": BN_SIMVPSV
	"$M24$": BN_PSV
	"$M25$": BN_HFO
	"$M26$": BN_CPAP
	"$M27$": BN_NCPAP
	"$M28$": BN_DUOPAP
	"$M29$": BN_THERAPIE
	"$M30$": RETURN VS_SYSFAIL
	"$M31$": RETURN VS_BATTERY
	"$M32$": RETURN VS_PATDATA
	"$M33$": RETURN VS_TREND
	"$M34$": RETURN VS_ALARM_SYS
	"$M35$": RETURN VS_GRAPH
	"$M36$": RETURN VS_PARA
	"$M37$": RETURN VS_SETUP
	"$M38$": RETURN VS_ALARM_LIMIT
	"$M39$": RETURN VS_SERVICE
	"$M50$": MATRIX_SOFTKEY1
	"$M51$": MATRIX_SOFTKEY2
	"$M52$": MATRIX_SOFTKEY3
	"$M53$": MATRIX_SOFTKEY4
 @endverbatim
 *
 **/