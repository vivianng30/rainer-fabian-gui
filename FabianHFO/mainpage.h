/** @file mainpage.h
 *   @brief File for documentation purpose only.
 *   @date 2013-22-01
 *
 * @mainpage fabian HFO documentation
 *
 *
 * @section ver Version history and changelog
 *
 * @subsection ver501476 Version 5.0.1.47
 * @date 2017-06-21
 * @verbatim
	- NIV trigger can be disabled/enabled through registry key: disableNIVtrigger.reg/enableNIVtrigger.reg
 @endverbatim
 *
 * @subsection ver50146 Version 5.0.1.46
 * @date 2017-06-20
 * @verbatim
	- conformation/textmessage in service password screen included 
 @endverbatim
 *
 * @subsection ver50145 Version 5.0.1.45
 * @date 2017-06-20
 * @verbatim
	- HTTPD and TELNET service are disabled at startup
	- REGISTRY folder included
 @endverbatim
 *
 * @subsection ver50144 Version 5.0.1.44
 * @date 2017-06-19
 * @verbatim
	- FOT: correction factors for the magnitude and the phase of the impedance Zrs are read from ampCorFactor.ini file and used for calculation of RESISTANCE and REACTANCE
 @endverbatim
 *
 * @subsection ver50143 Version 5.0.1.43
 * @date 2017-06-19
 * @verbatim
	- NULLPOINTER in case of HFO and no FOT license fixed
 @endverbatim
 *
 * @subsection ver50142 Version 5.0.1.42
 * @date 2017-06-01
 * @verbatim
	- AmplitudeCorrectionFactor included - only if FOT running
 @endverbatim
 *
 * @subsection ver50141 Version 5.0.1.41
 * @date 2017-05-31
 * @verbatim
	- MAINBOARD version 3.2 included
 @endverbatim
 *
 * @subsection ver50140 Version 5.0.1.40
 * @date 2017-05-30
 * @verbatim
	- IPPV: flow sensor == off -> alarm limit freq hidden
	- DUOPAP/NCPAP: loops general inactive
	- alarm limit check changed regarding flow sensor state
 @endverbatim
 *
 * @subsection ver50139 Version 5.0.1.39
 * @date 2017-05-26
 * @verbatim
	- startup with DUOPAP and MEDIJET fixed: flowsensor should be off
 @endverbatim
 *
 * @subsection ver50138 Version 5.0.1.38
 * @date 2017-05-09
 * @verbatim
	- ACULINK: alarm limits state off fixed
	- ACULINK: ALINK_SETT_O2COMPENSATION_CO2 only if module == CAPNOSTAT
	- ACULINK: apnea time in NCPAP and DUOPAP valid
	- ACULINK: PIPmin not valid in: CPAP, NCPAP, DUOPAP, THERAPY, HFO
	- CVentModeHandler::changeVentMode: order changed - 1.getModel()->getALARMHANDLER()->ventModeChanged() - 2.getModel()->Send_VENT_MODE(state);
	- ACULINK: THERAPY: IFLOW==NOTVALID
	- ACULINK: HFO: RESISTANCE; COMPLIANCE AND C20C set to NOTVALID
	- ACULINK: HFO: TAU==NOTVALID
	- ACULINK: HFO: FREQUENCY==VALID
 @endverbatim
 *
 * @subsection ver50137 Version 5.0.1.37
 * @date 2017-05-08
 * @verbatim
	- ACULINK: TAU value rounded correct
	- ACULINK: FiCO2 as average data
	- sequence of startup changed
 @endverbatim
 *
 * @subsection ver50136 Version 5.0.1.36
 * @date 2017-05-08
 * @verbatim
	- initialization of FOT fixed (ITEM-77)
	- ACULINK: SIQ of SpO2 will be send correct
 @endverbatim
 *
 * @subsection ver50135 Version 5.0.1.35
 * @date 2017-05-05
 * @verbatim
	- text changed of alarm "High PIP" in case of CPAP/NCPAP (ITEM-76)
 @endverbatim
 *
 * @subsection ver50134 Version 5.0.1.34
 * @date 2017-05-03
 * @verbatim
	- in DUOPAP when trigger is active, S-DUOPAP is displayed in infoline (ITEM-75)
	- EXCEPTION handling improved
 @endverbatim
 *
 * @subsection ver50133 Version 5.0.1.33
 * @date 2017-05-03
 * @verbatim
	- E-Flow setting view changed (ITEM-74)
	- E-Flow value in ventilation menu will be send directly when value is changed and confirmed
 @endverbatim
 *
 * @subsection ver50132 Version 5.0.1.32
 * @date 2017-05-02
 * @verbatim
	- alarm handling changed (ITEM-72)
 @endverbatim
 *
 * @subsection ver50131 Version 5.0.1.31
 * @date 2017-04-28
 * @verbatim
 - complete factory reset will save the demo license timestamp (ITEM-64)
 - info line will be refreshed when FOT is started (ITEM-70)
 - Message box for manual turn off FOT (ITEM-69)
 - new view for E-Flow menu (ITEM-68)
 - FOT values will be drawn correct regarding VG state (ITEM-71)
 @endverbatim
 *
 * @subsection ver50130 Version 5.0.1.30
 * @date 2017-04-27
 * @verbatim
	- EFlowMaxKey / IFlowMaxKey declared as WORD now
	- IFlow == EFlow function: EFlow will be send only if function will be enabled
 @endverbatim
 *
 * @subsection ver50129 Version 5.0.1.29
 * @date 2017-04-26
 * @verbatim
	- parameter Risetime will be send also to Monitor PIC now
	- Backup active message at CPAP is also signaled when FS off.
 @endverbatim
 *
 * @subsection ver50128 Version 5.0.1.28
 * @date 2017-04-21
 * @verbatim
	- new acuLink protocol implemented -> acuLink version 4.x !
	- AmplitudeCorrectionFactor excluded
	- Version AcuInstall.ini increased (ITEM-57)
	- text E-Flow = I-Flow changed (ITEM-58)
	- enabling FOT and PRICO license works now (ITEM-59)
	- PIPmax in FOT displayed correct (ITEM-60)
	- stabilisation period starts always when return is initiated (ITEM-61)
	- FOT text chganged (ITEM-63)
	- keylock FOT Peep parameters changed (ITEM-67)
	- E-Flow=I-Flow text and slider hidden when RISETIME is selected (ITEM-66)
	- changing I-Flow / Risetime updates the E-Flow parameter button in quick setrtings (ITEM-65)
 @endverbatim
 *
 * @subsection ver50127 Version 5.0.1.27
 * @date 2017-04-13
 * @verbatim
	- try/catch blocks replaced
	- NCPAP mode when the sensitivity of Apnea detection is OFF we show "--" at the Frequency numeric parameter (ITEM-III)
 @endverbatim
 *
 * @subsection ver50126 Version 5.0.1.26
 * @date 2017-04-10
 * @verbatim
	- //rku check1
	- CRITICAL_SECTION csLPFS_FUNCTIONSTRUCT implemented
	- CRITICAL_SECTION csMenuGraphs implemented
 @endverbatim
 *
 * @subsection ver50125 Version 5.0.1.25
 * @date 2017-04-07
 * @verbatim
	- CPAP and SIMV fixed regarding FS->off (ITEM-55)
	- possibility E-Flow=IFlow implemented
 @endverbatim
 *
 * @subsection ver50124 Version 5.0.1.24
 * @date 2017-04-03
 * @verbatim
	- test version Basel: MAXRANGE_PEDIATRIC_NMODEMAX and MAXRANGE_NEONATAL_NMODEMAX changed to 30 mbar
	- PSV criteria changed to 1 ... 85 %
 @endverbatim
 *
 * @subsection ver50123 Version 5.0.1.23
 * @date 2017-03-31
 * @verbatim
	- numeric for CPAP / flows sensor off implemented
	- FOT: cancel does not send values to PICs
	- HFO light: if no BisasFlow and no HFIlicense -> HFO button not displayed
	- hospital settings work correct (ITEM-45)
	- "delete" buttons of "volume limit" and "lung recruitment" licenses are hidden after deleting these licenses.
	- autoscale in O2-Therapy works now (ITEM-47)
	- measurements acuLink checked regarding valid values
 @endverbatim
 *
 * @subsection ver50122 Version 5.0.1.22
 * @date 2017-03-31
 * @verbatim
	- Bodyweight: only written to log if changed (ITEM-37)
	- DUOPAP: when I-time is less than or equal with 0.6 s the Freq button is renamed to "BackupRate" (ITEM-38)
	- autoscale fixed (Flow sensor OFF, so we have only PRESSURE graph, changing between O2 Therapy and conventional modes (IPPV, SIPPV, SIMV), (ITEM-40)
	- CPAP: flow sensor off and Pressure trigger -> apnea drawn correct now (ITEM-41)
	- text VTe (HFO)BW changed
	- HFO mode, when VG is displayed but not active, changing Amp_max value will not5 be send to PIC (ITEM-42)
 @endverbatim
 *
 * @subsection ver50121 Version 5.0.1.21
 * @date 2017-03-22
 * @verbatim
	- NEO+PED: old text deleted (ITEM-28)
	- HFO Pmean-rec warning triangle at MaxKey drawn correct (ITEM-30)
	- range of PIPmin checked at startup to PEEP and PIPmax
	- Leak comp setting off with bigger font (ITEM-31)
	- "EFLOW" numeric in O2 therapy without "CPAP" text (ITEM-32)
	- "EFLOW" numeric in O2 therapy resolution of digits changed text (ITEM-33)
	- "Freq etCO2" numeric displays "--" if Freq etCO2 has no valid value (ITEM-34)
	- bodyweight without buttons
	- bodyweight step size between 2000 and 10000g is 100 g now
	- scaling of loops fixed
 @endverbatim
 *
 * @subsection ver50120 Version 5.0.1.20
 * @date 2017-03-20
 * @verbatim
	- todo acuLink: wave SpO2 !!!!!
	- new acuLink parameters/alarms implemented //NEWACULINK
		ALINK_SETT_ALIMIT_PRICO_SPO2LOW		
		ALINK_SETT_ALIMIT_PRICO_SPO2HIGH
		ALINK_SETT_ALIMIT_PRICO_FIO2LOW
		ALINK_SETT_ALIMIT_PRICO_FIO2HIGH
		ALINK_SETT_ALIMIT_SPO2SIQMIN
		ALINK_SETT_ALIMIT_SPO2MAX
		ALINK_SETT_ALIMIT_SPO2MIN
		ALINK_SETT_ALIMIT_SPO2PIMIN
		ALINK_SETT_ALIMIT_PULSERATEMAX
		ALINK_SETT_ALIMIT_PULSERATEMIN
		ALINK_ALARM_PRICO_FiO2max
		ALINK_ALARM_PRICO_FiO2min
		ALINK_ALARM_Sens_PRICO_FiO2outOfRange
		ALINK_ALARM_Sens_SPO2_MODULE_NOTCONNECTED
		ALINK_ALARM_Sens_SPO2_SENSORFAULTY
		ALINK_ALARM_Sens_SPO2_CHECKSENSOR
		ALINK_ALARM_PatAl_SPO2_SIQmin
		ALINK_ALARM_PatAl_SPO2max
		ALINK_ALARM_PatAl_SPO2min
		ALINK_ALARM_PatAl_SPO2_PImin
		ALINK_ALARM_PatAl_PIPmin
		ALINK_ALARM_PatAl_FICO2max
		ALINK_ALARM_PatAl_FICO2min
		ALINK_ALARM_PatAl_PulseRatemax
		ALINK_ALARM_PatAl_PulseRatemin
	- return button FOT changed (ITEM-18)
	- barometric pressure in mmHg (ITEM-19)
	- FOT acceptance criteria changed (ITEM-20)
	- text "leak compensation" and "Tube set changed", NIV added (ITEM-24,25)
	- Patient range selector implemented (ITEM-26)
	- "UseBTB for Vt" switched to menu ((ITEM-27) 
	- new alarm texts (ITEM-21)
	- new alarm priorities (ITEM-22)
 @endverbatim
 *
 * @subsection ver50119 Version 5.0.1.19
 * @date 2017-03-10
 * @verbatim
	//todo FOTLANGUAGE
	- HFO: warning drawn if Pmanual is above 25mbar and increased by Pmean (ITEM-1)
	- etCO2 barometric pressure for Capnostat is calculated (ITEM-13)
 @endverbatim
 *
 * @subsection ver50118 Version 5.0.1.18
 * @date 2017-03-09
 * @verbatim
	//todo FOTLANGUAGE
	- new acuLink parameters implemented //NEWACULINK
		ALINK_MSMNT_TAU
		ALINK_MSMNT_FREQETCO2
		ALINK_MSMNT_FICO2
		ALINK_MSMNT_SPO2
		ALINK_MSMNT_SPO2_PI
		ALINK_MSMNT_SPO2_PR
		ALINK_MSMNT_SPO2_SIQ
		ALINK_MSMNT_RSBI
		ALINK_MSMNT_VTEKG
	- Dynamic Compliance, Resolution for acuLink: 0.1 (0.0 – 500.0 ml/mbar), Resolution from PIC: 0.01 !!!
	- AmplitudeCorrectionFactor implemented
	- Altitude written to log file
	- focus after FOT sequence fixed
	- FOT licensing working correct now
	- trend time update fixed (ITEM-2)
	- altitude setting default changed to 300 (ITEM-3)
	- O2field fixed (ITEM-5)
	- bodyweight logge to file (ITEM-6)
	- bodyweight setting deleted if range is changed(ITEM-7)
	- FOT return button mirrored (ITEM-8)
	- FS buttons displayed and greyed (ITEM-10)
	- FS calibration dependent text not displayed in Therapy mode (ITEM-11)
	- FS cal required suppressed when FS is off (ITEM-12)
	- no trigger line in waves if pressure trigger is selected (ITEM-14)
	- O2 flush off (ITEM-15)
	- last conv trigger will be set after FS is enabled again (ITEM-17)
	- amplitude correction factor implemented
	- frequency trend included (ITEM-16)
 @endverbatim
 *
 * @subsection ver50117 Version 5.0.1.17
 * @date 2017-02-24
 * @verbatim
	//todo FOTLANGUAGE
	- FOT values written to log 
	- FOT: VG will be used for PIPmax if VG==on
	- bodyweight included
 @endverbatim
 *
 * @subsection ver50116 Version 5.0.1.16
 * @date 2017-02-20
 * @verbatim
	- FOT improvements: 
		PIPmax always visible
		after middle step return button disabled
		calculation return button fixed
		repeat button during stabilization period disabled
		return button during stabilization period active
 @endverbatim
 *
 * @subsection ver50115 Version 5.0.1.15
 * @date 2017-02-16
 * @verbatim
	- only oner loop will be drawn
 @endverbatim
 *
 * @subsection ver50114 Version 5.0.1.14
 * @date 2017-02-09
 * @verbatim
	- new numerics implemented, default numerics deleted
	- licenses can not be deleted if module is active
	- demo licenses 3 months implemented
 @endverbatim
 *
 * @subsection ver50113 Version 5.0.1.13
 * @date 2017-02-03
 * @verbatim
	- new ConfigVersion=3005
	- new leak compensation settings: disabled, low, middle, high
	- leak compensation written to log file
	- O2 flush resets the PRICO auto on flag
	- PRICO "DisableAlarms" deletes the auto on flag
 @endverbatim
 *
 * @subsection ver50112 Version 5.0.1.12
 * @date 2017-02-03
 * @verbatim
	- new language menu icon
 @endverbatim
 *
 * @subsection ver50111 Version 5.0.1.11
 * @date 2017-02-02
 * @verbatim
	- MAINBOARD 2.2 and 3.3 included in HWconfig.ini
	- Frequency alarm setting always available  in SIPPV, SIMV and CPAP modes
	- Frequency alarm setting available in DUOPAP and NCPAP modes when the Trigger/sensitivity is turned on
 @endverbatim
 *
 * @subsection ver50110 Version 5.0.1.10
 * @date 2017-02-01
 * @verbatim
	- quick settings: buttons depends on vent mode
 @endverbatim
 *
 * @subsection ver5019 Version 5.0.1.9
 * @date 2017-02-01
 * @verbatim
	- FOT running fixed, restart possible
 @endverbatim
 *
 * @subsection ver5018 Version 5.0.1.8
 * @date 2017-02-01
 * @verbatim
	- apnea alarm limit displayed if pressure trigger and SIMV and flow sensor == off
 @endverbatim
 *
 * @subsection ver5017 Version 5.0.1.7
 * @date 2017-02-01
 * @verbatim
	- FOT licensing implemented
 @endverbatim
 *
 * @subsection ver5016 Version 5.0.1.6
 * @date 2017-01-31
 * @verbatim
	- auto turn on of PRICO implemented, version2//rku AUTOPRICO
 @endverbatim
 *
 * @subsection ver5015 Version 5.0.1.5
 * @date 2017-01-27
 * @verbatim
	- auto turn on of PRICO implemented//rku AUTOPRICO
	- date change will not affect 0mbar cal and 60mbar cal
	- PRICO will be only reactivated if no other PRICO dependent alarm is active
 @endverbatim
 *
 * @subsection ver5014 Version 5.0.1.4
 * @date 2017-01-25
 * @verbatim
	- AUTOIFLOW disabled //TODO_AUTOIFLOW
	- Trigger NMode new setting: 1-10, then off
	- graph handling changed:
		DUOPAP: only pressure graph available
		NCPAP: with MEDIJET only pressure graph available, else all graphs
	- CPAP pressure trigger included
	- range change: also in NIV mode "Flow Sensor Calibration Required"
 @endverbatim
 *
 * @subsection ver5013 Version 5.0.1.3
 * @date 2017-01-23
 * @verbatim
	- AUTOIFLOW enabled //TODO_AUTOIFLOW
 @endverbatim
 *
 * @subsection ver5012 Version 5.0.1.2
 * @date 2017-01-13
 * @verbatim
	- test NMODE trigger
	- new ENTREK simulation implemented
	- //TESTtrigger only NEONATAL available, no FOT available
 @endverbatim
 *
 * @subsection ver5011 Version 5.0.1.1
 * @date 2016-12-21
 * @verbatim
	- //rkuNEWTREND1
	- Trends: if last written trend older than 5 days, all trends will be deleted
	- HFO diagramm correct displayed at long time axis
	- version controller PIC correct displayed
	- new FOT layout
 @endverbatim
 *
 * @subsection ver50099 Version 5.0.0.99
 * @date 2016-12-14
 * @verbatim
	- AUTOIFLOW test //TODO_AUTOIFLOW
 @endverbatim
 *
 * @subsection ver5010 Version 5.0.1.0
 * @date 2016-12-01
 * @verbatim
	- CViewDiagramm::refreshLastBreath... -> return with critical section fixed
	- //rkuNEWFIX
	- //rkuINITIF
	- Pmax and AMPmax for VG different buttons
	- etco2 subview -> Respironics: zero calibration with encoder fixed
	- new trend drawing//rkuTRENDIMPR 
	- trends calculated as average values to draw
	- GetTickCount as static DWORD 32BIT and check for overrun//rkuTICKCOUNT
	- new CInterfaceSPI::SPIMonitorData(void)
 @endverbatim
 *
 * @subsection ver5005 Version 5.0.0.5
 * @date 2016-11-08
 * @verbatim
	- I2C: startup error setting
 @endverbatim
 *
 * @subsection ver5004 Version 5.0.0.4
 * @date 2016-10-27
 * @verbatim
	- FOT: new display of settings
 @endverbatim
 *
 * @subsection ver5003 Version 5.0.0.3
 * @date 2016-10-27
 * @verbatim
 ------------------ todo CPen i.e. as member!!!!!!!!!!!!!!!!!!!!!!!!!!
	- quick settings: buttons enabled/disabled, allways visible
	- NCPAP/DUOPAP and Medijet: no flow and volume graph
	- FOT requirements 
	- vent menu buttons are greyed instead of hidden
	- //rku,PRETRIGGER
	- //todoFOT implement to excel
	- //todo delete //check test
	- //todo //rku PMEANLIMITS in HFO, todo
	- //todo FOTLANGUAGE
	- //todo NEWLANGUAGE
 @endverbatim
 *
 * @subsection ver5002 Version 5.0.0.2
 * @date 2016-10-17
 * @verbatim
	 - new file messages.h implemented
	 - NCPAP/DUOPAP only in NEONATAL available
	 - DUOPAP/NCPAP: apnea alarm available
	 - new registry settings for TRIGGER NMODE (DUOPAP/NCPAP) and CONVENTIONAL
	 - new text ventilation menu: Trigger (conventional)
	 - TRIGGER: PRESSURE trigger available
	 - set FlowSensor manual off: SIMV + SIPPV available with PRESSURE TRIGGER, PSV+SIMV -> SIMV + PRESSURE TRIGGER, PSV -> SIPPV + PRESSURE TRIGGER
	 - DUOPAP/NCPAP: dependent on tube set trigger will be set automatic: Medijet -> PRESSURE TRIGGER, others -> FLOW TRIGGER
	 - DUOPAP/NCPAP: TRIGGER can be switched off in sensitivity parameter
	 - DUOPAP/NCPAP: FLow + Volume wave available (no loops!)
	 - //rku,PRETRIGGER
	 - //todoFOT implement to excel
	 - //todo delete //check test
	 - //todo //rku PMEANLIMITS in HFO, todo
	 - //todo FOTLANGUAGE
	 - //todo NEWLANGUAGE
	 - numerics with critical section
	 - language: danish included
 @endverbatim
 *
 * @subsection ver5001 Version 5.0.0.1
 * @date 2016-10-17
 * @verbatim
	- new file messages.h implemented
	- NCPAP/DUOPAP only in NEONATAL available
	- DUOPAP/NCPAP: apnea alarm available
	- new registry settings for TRIGGER DUOPAP and CONVENTIONAL
	- new text ventilation menu: Trigger (conventional)
	- TRIGGER: PRESSURE trigger available
	- set FlowSensor manual off: SIMV + SIPPV available with PRESSURE TRIGGER, PSV+SIMV -> SIMV + PRESSURE TRIGGER, PSV -> SIPPV + PRESSURE TRIGGER
	- DUOPAP: dependent on tube set trigger will be set automatic: Medijet -> PRESSURE TRIGGER, others -> FLOW TRIGGER
	- DUOPAP: TRIGGER can be switched off in sensitivity parameter
	- DUOPAP: FLow + Volume wave available (no loops!)
	- //rku,PRETRIGGER
	- FOT autoscale implemented
	- //todoFOT implement to excel
	- //todo delete //check test
	- //todo //rku PMEANLIMITS in HFO, todo
	- //todo FOTLANGUAGE
	- //todo NEWLANGUAGE
	- numerics with critical section
 @endverbatim
 *
 * @subsection ver50000 Version 5.0.0.0
 * @date 2016-09-05
 * @verbatim
	- in case of NMODE or flow sensor off -> FOT is not enabled
	- Pmean numeric displayed as x.x mbar
	- FABI-3952 - Pmanual/Pinsp - Equality
	- FABI-3936 - CPAP/PEEP - Equality
	- SPI: member m_bMonitorData, m_bNewATZ deleted, no more used in MonitorThread
	- PIPlow alarm implemented
	- PRICO will be turned off:
		AlarmType: AT_SYSFAIL
		AlarmType: AT_SYSALARM
		AlarmType: AT_DISCON_TUBUS)
		AL_PatAl_SPO2_SIQmin
		AL_Sens_SPO2_MODULE_NOTCONNECTED
		AL_Sens_SPO2_SENSORFAULTY
		AL_Sens_SPO2_CHECKSENSOR
		AL_Sens_O2_SENSOR_DEFECT
		AL_Sens_O2_SENSOR_USED
		AL_Sens_O2_VALUE_INCORRECT
		AL_Sens_O2_NOT_CALIBRATED
		AL_Sens_PRICO_FiO2outOfRange
	- FOT:
		- the current used Pmean of FOT will be set as genberal Pmean parameter
		- if ventilation mode will be changed while FOT is running, FOT will be stopped
		- changing any parameter button will stop FOT
	- FOT CONVENTIONAL:
		- when FOT is started -> BACKUP of CPAP will be turned off
		- mode is changed to CPAP while FOT is running, afterwards back to previous mode
	- FOT HFO:
		- step size max 21, from 1...3...5...7...x
		- Pmean start<->Pmean end are limitted by each other (xxx)
		- if graph==autoscale: with each step the scaling of pressure graph will be checked, also after restoring prev HFO values
		- if HFO FOT will be started, VG will be turned off
		- if HFO FOT will be started, lung recruitment will be turned off
		- following alarm stop the FOT:
			AlarmType: AT_SYSFAIL
			AlarmType: AT_SYSALARM
			AlarmType: AT_DISCON_TUBUS
			AL_Sens_FLOW_SENSOR_DEFECT
			AL_Sens_FLOW_SENSOR_CLEANING
			AL_Sens_FLOW_SENSOR_NOTCONNECTED
	- numerics: text size for all alarm limit states auto/off/calc changed to g_hf9AcuBold
	- //rku, PMAX check PEEP+2
	- acuLink: NMODE freq sending changed
	- bugfix VolumeLimit value zero
	- RiseTime can be disabled/hidden by registry key
	- Numerics HFO mode: DCO2 unit "2" fixed
	- //new alarm prio
	- trendfix: 5 days now
	- HFO license deletion correction
	- Altitude setting implemented
	- SpO2 reinit log file fixed
	- //problems for VL=0
	- proximal pressure calibration separation (discuss it)
	- Menu: switching back from WndSubSettings with hard key menu button fixed
	- HFO: Pman>Pmean+0.5mbar
	- manual breath + autoscale->  new conditions for rescale:
		hfo: rescaled only if Pmanual>HighAmlitude value
		cpap, nmode: rescaled to Pman
		other mode: only if VG is on and Pmanual>PmaxVG
	- FOT dependent alarms checked
	- AlarmLimitView: only PIP will be displayed as BTB value, each others as AVERAGE value
	- NumericValues: only PIP will be displayed as BTB value, TVE/TVI as setting in ServiceMenu, each others as AVERAGE value
	- PRICO: FiO2 will not be checked during O2 calibration, after O2 cal the delay will be reset
	- time of etCO2 and SpO2 disconnection detection increased to 4 sec
	- PULSERATE trend included
	- When VG is active in HFO mode the red line is not drawn on the pressure graph while a manual breath is running.
	- alarm limit states are written to system log file
	- PEEP numerics: limit displayed with one decimal digit
	- fabianhfo.ini split for numerics to numericconfig.in
	- MV low alarm limit can be set to off, then in each numeric it will be displayed with warning triangle
	- ventilation STANDBY/STOPPED deactivates O2 flush if running
	- no max key limit in HFO amp + Amp max parameters
	- ETime recruitment limited to 2 min
	- new alarm AL_Sens_PRICO_FiO2outOfRange
	- trend data can only be saved to USB if license is available- O2 flush in alarm limit view only displayed in subpara (yellow field), not in info line
	- view focus through WM messages
	- general view handling changed - application error view switch fixed //rku APPERR
	- HFO mode: type biasflow displayed in yellow field
	- LEAKCOMPENSATION default on -> LEAKCOMPENSATIONOFF_8==FALSE
	- retry of CO2 module through WM message
	- if only wave diagram is changed the loops i.e. are not redrawn
	- RISETIME in PEDIATRIC mode enabled //RISE_NEO
	- auto alarm limits: if no module or no measurement is available, it is set to factory settings, if max and min is available -> both
	- ViewHandler with CriticalSections && refactoring part1 //rku cs1
	- Operating hours reset is deleted from Accuboard submenu in Service.
	- theApp writeLogError implemented
	- exported Device Info file has the name of: "deviceID - DeviceInfo.txt"
	- exported logs have the folder name of: "deviceID - Logs", seperated into FFSDISK and SD
	- exported trends are saved into 1 folder, named "deviceID - Trends"
	- exported Device Info file extended and contains "Next service date:" in one row and "- off" or "- yyyy.mm.dd" below
	- japanese language: mode SIMV+VG + PSV hard coded, same for preset mode
	- disconnection of CO2 module will be displayed at startup with delay of 10 sec, also after reinit
	- CO2: delete deleteAllAlarms reset the state of CO2 module if CO2 alarm was set
	- servcie: "enable BTPS" renamed to "enable O2 comp. "
	- negative scaling of flow graph fixed
	- Device Info: license keys are exported if license is available
	- auto alarm limits PIP/PEEP: PIP alarm limit is always higher than PEEP alarm limit - general diff implemented as DIFF_PMAX_PMIN_LIMIT	and DIFF_PMIN_PMAX_LIMIT
	- changing between VG / VL resets the auto enable flag
	- changeings from 3.3.0 to 4.0.1
		- PEEP / Ppsv difference at max key correct now
		- VL / VG buttons only if license is available
		- VG / VL will be turned off if FLowSensor is turned off, also in IPPV mode
		- color.h replaced by colour.h
		- language ID 12097 included 
		- text "last calibration" will be translated into language now 
		- RiseTime: PEEP limited at 1mbar (file limits_default.reg) 
		- no message for required flow sensor calibration shown in NMODE in case of range change NEONATAL/PEDIATRIC
		- default settings VTEBTB and factory settings fixed
		- lungrec changed (limitted I-Time / Freq)
		- Alarm Thread will not be started again by factory settings
		- the breath buffer is reduced if no breath is detected in case of leak
		- new diagram class
		- configuration for new HFO module implemented
 @endverbatim
 *
 * @subsection ver40117 Version 4.0.1.17
 * @date 2016-06-06
 * @verbatim
	- etCO2 submenu in waves/loops fixed
	- naming version "fabianHFO" fixed
	- when the displayed measured value of SpO2 is equals with either of the alarm limits there is no alarm
	- The lower and higher alarm limit cannot to be set to the same value (difference 5).
	- date/time format of DeviceInfo changed to dd.mm.yyyy hh:mm:ss
	- alarm limit: The upper limit of SpO2 should not be able to be set lower than 2. -> general alarm limit bug fixed (if lost focus)
 @endverbatim
 *
 * @subsection ver40116 Version 4.0.1.16
 * @date 2016-06-01
 * @verbatim
	- volume/flow trigger bit will be send as MODE_OPTION2 like in EVO -> Monitor PIC >=5.0.8 needed
	- HFO PIC version included in sys infos
	- tools: device info file can be saved to USB
	- new license handling, no MASTER key available, if old MASTER detected, all licenses will be active but not PRICO and a new file will be written
 @endverbatim
 *
 * @subsection ver40115 Version 4.0.1.15
 * @date 2016-05-31
 * @verbatim
	- push of alarm silent button will be send allways to SPI
	- new redraw/refresh of CViewDiagramm
	- init/deinit SpO2 blocked
 @endverbatim
 *
 * @subsection ver40113 Version 4.0.1.14
 * @date 2016-05-09
 * @verbatim
	- FREQ para button in case PSV and PresetPSV renamed to BackupRate
	- PRICO will be stopped if ventilation is "stopped/set standby"
	- SpO2 alarms are checked also in THERAPY mode
	- auto O2 calibration will be started at startup after no AIR and O2 alarm is available, else as soon as there is no more one of these alarms active
	- while O2 cal running -> the numeric field OXY displays "--"
	- startup condition FiO2 set when PRICO started
	- if measured SpO2 val is out of alarm limits the value of PRICO range is flushing yellow at the range
	- Prico started: if measured FiO2 is out of range of PRICO FiO2 settings, the range adjusts the setting
	- in case of O2 calibration PRICO uses the applied value: getMessureDataO2calindependent //rku O2
	- Oxy parameter is in case of running PRICO a backup setting, O2 flush is possible while PRICO -> PRICO will be stopped
	- SpO2 alarm limits fixed
	- stopping PRICO deletes general alarm “PRICO: FiO2 out of range”
	- SIQ alartm limit: autoset -> value is set to 30, no auto displayed
	- memory test switched from watchdog to timer function
	- PRICO alarm AL_PRICO_FiO2limit: in case of measuered oxy value below 21%: alarm message + "- check O2 calibration!"
	- flickering of clock / PRICO fixed
	- SpO2 wave renamed to PLETH (hard coded all languages)
	- When FiO2 range is changed while PRICO is running and the current FiO2 would be outside, then it is immediately set to the closest allowed value
	- The absolute value of the calculated FiO2 steps is limited to 10% O2
 @endverbatim
 *
 * @subsection ver40113 Version 4.0.1.13
 * @date 2016-04-28
 * @verbatim
	- //PRICO03
	- Dependency PEEP <-> Pmax: DIFF_PEEP_PINSP will be only checked in case of VG control //rku, VGVL check
	- if VL control will be changed to VG control: DIFF_PEEP_PINSP will be checked  and if required Pmax increased to PEEP+DIFF_PEEP_PINSP //rku, VGVL check
	- Ppsv max key only checked if Ppsv != Pinsp
	- new PRICO entries in log file
	- //todo translate
 @endverbatim
 *
 * @subsection ver40112 Version 4.0.1.12
 * @date 2016-04-11
 * @verbatim
	- //todo translate
	- bool variable of threads thread safe now
	- CInterfaceAcuLink implemented as base class
	- FOT terminal set to 250Hz (PRESSURE, FLOW)//FOT-TERM1
	- operating time overflow 4294967295 millisec fixed
	- leak compensation menu is visible only if there is nMODE license available
 @endverbatim
 *
 * @subsection ver40111 Version 4.0.1.11
 * @date 2016-04-06
 * @verbatim
 - SpO2 and CO2 can be disabled/hidden through registry key "HKCU\\Software\\FabianHFO\\WorkState": 
	"SPO2" value 0 == enabled (default), "CO2" value 0 == enabled (default)
 @endverbatim
 *
 * @subsection ver40110 Version 4.0.1.10
 * @date 2016-04-06
 * @verbatim
	- Testversion FOT disabled
 @endverbatim
 *
 * @subsection ver4019 Version 4.0.1.9
 * @date 2016-03-23
 * @verbatim
	- Maintenance included to Service: Service time, Operation time, System silent, Password enable/disable (excluded from System Menu)
	- FOT buttons enabled (conventional mode)
	- test terminal FOT is disabled in SPI //FOT-TERM1
	- AlarmLog: code will be written correct: CHECK_BLENDER code
 @endverbatim
 *
 * @subsection ver4018 Version 4.0.1.8
 * @date 2016-03-21
 * @verbatim
	- NEBULIZER uncommented
	- O2 flush can be disabled
 @endverbatim
 *
 * @subsection ver4017 Version 4.0.1.7
 * @date 2016-03-18
 * @verbatim
	- LeakCompensation on/off for NCPAP/DUOPAP included
	- Submenu Ventilation Settings new sequence
	- //PRICO04
 @endverbatim
 *
 * @subsection ver4016 Version 4.0.1.6
 * @date 2016-03-09
 * @verbatim
	- FOT disabled //FOT02 //todoFOT
	- //PRICO03
	PRICO: the calculated O2 adjusts now the O2 para while running and also O2flush keeping the difference
	while PRICO is runnung O2 and O2flush para are disabled, O2 flush is not possible -> warning will be displayed if flush button pressed
 @endverbatim
 *
 * @subsection ver4015 Version 4.0.1.5
 * @date 2016-03-09
 * @verbatim
	- //PRICO03
		PRICO set the O2 para while running and also O2flush with difference
		while PRICO is runnung O2 and O2flush para are disabled, O2 flush is not possible -> warning will be displayed if flush button pressed
	- test terminal FOT disabled in SPI
	- CViewService with critical section
	- RiseTime disabled in NEO mode, if range changes to NEO and RiseTime is enabled it will be disabled, will be also checked at startup and reset to factory //RISE_NEO
	- 20160310 FWI: alarm AL_TUBUSBLOCKED, AL_DISCONNECTION, AL_SysLimit_Vgaranty, AL_SysLimit_Vlimitted, AL_SysLimit_Pinsp_NotReached in case of silent state ASTATE_AUTOSILENT not signaled //20160310FWI
	- 20160310 FWI: autoscale Flow diagram to minimum fixed
	- E-Flow checked at startup and if range switched for limits //EFLOW_CHECK
	- Manual breath: in autoscale the diagram will be scaled regarding the pressure of given manual breath //MANBREATH_SCALE
 @endverbatim
 *
 * @subsection ver4014 Version 4.0.1.4
 * @date 2016-03-08
 * @verbatim
	- test terminal for FOT -> SPI with 333hz //FOT-TERM1
	- EFlow quick settings will be only displayed in conventional mode//EFLOW1
	- EFLow settings menu: value is only send to PICs in conventional mode
	- changing VG / VL will only affect conventional mode, not HFO
	- //PMAN uncommented
	- Pmanual: separate values for HFO and CPAP mode
 @endverbatim
 *
 * @subsection ver4013 Version 4.0.1.3
 * @date 2016-03-02
 * @verbatim
	- test terminal for FOT
 @endverbatim
 *
 * @subsection ver4012 Version 4.0.1.2
 * @date 2016-02-26
 * @verbatim
	- //PRICO02
	- Acutronic logo excluded from CViewFullscreenMsg
	- vent range changed: flow sensor state will be refreshed //FLOW1
	- TlsMessageBox deleted from TlsLanguage.h
	- trend Vte scaling fixed
	- bugfix trend (cursor position and Min/Max range regarding Y-axis) //newtrend
		 G_UPPER_MAXSCALE_PRESSURE			100	
		 G_LOWER_MAXSCALE_PRESSURE			10	
		 G_UPPER_MAXSCALE_FIO2				1200
		 G_LOWER_MAXSCALE_FIO2				200
		 G_UPPER_MAXSCALE_VTE				600	
		 G_LOWER_MAXSCALE_VTE				10
		 G_UPPER_MAXSCALE_COMPL				120	
		 G_LOWER_MAXSCALE_COMPL				10
		 G_UPPER_MAXSCALE_DCO2				5000	
		 G_LOWER_MAXSCALE_DCO2				10
		 G_UPPER_MAXSCALE_MV					10000	
		 G_LOWER_MAXSCALE_MV					100
		 G_UPPER_MAXSCALE_HFAMP				100	
		 G_LOWER_MAXSCALE_HFAMP				10
		 G_UPPER_MAXSCALE_RSBI				2500	
		 G_LOWER_MAXSCALE_RSBI				50
		 G_UPPER_MAXSCALE_SHAREMVMAND		120	
		 G_LOWER_MAXSCALE_SHAREMVMAND		10
		 G_UPPER_MAXSCALE_RESISTANCE			10000	 
		 G_LOWER_MAXSCALE_RESISTANCE			100
		 G_UPPER_MAXSCALE_LEAK				120	
		 G_LOWER_MAXSCALE_LEAK				10
		 G_UPPER_MAXSCALE_SPO2PI				200	
		 G_LOWER_MAXSCALE_SPO2PI				50
		 G_UPPER_MAXSCALE_SPO2				120	
		 G_LOWER_MAXSCALE_SPO2				10
		 G_UPPER_MAXSCALE_CO2				160	
		 G_LOWER_MAXSCALE_CO2				10
 @endverbatim
 *
 * @subsection ver4011 Version 4.0.1.1
 * @date 2016-02-18
 * @verbatim
	- FOT conventional mode - research
 @endverbatim
 *
 * @subsection ver4010 Version 4.0.1.0
 * @date 2016-01-26
 * @verbatim
	- spi interface: new SPI error handling regarding new PIC, check alive //newSPI
	- start FOT implementation -> //todoFOT
	- Pmax VG is independend from Pinsp/Pbackup
	- CodeReview ViewDiagramm, refactoring and reorganization of critical sections
	- VolumeLimit drawn with 1 digit [0.0] in graph
	- ViewAlarmLimits: the focus will be only set to the alarm limits if the view owns the focus
	- drawing text field correct now
	- fix: disabling SPO2 and leaving CWndSubSettingsSPO2Module with button EV_BN_MATRIX_GRAPHS
	- serial interface: check routine throws WM_SETALARM_IF_COM_REINIT after 2 read error //newCOMCHECK
	- text of wave digarmms vertical aligned
	- new ACUTRONIC logo included
	- //newtrend
 @endverbatim
 *
 * @subsection ver4000 Version 4.0.0.0
 * @date 2016-01-13
 * @verbatim
	- //newVG
	- E-Flow menu settings: text included: (default: 6 l/min)
	- Tau: text "Time Constant" included into numeric
	- new absolute allowed minimum / maximum registry keys, see limits_default20160118.reg
	- new limits_default20160118.reg needed: MINRANGE_NEO_VGARANT_IPPV, MINRANGE_NEO_VGARANT_TRIGGER default changed to 8ml
	- Pinsp parameter available in PSV mode
	- PSV: VG on -> Pbackup is used as manual breath, Ppsv value will not be send as Pinsp
	- Pmax parameter is own command to PICs now (Ctrl: "<", ">", Monitor: address 30) 
	- Pmax >= Pinsp >= Ppsv
	- PSV mode: Pinsp para & Ppsv para greyed
	- SIMV-PSV mode: Pinsp para greyed
	- check routine of serial improved
	- ConfigVersion 3002: factory default values changed: UseTveBTB=true, EnableFactory=false, [HKEY_LOCAL_MACHINE\Platform]"AcuVersion"="V2.0"
	- Autoscale: switching between CPAP and conventional mode will result in redraw the scaling
	- Graph of Volume scale: minimum changed from 10ml to 2ml
	- PSV mode: Pinsp renamed to Pbackup
	- operating times new implemented //newOPtime
	- Pbackup, Ppsv and Pmax are sent also if greyed
	- CO2: text changed from "Generic Mainstream" to "Respironics (Philips)" due to support of Philips Mainstream Sensor
	- Unit of MV in chinese language: [l/min]
	- o2 value below 21% possible, will be displayed in subpara as "--"
	- CodeReview: check for correct closing of all windows and threads
	- CodeReview: checked for correct handling alarm array
	- CO2MODULE_MICROPOD: only one state will be displayed
	- CWndNumConfig::drawCursor() fixed
	- Logfile as thread
	- hwConfig.ini Version 1.1 needed for different HFO modules, will be send to PICs
	- DUOPAP: RiseTime ignored
	- FLowSensor Calibration fixed
	- VG / VL will be turned off if FLowSensor is turned off, also in HFO mode
	- SpO2 wave will not be drawn in green trigger color
	- before "reset complete" OpTime is saved
	- Logs are saved on SDcard if available
	- trend will be saved SDcard if available
	- Diagram: line strength correct used after switch from/to HFO mode
	- PRICO included:
		- new PRICO alarms:  -> PRICO will be turned off
			AL_PRICO_FiO2limit: if the calculated FiO2 is out of FiO2 limits, priority 1 "red"
			AL_PRICO_SpO2value: if the current measured SpO2 value is outside of 10% of the average value, priority 1 "red"
			AL_PRICO_SIQvalue: limit can be selected in  SpO2 settings, priority 1 "red"
		- settings are written to logfile
		- FiO2 handling:	PRICO inactive-> the parmeter FiO2 is send to PICs
							PRICO active-> the calculated FiO2 is send to PICs
							O2 flush active: value of flush is send to PICs
							O2 flush stops: PRICO inactive -> the parmeter FiO2 is send tp PICs
											PRICO active -> the calculated FiO2 is send to PICs
							PRICO stops:  -> the parmeter FiO2 is send tp PICs
		- licensing implemented for PRICO
		- the SIQ-alarm could be set now in the SpO2 menu
		- O2 para button is inactive while PRICO running
	- if PRICO license is not available, only the senosr state is displayed
	- if SIQ limit is "0" (zero), it will be displayed as "off"
	- in case of following alarms PRICO will be turned off:
		AL_PatAl_SPO2_SIQmin
		AL_DISCONNECTION
		AL_TUBUSBLOCKED
		AL_Sens_SPO2_MODULE_NOTCONNECTED
		AL_Sens_SPO2_SENSORFAULTY
		AL_Sens_SPO2_CHECKSENSOR
		AL_PRICO_FiO2limit
		AL_PRICO_SpO2value
		AL_Sens_O2_SENSOR_DEFECT
		AL_Sens_O2_SENSOR_USED
		AL_Sens_O2_VALUE_INCORRECT
		AL_Sens_O2_NOT_CALIBRATED
		AL_SysAl_P_IN_O2
		AL_SysAl_P_IN_AIR
		AL_SysFail_P_IN_MIXER
		AL_SysAl_TUBE_OCCLUSION
	- if O2 sensor will be turned off -> PRICO will be turned off
	- acuLink: ITimePSV will be send now in PSV and SIMV/PSV mode to acuLink -> Philips EC4080
	- new handling serial interface regarding new BOOTLOADER implemented
	- PSV: VG on -> Pmax is used as manual breath, Ppsv value will not be send as Pinsp
	- ConfigVersion 3002: factory default values changed: UseTveBTB=true, EnableFactory=false, 
		[HKEY_LOCAL_MACHINE\Platform]"AcuVersion"="V2.0", platformV2.reg needed for firts installation at production, old systems will be updated automatic
	- CViewParaTbn: CriticalSection(&csDraw) implemented
	- GetTickCount64 implemented
  @endverbatim
 *
 * @subsection ver3300 Version 3.3.0.0
 * @date 2015-03-11
 * @verbatim
	(Ctrl 3.4.02 / Mon 2.9.28 bzw 3.9.28 / HFO 3.0.1 / Blender 5.3 / Acu 5.0) 
	 - VG: alarm Vte not reached -> in PRESET mode the arrow in Pmax / AMPmax will not be displayed
	 - date change entry in alarm log included
	 - AutoIFlow included
	 - RiseTime included
	 - Delay Pmin in CPAP included, measurement Pmin will not be checked anymore
	 - Startup view calibration changed
	 - setting NEO/PED moved from ventilation menu to startup calibration view
	 - O2 calibration view changed
	 - setting CO2 and SpO2 sensor moved from ventilation menu to startup calibration view
	 - no different SpO2 alarm limits between the modes
	 - new HARDWARE configuration included, installed by SetupFabian3.x, displayed in Service-Menu -> System:
		System-Hardware Revision: SysHW (new Revision=5, March 2015)
		Device-Type: HFO, EVO, THERAPY
		FlowSensorRange: FlowSens: Neo / FlowSens: Ped(=Neonatal+Pediatric)
		PatientGasSupply (ATG): Rev.1-3, Rev.4
	- new HARDWARE configuration will be send to MonPIC + CtlPIC
	- AutoIFlow excluded
	- O2 calibration view: buttons to calibrate without text "O2"
  @endverbatim
 *
 * @subsection ver3211 Version 3.2.1.1
 * @date 2015-01-19
 * @verbatim
	Ctrl 3.4.02 / Mon 2.9.28 bzw 3.9.28 / HFO 3.0.1 / Blender 5.3 / Acu 5.0) 
	- Ppsv is not limited against Pmax
  @endverbatim
 *
 * @subsection ver3200 Version 3.2.1.0
 * @date 2015-01-19
 * @verbatim
	(Ctrl 3.4.01 / Mon 2.9.27 bzw 3.9.27 / HFO 3.0.1 / Blender 5.3 / Acu 5.0) 
	- Merge of FiCO2
	- VG auto on/off in case of ET tube alarm
	- HFO: AMPLmax range will be affected by PowerSupply correct
	- Bugfix: AlarmLimitView: button Log text correct now
	- Bugfix: Warning in PEEP button (PSV) correct now
	- Bugfix: AlarmLogList: double text in case PEEP low deleted
  @endverbatim
 *
 * @subsection ver3200 Version 3.2.0.0
 * @date 2014-12-22
 * @verbatim
	(Ctrl 3.4.01 / Mon 2.9.27 bzw 3.9.27 / HFO 3.0.1 / Blender 5.3 / Acu 5.0) 
	- Branch FiCO2
	- ConfigVersion: 3001
	- Flowsensor off and etCO2 Modul enabled -> Freq will be alarm checked of CO2 Freq
	- CAPNOSTAT: gas compensation will be sent again if module does not takes the value
 @endverbatim
 *
 * @subsection ver31023 Version 3.1.0.23
 * @date 2015-01-05
 * @verbatim
	(Ctrl 3.4.01 / Mon 2.9.27 bzw 3.9.27 / HFO 3.0.1 / Blender 5.3 / Acu 5.0) 
	- CAPNOSTAT: gas compensation will be sent again if module does not takes the value
 @endverbatim
 *
 * @subsection ver31022 Version 3.1.0.22
 * @date 2014-12-18
 * @verbatim
	(Ctrl 4.0.0 / Mon 2.9.23 bzw 4.0.0 / HFO 3.0.0 / Blender 5.3 / Acu 5.0) 
	- BTB is default for Vte value
	- button factory settings disabled as default
 @endverbatim
 *
 * @subsection ver31021 Version 3.1.0.21
 * @date 2014-12-03
 * @verbatim
	(Ctrl 4.0.0 / Mon 2.9.23 bzw 4.0.0 / HFO 3.0.0 / Blender 5.3 / Acu 5.0) 
	- serial IOCTL_COM_xxx from F&S excluded -> new kernel from F&S
	- trends will be drawn till end of diagramm
	- if VG will be enabled, actual TAU value will be written to systemlog
 @endverbatim
 *
 * @subsection ver31020 Version 3.1.0.20
 * @date 2014-11-21
 * @verbatim
	(Ctrl 3.3.26 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- serial IOCTL_COM_xxx from F&S included
 @endverbatim
 *
 * @subsection ver31019 Version 3.1.0.19
 * @date 2014-11-20
 * @verbatim
	(Ctrl 3.3.26 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- CInterfaceSerial::SendSerialData imroved
 @endverbatim
 *
 * @subsection ver31018 Version 3.1.0.18
 * @date 2014-11-19
 * @verbatim
 (Ctrl 3.3.26 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- CInterfaceSerial::SendSerialData changed
 @endverbatim
 *
 * @subsection ver31017 Version 3.1.0.17
 * @date 2014-11-15
 * @verbatim
 (Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- EnterCriticalSection(&csMSGSend) replaced with TryEnterCriticalSection(&csMSGSend) in CInterfaceSerial::SendSerialData thread (will not lock if WriteFile fails/locks)
 @endverbatim
 *
 * @subsection ver31016 Version 3.1.0.16
 * @date 2014-11-10
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- serial class: if check of controller PIC fails once, all settings are sent again
	- numeric IPPV: display PIP fixed
	- in case of mode change the settings are sent only once
	- in case of serial error, next try each 10 seconds
	- VG on/off: alarm silent 15 sec is activated
 @endverbatim
 *
 * @subsection ver31013 Version 3.1.0.13
 * @date 2014-11-07
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- language files for new features included
	- naming Ppeak changed to PIP
 @endverbatim
 *
 * @subsection ver31012 Version 3.1.0.12
 * @date 2014-11-06
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- memory leak in CViewParaBtn fixed
	- new reinitialization of serial IF in case of problems
 @endverbatim
 *
 * @subsection ver31011 Version 3.1.0.11
 * @date 2014-11-05
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- PSV mode: if alarm limits are state auto -> changing Ppsv will affect alarm limit Ppeak
 @endverbatim
 *
 * @subsection ver31010 Version 3.1.0.10
 * @date 2014-11-05
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- check routine of serial with try of reinit serial befor alarming
	- alarm limit of Ppeak will be changed if state==auto and VG will be enabled/disabled
 @endverbatim
 *
 * @subsection ver3109 Version 3.1.0.9
 * @date 2014-11-04
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- scaling of etCO2 trend fixed
	- additional entry log: screenlock state and auto time
 @endverbatim
 *
 * @subsection ver3108 Version 3.1.0.8
 * @date 2014-11-03
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- test trend etCO2
 @endverbatim
 *
 * @subsection ver3107 Version 3.1.0.7
 * @date 2014-11-03
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- PSV mode: if VG is enabled, value Ppsv will be displayed grey
	- new log entries regarding threading
  @endverbatim
 *
 * @subsection ver3106 Version 3.1.0.6
 * @date 2014-10-30
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- PSV mode: Pinsp is replaced by Ppsv parameter button
	- auto scaling of pressure graph adapted to state of volume guarantee
	- alarm limit of Pmax (Ppeak) adapted to state of volume guarantee
	- BiasFLow enable/disable with registry key: "HKCU\\Software\\FabianHFO\\WorkState" -> "BiasFlow"
 @endverbatim
 *
 * @subsection ver3105 Version 3.1.0.5
 * @date 2014-10-29
 * @verbatim
 (Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- Sleep() included in case of blocking I2C
	- define OWN_ACULINK included
	- at STARTUP patient alarms are ignored for 120 seconds (or until alarm silent is changed manually)
	- scaling of VOLUME wave/loop changed: NEO mode: max 60ml / PED mode max. 300ml
	- range volume guaranty HFO: NEO mode: 0.3ml - 30ml / PED mode: 10ml - 100ml
	- range volume guaranty conventional: NEO mode: 1.2ml - 60ml / PED mode: 10ml - 300ml
	- range flow in therapy PED: 1 - 25 ml
	- possible double click time of HW buttons reduced to 150 ms
	- BoardSubType will be written to log file
	- color of Pmax (VG) line in pressure graph changed to orange
	- EFlow parameter: 6 l/min displayed as default
	- ventilation menu: possibility for beep of backup deleted
	- ventilation menu: possibility for beep of vlimit deleted
	- ventilation menu: possibility for flush time deleted
	- menu: order of buttons changed
 @endverbatim
 *
 * @subsection ver3104 Version 3.1.0.4
 * @date 2014-10-19
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- correct EFlow check over serial in Therapy mode
	- EFlow in ventilation menu with correct unit of range
 @endverbatim
 *
 * @subsection ver3103 Version 3.1.0.3
 * @date 2014-10-17
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- Ppsv parameter will not bechecked in PSV mode
 @endverbatim
 *
 * @subsection ver3102 Version 3.1.0.2
 * @date 2014-10-15
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- EFlow parameter in ventilation menu / configurable for settings menu
	- Numerics can be changed in PRESET mode
	- in PRESET mode a signaled alarm (previous alarm) is not displayed in info line
 @endverbatim
 *
 * @subsection ver3101 Version 3.1.0.1
 * @date 2014-08-25
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- japanese language: Date/Time displayed with smaller font size
	- Text "lungrecruitment" only visible if lungrecruitment license is enabled
	- alarm volume guarantee has same priority like patient alarms
	- VG: new info line text: SIMV + VG + PSV / SIMV + VG + PSV-PRESET
	- new VG
 @endverbatim
 *
 * @subsection ver3100 Version 3.1.0.0
 * @date 2014-08-20
 * @verbatim
	(Ctrl 3.3.91 / Mon 2.9.23 bzw 3.9.23 / HFO 2.9.5 / Blender 5.3 / Acu 5.0) 
	- AL_Accu_60 and AL_Accu_30 has alarm priority middle
	- flag //TESTVERSION PSV
	- BTPS can be enabled/disabled in Service -> Calibration menu
	- possibility of flow sensor correcting factor NEO and PEDIATRIC in Service -> Calibration menu
	- blocking of select etco2 module will be canceled after startup and deinit phase
	- additional entries in system log for etco2
	- if ventilation stopped/standby the manual breath will be ignored
	- Setting delay Pmin NCPAP/DUOPAP mnoved to Alarmlimits
	- in case of failed loading of hospital settings the config will not be reloaded
	- TAG //VG_VL_NEW for new VolumeGuarantee
	- test inbiolab, flag with //test inbiolab
	- test closed loop, flag with //test closedloop
	- CPAP: will be saved as PRESET value!
	- no change of wave view when VG is turned on
	- no change of wave view in case of VG alarm
	- PSV criteria changed to 5 ... 35 %
	- more trends included: TREND_RSBI, TREND_SHAREMVMAND, TREND_RESISTANCE, TREND_LEAK, TREND_SPO2, TREND_SPO2PI, TREND_ETCO2
	- if SpO2 alarm of any limit is active and SpO2 module will be deactivated the alarms are deleted
	- Alarm limit leak can be turned off at high and low limit range
	- VolumeGuarantee off -> last Pinsp will be set, if PinspVG is not below the last Pinsp -> then Pinsp will be set to PinspVG
	- RSBI without unit now
	- Alarm log will be written only using english definitions
	- font loading changed
	- CO2 Module -> new initialization (blocked while not initialized)
	- Freshgas external/internal implemented
	- new setting VG / VL
	- Ranges of parmeters adjustable with registry file:
	[HKEY_CURRENT_USER\Software\FabianHFO\WorkState\Limits]
	
 @endverbatim
 *
 * @subsection ver30035 Version 3.0.0.35
 * @date 2014-05-07
 * @verbatim
	(Ctrl 3.3.17 / Mon 2.9.18 bzw 3.9.18 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- SpO2 and CO2 is independent from Flowsensor (on/off)
 @endverbatim
 *
 * @subsection ver30034 Version 3.0.0.34
 * @date 2014-03-25
 * @verbatim
	(Ctrl 3.3.17 / Mon 2.9.18 bzw 3.9.18 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- only new update process is supported now: SetupFabian.exe
	- SpO2: when enabled but no USB device is connected jet, an alarm appears after 30 seconds: SpO2 not connected
	- if ventilation is paused, exhalation valve calibration is not possible
	- UPDATE: only possible now with the new SetupFabian process
	- Numeric SIPPV Mechanic block: Compliance now with correct unit
 @endverbatim
 *
 * @subsection ver30033 Version 3.0.0.33
 * @date 2014-03-21
 * @verbatim
	 (Ctrl 3.3.17 / Mon 2.9.17 bzw 3.9.17 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- Flow HF range checked at startup: 5 - 20, default 8 l/min
	- "Oridian" changed to "Covidien"
	- SpO2 Rate: automatic calculation of alarm limits in 5bpm steps
 @endverbatim
 *
 * @subsection ver30032 Version 3.0.0.32
 * @date 2014-03-17
 * @verbatim
	(Ctrl 3.3.17 / Mon 2.9.16 bzw 3.9.16 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- changed CO2 Respironics to "generic mainstream"
	- Service mode ->  System -> hardware configuration: type of displayed added 
 @endverbatim
 *
 * @section ver Version history and changelog
 *
 * @subsection ver30031 Version 3.0.0.31
 * @date 2014-03-14
 * @verbatim
	(Ctrl 3.3.17 / Mon 2.9.16 bzw 3.9.16 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- while logfiles are copied to USB no writing is supported
	- Log is now static
 @endverbatim
 *
 * @subsection ver30030 Version 3.0.0.30
 * @date 2014-03-04
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- FlowCorrecture Button is now in the calibration menu
	- I-Flow / E-Flow and FSACL from flowboard menu (service) excluded
	- New CO2 message: CO2 pump turned on
	- automatic restart of CO2 pump initialized
 @endverbatim
 *
 * @subsection ver30029 Version 3.0.0.29
 * @date 2014-02-24
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- in case of alarm state changing of apnea state the etCO2 will reset the breath algorithm
 @endverbatim
 *
 * @subsection ver30028 Version 3.0.0.28
 * @date 2014-02-14
 * @verbatim
 (Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- mainboard version will be written to system log
	- bug fixed in alarm limit view in case of etCO2 and SpO2 alarms
	- bug fixed alarm limit SpO2 and etCO2 -> refresh
 @endverbatim
 *
 * @subsection ver30027 Version 3.0.0.27
 * @date 2014-02-06
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- personalization of tools excluded
 @endverbatim
 *
 * @subsection ver30026 Version 3.0.0.26
 * @date 2014-02-04
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- startup double beeb longer delay
 @endverbatim
 *
 * @subsection ver30025 Version 3.0.0.25
 * @date 2014-01-30
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- leak alarm limits in HFO mode implemented //todo HFOleak
 @endverbatim
 *
 * @subsection ver30024 Version 3.0.0.24
 * @date 2014-01-29
 * @verbatim
 (Ctrl 3.3.16 / Mon 2.9.15 bzw 3.9.15 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- MesData_Resistance_Max changed to 9990
 @endverbatim
 *
 * @subsection ver30023 Version 3.0.0.23
 * @date 2014-01-29
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.14 bzw 3.9.14 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- alarm limits BPM: calculating value checked against lower and upper limit
	- DEMOVERSION -not for clinical use-
 @endverbatim
 *
 * @subsection ver30022 Version 3.0.0.22
 * @date 2014-01-28
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.14 bzw 3.9.14 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- Trends: DCO2 value will be drawn with correct scale factor
 @endverbatim
 *
 * @subsection ver30021 Version 3.0.0.21
 * @date 2014-01-22
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.13 bzw 3.9.13 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- checked with ENTREK code snitch for Memory leaks
 @endverbatim
 *
 * @subsection ver30020 Version 3.0.0.20
 * @date 2014-01-20
 * @verbatim
	(Ctrl 3.3.16 / Mon 2.9.13 bzw 3.9.13 / HFO 2.0.1 / Blender 3.6 oder 4.2 / Acu 5.0) 
	- HFO manual scaling of wave/loop: fixed moving scale
	- Alarm limits + O2Therapy: Focus will not be lost anymore
	- CTime and CTime::GetCurrentTime() through COleDateTime and GetLocalTime(&st); replaced -> Memory Leak in Window class!
	- checked with ENTREK code snitch for Memory leaks
 @endverbatim
 *
 * @subsection ver30019 Version 3.0.0.19
 * @date 2014-01-16
 * @verbatim
	(Ctrl 3.3.15 / Mon 2.9.13 bzw 3.9.13 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- automatic Alimits of MV will be rounded correctly for acuLink
	- at shutdown the operating times are logged
 @endverbatim
 *
 * @subsection ver30018 Version 3.0.0.18
 * @date 2013-12-30
 * @verbatim
	(Ctrl 3.3.15 / Mon 2.9.13 bzw 3.9.13 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- Trend will be exported to USB stick with all data
 @endverbatim
 *
 * @subsection ver30016 Version 3.0.0.16
 * @date 2013-11-28
 * @verbatim
	(Ctrl 3.3.15 / Mon 2.9.13 bzw 3.9.13 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- Chinese Language: no arial font will be used anymore
	- Ppsv will be set correctly
	- Ppsv fixed (absolute value and above PEEP)
	- Ppsv: no key anymore for higher range
	- auto alarm limit of Pinsp, CPAP, NCPAP, DUOPAP: 5 mbar above setting
	- color of menu btns changed to dark blue
	- if 100% oxy is calibrated, the state is displayed now as "100% calibrated"
	- O2 value: in O2 calibration, service view MonitorPIC and Flowboard with one digit now
	- bootsequency faster now
	- HFO: volume wave/loop minimum scale is 2ml 
	- HFO: waves with right colors of ini file now
	- new numeric button shows current block number
	- in case of NMODE: pressure graph y-scale is only scalable 10mbar or 20mbar
	- alarm limits DUOPAP / NCPAP independent from each other
	- AutoIFlow deactivated
	- Alarm limits PEEP min / max:
	PEEP max:	is checked in case on CPAP and NCPAP mode, but after a manual and mechanic breath there is a delay: in case of CPAP delay of 300ms, in case of NCPAP a delay of 3000ms
	if no manual and mechanic breath is running and this delay has passed, the PEEP max will be checked against the alarm limit. If the value is above the alarm limit of PEEP max a delay of 5000ms is started
	after this delay has passed and the value is still above the limit an alarm occurs. as soon as the value is below the alarm limit, the delay will be aborted
	PEEP min:	is checked in case on DUOPAP and NCPAP mode, if no mechanical breath runs. for the NCPAP mode there is the same delay after manual and mechanical breath as described in PEEP max.
	if no manual and mechanic breath in case NCPAP and no mechanic breath in case DUOPAP is running and this delay has passed, the PEEP min will be checked against the alarm limit.
	If the value is below the alarm limit of PEEP min the delay of the settings menu (delay Pmin alarm (NCPAP/DUOPAP)) is started. This delay is only aborted if the next breath is in range of the alarm limit. Otherwise if the dealy has passed an alarm is generated.
	- Ppsv above PEEP/absolute value fixed
	- Trend will be exported to USB stick with all data
 @endverbatim
 *
 * @subsection ver30015 Version 3.0.0.15
 * @date 2013-11-21
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.12 bzw 3.9.12 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- HFO: value 8 is displayed as default
	- etCO2 Covidian: if pump is turned off with timer and timer is set to zeor, the times is killed
	- Numeric values are blue
 @endverbatim
 *
 * @subsection ver30014 Version 3.0.0.14
 * @date 2013-11-18
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.11 bzw 3.9.11 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- Test
 @endverbatim
 *
 * @subsection ver30013 Version 3.0.0.13
 * @date 2013-11-18
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.11 bzw 3.9.11 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	 - Test version with ADC values in Info and faster startup
 @endverbatim
 *
 * @subsection ver30012 Version 3.0.0.12
 * @date 2013-11-14
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.11 bzw 3.9.11 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- Autoscale of disconnection removed
 @endverbatim
 *
 * @subsection ver30011 Version 3.0.0.11
 * @date 2013-11-13
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.11 bzw 3.9.11 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- CPAP: in case of a manual breath, the mechanical breath (BACKUP) is ignored
 @endverbatim
 *
 * @subsection ver30010 Version 3.0.0.10
 * @date 2013-11-12
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.11 bzw 3.9.11 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- available FlowAC values can be configured in fabianHFO.ini "FLOWAC=TRUE"
	- #define FRANZ_VERSION included: Presure of air and oxy in Info Zeile + schnelleres Startup
	- factory settings will not touch the setting of Ppsv value (absolute/above PEEP)
 @endverbatim
 *
 * @subsection ver30009 Version 3.0.0.9
 * @date 2013-11-07
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.10 bzw 3.9.10 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- unit of Vte changed to [ml]
	- check of acuLink installation in service menu:
		no AcuLink.exe"Installation: #001"
		no AcuLink_DLL.dll"Installation: #002"
		GetPDMSprotocol()==ACL_NOPDMS"Installation: #003"
		GetPDMSprotocol()==ACL_TERMINAL"Installation: #004"
		"Installation: ok");
 @endverbatim
 *
 * @subsection ver30008 Version 3.0.0.8
 * @date 2013-11-07
 * @verbatim
	(Ctrl 3.3.14 / Mon 2.9.10 bzw 3.9.10 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	- hungarian language included
	- in case of NCPAP and manual breath, at end of manual brath the alarm delay of the menu is started
 @endverbatim
 *
 * @subsection ver30007 Version 3.0.0.7
 * @date 2013-10-17
 * @verbatim
	 (Ctrl 3.3.14 / Mon 2.9.10 bzw 3.9.10 / HFO 2.0.1 / Blender 3.4 oder 4.1 / Acu 5.0) 
	 - after factory and hospital setting a messaqe is displayed: configuration reloaded
	 - missing icons in message box replaced
	 - Ppsv above PEEP: fixed
	 - configurable numerics
 @endverbatim
 *
 * @subsection ver30006 Version 3.0.0.6
 * @date 2013-09-24
 * @verbatim
	(Ctrl 3.3.13 / Mon 2.9.9 bzw 3.9.9 / HFO 2.0.1 / Blender 3.4 oder 4.1)
	- etCO2 fixed
	- etCO2 values interpolated
	- Numeric PSV mechanics bugfixing
	- DemandFlow bugfixing
	- Command to SPI if shutdown
	- Lungrecruitment activated
	- American language included
	- Norway language included
	- new entry in service -> network:	if no AcuLink.exe installed: "Installation: #001"
										if no AcuLink_DLL.dll installed: "Installation: #002"
										if GetPDMSprotocol()==ACL_NOPDMS: "Installation: #003"
										if GetPDMSprotocol()==ACL_TERMINAL: "Installation: #004"
										else "Installation: ok"
	- Configuration version / hospital settings: version 2008 and higher are compatible now
	- color of waves/loops configurable via ini-file
	- if acuLink is not available, the reason is written to the syslog
	- manual scaling of diagramm in O2 therapy mode is possible now
	- Systemlog entries changed to code numbers
 @endverbatim
 *
 * @subsection ver30005 Version 3.0.0.5
 * @date 2013-09-24
 * @verbatim
	(Ctrl 3.3.13 / Mon 2.9.9 bzw 3.9.9 / HFO 2.0.1 / Blender 3.4 oder 4.1) ??????????????????????????????????
	- regarding AutoIFlow: following parameter will be displayed: 
	I-Flow/E-Flow:  I-Flow parameter, E-Flow parameter
	Ramp:  only Plateau parameter
	AutoIFlow: no I-Flow parameter, no E-Flow parameter, no Plateau parameter
 @endverbatim
 *
 * @subsection ver30004 Version 3.0.0.4
 * @date 2013-09-13
 * @verbatim
	(Ctrl 3.3.13 / Mon 2.9.9 bzw 3.9.9 / HFO 2.0.1 / Blender 3.4 oder 4.1) ??????????????????????????????????
	- Decrease Contrast -> at minimum an audio signal is now produced
	- Ppsv: Above PEEP -> text will be displayed correctly
	- ForceShutdown implemented !!! -> independent from USB stick type
	- DevcieID will be written to SystemLog
	- SpO2 value rounded to integer value
	- old configureable numerics excluded
	- in case of manual breath "Backup active" will not be displayed in CPAP
	- manual breath: maximum allowed time increased to 30 seconds
 @endverbatim
 *
 * @subsection ver30003 Version 3.0.0.3
 * @date 2013-07-27
 * @verbatim
	(Ctrl 3.3.12 / Mon 2.9.8 bzw 3.9.8 / HFO 2.0.1 / Blender 3.4 oder 4.1)
	- G_FACTOR_FLOW = 500 //Achtung, tatsächlicher Meß-Wert ist um Faktor 2 größer ->PediatrieSensor, Achtung acuLink noch nicht angepasst -> ALINK_FACTOR_FLOW
	- ViewSubState CO2 + SpO2 changed
	- AccuPIC: ChecksumBit in ServiceAccuboard included
	- Ppsv configurable: delta PEEP/absolute value
	- ConfigVersion changed to 2008 
	- Language file version 102 
	- HF-Amplitude to 120mbar with registry key "HKCU\\Software\\FabianHFO\\WorkState"-> "HFamplitude": 0==80, 1==100, 2=120, Pmean 1/2
	- MesData_C20C_Max	increased to 999
	- Automatic calculation of SpO2 alarm limits:
		AUTOALARMLIMIT_SPO2MAX	150			SPO2 max: 50% above measured SPO2 
		AUTOALARMLIMIT_SPO2MIN	50			SPO2 min: 50% below measured SPO2
		AUTOALARMLIMIT_PULSERATEMAX	150		PULSERATE max: 50% above measured PulseRate 
		AUTOALARMLIMIT_PULSERATEMIN	50		PULSERATE min: 50% below measured PulseRate
		AUTOALARMLIMIT_SPO2_PIMIN	50		PERFUSIONINDEX min: 50% below measured PI
	- regarding AutoIFlow: following parameter will be displayed: 
		I-Flow/E-Flow:  I-Flow parameter, E-Flow parameter
		Ramp:  only Plateau parameter
		AutoIFlow: no I-Flow parameter, no E-Flow parameter, no Plateau parameter
	- Range-ERROR of SPI will only be displayed in Logfile once
	- in case of new date an entry "DATECHANGE" is written to logfile with new date
	- Therapy-Mode: Preset some other mode and back to Therapy is now possible
	- ViewParaBtn->case VM_SIMVPSV: if(m_pcPara_TRIGGER) included
	- !!!!!!!!!!!Lungrecruitment disabled: -> //TODO_LUNREC
	- Terminal Interface Version 1.0
 @endverbatim
 *
 * @subsection ver30002 Version 3.0.0.2
 * @date 2013-06-03
 * @verbatim
	(Ctrl 3.3.08(ohne VGLimit) oder 3.3.09(mit VGLimit) / Mon 2.9.2(ohne VGLimit) bzw 3.9.2(ohne VGLimit) oder 2.9.3(mit VGLimit) bzw 3.9.3(mit VGLimit) / HFO 2.0.1 / Blender 3.2)
	- PDMS:serial with wave data prepared (ACL_RS232_WAVE)
	- Flow from SPI: value must be multiplied with factor 2 -> pediatric sensor!!! will be realised for the diagramms with the scaling factor, the flow value of the service menu is multiplied with 2
	- SpO2 wave implemented
	- SpO2 Alarms implemented: AL_Sens_SPO2_MODULE_NOTCONNECTED, AL_Sens_SPO2_SENSORFAULTY, AL_Sens_SPO2_CHECKSENSOR
	- WM_MOUSEMOVE will be forwarded in the message queue -> CBitmapSlider
	- Blender Error: code error displayed as decimal
	- change of range: Send_MODE_OPTION1() will be sent
 @endverbatim
 *
 * @subsection ver30001 Version 3.0.0.1
 * @date 2013-04-26
 * @verbatim
	 (Ctrl 3.3.08(ohne VGLimit) oder 3.3.09(mit VGLimit) / Mon 2.9.2(ohne VGLimit) bzw 3.9.2(ohne VGLimit) oder 2.9.3(mit VGLimit) bzw 3.9.3(mit VGLimit) / HFO 2.0.1 / Blender 3.2)
	 - Dellaca-Terminal implementiert
	 - Graphik: Skalierung Flow auf 80l/min erhöht
	 - Graphik: Skalierung HFO VOlumen auf 80ml erhöht
	 - Cursor manual scaling of graphs in therapy mode will be displayed now
	 - MAXRANGE_PEDIATRIC_THERAPYFLOW auf 19lpm begrenzt
 @endverbatim
 *
 * @subsection ver30000 Version 3.0.0.0
 * @date 2013-04-15
 * @verbatim
	(Ctrl 3.3.05 / Mon 2.9.2 oder 3.9.2 / HFO 2.0.1 / Blender 3.2)
	- Terminal-Funktionalität implementiert 
	- MAXRANGE_PEDIATRIC_THERAPYFLOW bei 20l/min begrenzt
	- Config-Version wird in das Systemlog eingetragen (Kompatibilität für Hospital-Settings)
	- Service-Menü: P-Mix -> P-insp umbenannt
	- ParaBtn Abbruchkriterium in den Schnelleinstellungen nur bei den PSV-Modi
	- FlowParaBtn HFO umbenannt von "auto" nach "default"
	- Languagefiles 1.0.0.99 nötig
	- im HFO wird bei FREEZE nur 1 ATZ-Loop gezeichnet
 @endverbatim
 *
 * @subsection ver20037 Version 2.0.0.37
 * @date 2013-04-10
 * @verbatim
	(Ctrl 3.3.03 / Mon 2.9.1 oder 3.9.1 / HFO 2.0.1 / Blender 2.6)
	- Operating time: "Operating time HFO module" wird in der Caption dargestellt 
 @endverbatim
 *
 * @subsection ver20036 Version 2.0.0.36
 * @date 2013-04-08
 * @verbatim
	(Ctrl 3.3.03 / Mon 2.9.1 oder 3.9.1 / HFO 2.0.1 / Blender 2.6)
	- Operating time lässt sich im ServiceMenü einstellen
	- AcuLink wird explizit beendet
 @endverbatim
 *
 * @subsection ver20035 Version 2.0.0.35
 * @date 2013-03-28
 * @verbatim
	(Ctrl 3.3.03 / Mon 2.9.1 oder 3.9.1 / HFO 2.0.1 / Blender 2.6)
	- CInterfaceSerial::CheckSerialData geändert bezüglich ERROR-Code
	- Mischer Checksum fail-> Mischdefekt Alarm           28.3.13
		'g_bMischAlarm'
		bei Ausfall HFO-Modul:'g_b_HFO_HW_ok' sicherer Zustand Druck
		FILTER_UEB_HFO_E (35)-> 90 ms Prox-Filt bei HFO
		sonst sporadisch Entlüftg.bei Start Man_ATZ
		Ctl_3_3_03 CS= 9F0B  MPLAB= 4e2a (combi)
		Fct_3_3_03 CS= C0CA  MPLAB= 71a7 (combi)
 @endverbatim
 *
 * @subsection ver20034 Version 2.0.0.34
 * @date 2013-03-21
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.9.0 oder 3.9.0 / HFO 2.0.1 / Blender 2.6)
	- CInterfaceSerial::CheckSerialData geändert:
	- es wird alle 2 Sekunden ein Wert abgefragt
	- stimmt der Wert nicht überein, wird dieser noch 2x mal geprüft
	- stimmt der Wert immer noch nicht, werden alle Vent-Settings nochmal geschickt
	- stimmt der Wert nicht überein, wird dieser noch 1x mal geprüft
	- stimmt der Wert immer noch nicht, wird der entsprechende ERROR-Code gesetzt und alarmiert
	- die Schnittstelle wird darauf hin alle 2 Sekunden erneut überprüft (gem. )
	- folgende ERROR-Code bezüglich Wertüberprüfung existieren:
		ERRC_COM_CHECK_VENTMODE:	//"Oq"
		ERRC_COM_CHECK_ITIME:		//"Wq"
		ERRC_COM_CHECK_ETIME:		//"Eq"
		ERRC_COM_CHECK_IFLOW:		//"Uq"
		ERRC_COM_CHECK_EFLOW:		//"Aq"
		ERRC_COM_CHECK_PINSP:		//"Tq"
		ERRC_COM_CHECK_PEEP:		//"Zq"
		ERRC_COM_CHECK_O2:			//"Dq"
		ERRC_COM_CHECK_MODEOPTION: //"Rq"
		ERRC_COM_CHECK_PPSV:		//"Cq"
		ERRC_COM_CHECK_HFFREQ:		//"Fq"
		ERRC_COM_CHECK_HFPMEAN:	//"Jq"
		ERRC_COM_CHECK_HFAMP:		//"Mq" 
	- Therapy in CInterfaceSerial::CheckSerialData eingefügt
 @endverbatim
 *
 * @subsection ver20033 Version 2.0.0.33
 * @date 2013-03-20
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.15 oder 3.8.15 / HFO 2.0.1 / Blender 2.6)
	- Flowsensor und OxySensor haben jetzt unterschiedliche Adresse für Kommandos
 @endverbatim
 *
 * @subsection ver20032 Version 2.0.0.32
 * @date 2013-03-20
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.14 oder 3.8.14 / HFO 2.0.1 / Blender 2.6)
	- Demo-Version Graz
 @endverbatim
 *
 * @subsection ver20031 Version 2.0.0.31
 * @date 2013-03-19
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.14 oder 3.8.14 / HFO 2.0.1 / Blender 2.6)
	- ALarm PIF (ParallelInterface) löst WD0 aus
 @endverbatim
 *
 * @subsection ver20030 Version 2.0.0.30
 * @date 2013-03-15
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.14 oder 3.8.14 / HFO 2.0.1 / Blender 2.6)
	- Version jetzt 3stellig <major><minor><patch>
	- CO2 disabled
	- Lungrekruitment disabled
 @endverbatim
 *
 * @subsection ver20029 Version 2.0.0.29
 * @date 2013-03-14
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.14 oder 3.8.14 / HFO 2.0.1 / Blender 2.6)
	- neuen Kernel (NK11MIM_UARTx_130313) implementiert -> KernelVersion in Systeminfo + Systemlog: KernelVersion + SpezialVersion (wenn Eintrag in Registry vorhanden), sonst wie bisher Datum des Kernel
	- 2er von etCO2 und SPO2 im Graphikmenü tiefgestellt, Text wird nicht von Language-Files bezogen!
	- im ServiceMenü System wird die Buildversion angezeigt
 @endverbatim
 *
 * @subsection ver20028 Version 2.0.0.28
 * @date 2013-03-07
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.14 oder 3.8.14 / HFO 2.0.1 / Blender 2.6)
	- neue MonitorPIC Version Mon 2.8.14 oder 3.8.14
 @endverbatim
 *
 * @subsection ver20027 Version 2.0.0.27
 * @date 2013-03-07
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.13 oder 3.8.13 / HFO 2.0.1 / Blender 2.6)
	- I2CWatchdog-Thread überprüft verfügbaren Systemspeicher, wenn unter 1000k fällt -> Alarm OUTOFMEMORY
 @endverbatim
 *
 * @subsection ver20026 Version 2.0.0.26
 * @date 2013-03-05
 * @verbatim
	(Ctrl 3.3.02 / Mon 2.8.13 oder 3.8.13 / HFO 2.0.1 / Blender 2.6)
	- wenn ParaBtn mit UDP grün geschalten wird und der UDP weiterhin gedrückt wird, wird nach 3 Sekunden der Screenlock aktiv und der ParaBtn wird inaktiv
	- Pfeil bei I-Flow wird bezüglich Vgaranty und PinspNotReached überprüft
	- Einträge ins LogFile: Kernelversion, SerialTreiber, acuLink
	- *.dl_ Dateien von der Installation werden gelöscht
 @endverbatim
 *
 * @subsection ver20025 Version 2.0.0.25
 * @date 2013-02-28
 * @verbatim
	(Ctrl Ctrl 3.3.1 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- tsAlarmTime aus CAlarm entfernt (Performance)
	- wird der AV Abgleich ausgeführt und Flowsensor war ausgeschalten (DUOPAP/NCPAP), wird dieser eingeschalten
	- PSV + VG: Volumen nicht erreicht -> bei Apnoebeatmung aktiv wird Pfeil in Pbackup angezeigt, sonst bei Ppsv
	- Einstellung Pmin Alarmdelay -> Setup: Text ist jetzt mittig
 @endverbatim
 *
 * @subsection ver20024 Version 2.0.0.24
 * @date 2013-02-18
 * @verbatim
	(Ctrl Ctrl 3.3.1 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- StartupDelay 10sec
	- beim Einschalten von VG wird der Auto-Wert je nach Bereich gerundet
	- bei "Apnoebeatmung aktiv" wird die rote Linie VG nicht gezeichnet
	- Alarmhandler: Membervariable m_pTempAlarmClass gelöscht
	- CDataHandler::checkLimits() wird nur ausgeführt, wenn 
		getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SYSTEM_SILENT
		&&	getModel()->getALARMHANDLER()->getAlarmSilentState()!=ASTATE_SILENT
		&&	getModel()->getVIEWHANDLER()->getViewState()!=VS_STARTUP
	- Code mit cppCheck überprüft und bereinigt
	- Version für Customer besteht nur noch aus <major release>.<minor release>
	- Lizenzdatei wird auf Schreibschutz überprüft und wenn vorhanden beseitigt
	- VG: wenn VG wieder automatisch eingeschalten wird, wird der max erlaubte Pinsp wieder auf alten Wert gesetzt
 @endverbatim
 *
 * @subsection ver20023 Version 2.0.0.23
 * @date 2013-02-09
 * @verbatim
	(Ctrl Ctrl 3.3.1 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- HRESULT res= entfernt aus allen HRESULT res=CloseHandle(..)
	- #define SIMULATION_ENTREK implementiert
	- CViewDTBField::DrawLine(...): ohne try/catch, ist bereits in der Funktion
	- bei Startup CDataHandler::PatientDataAvailable() ... und bei Änderung....wird sonst laufend durch ViewDTBField aufgerufen -> Zugriff auf Registry....
 @endverbatim
 *
 * @subsection ver20022 Version 2.0.0.22
 * @date 2013-02-07
 * @verbatim
	(Ctrl 3.3.1 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- AutoIFlow deaktiviert
 @endverbatim
 * 
 * @subsection ver20021 Version 2.0.0.21
 * @date 2013-02-05
 * @verbatim
	(Ctrl 3.3.1 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- Graphik-Menü zurück gebaut, SPO2 ins Menü eingebaut
 @endverbatim
 * 
 * @subsection ver20020 Version 2.0.0.20
 * @date 2013-02-04
 * @verbatim
	(Ctrl 3.3.1 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- Plateau als T_ramp umgerechnet und an PIC gesendet (T_rampe =  (T_insp * (100 - Plateau)) / 100)
 @endverbatim
 * 
 * @subsection ver20019 Version 2.0.0.19
 * @date 2013-01-30
 * @verbatim
	(Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- AutoIFlow aktiviert
	- Datum wird bei Start ins Log geschrieben
 @endverbatim
 * 
 * @subsection ver20018 Version 2.0.0.18
 * @date 2013-01-22
 * @verbatim
	(Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- Alarm Flat battery löscht vorherige Alarme AL_Accu_60, AL_Accu_30, AL_Accu_15
 @endverbatim
 * 
 * @subsection ver20017 Version 2.0.0.17
 * @date 2013-01-11
 * @verbatim
	(Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
    - Serial CheckThread alle 1000 ms
 @endverbatim
 * 
 * @subsection ver20016 Version 2.0.0.16
 * @date 2013-01-07
 * @verbatim
    (Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
    - Version-Datum des Kernel wird angezeigt (PDMS-Version: 1.27 Oct 19 2012)
 @endverbatim
 *
 * @subsection ver20015 Version 2.0.0.15
 * @date 2012-12-29
 * @verbatim
    (Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
    - PSV: bei automatischem VG-off wird P-Backup richtig gesetzt
 @endverbatim
 *
 * @subsection ver20014 Version 2.0.0.14
 * @date 2012-12-26
 * @verbatim
	(Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- Pmean lässt sich auch wieder dekrementieren
 @endverbatim
 *
 * @subsection ver20013 Version 2.0.0.13
 * @date 2012-12-21
 * @verbatim
	(Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	BUG!!!!!! NICHT VERWENDEN !!!!!
	- P-PSV erhöht/verringert sich automatisch, wenn PEEP erhöht/verringert wird
	- Pmean + PmeanRec bei Lungenrekruitment gem PEEP/P-PSV implementiert
 @endverbatim
 *
 * @subsection ver20012 Version 2.0.0.12
 * @date 2012-12-17
 * @verbatim
	(Ctrl 3.3.01 / Mon 2.8.12 oder 3.8.12 / HFO 2.0.1 / Blender 2.6)
	- Alarm "Volumen nicht erreicht": je nach Ursache wird entweder bei Pinsp/Ppsv oder I-Flow der Pfeil nach oben gesetzt
	- PSV-Modus: "P insp" heisst jetzt "P Backup"
	- Mischeralarm -> Bit wird ins Log geschrieben
 @endverbatim
 *
 * @subsection ver20011 Version 2.0.0.11
 * @date 2012-12-10
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.10 oder 3.8.10 / HFO 2.0.1 / Blender 2.6)
	- Verriegelung P-Insp / PEEP bei PSV angepasst
	- VT wird generell je nach Einstellung Service BTB oder Average angezeigt
	- Flowsensor ausgeschalten: Numeriks werden richtig angezeigt (CPAP), "Backup aktiv" wird deaktiviert
	- Therapie-Flow: einstellbar 0, 1 bis 5 l/min(0,2er Schritten), über 5l/min in 1er Schritten
	- Alarm AL_SysLimit_Vgaranty -> im PSV wird Pfeil bei ParaBtn P-PSV angezeigt
	- Licensing: wird in Eingabefenster für jeweilige Lizenznummer "Cancel" gedrückt, wird die Lizens nicht mehr gelöscht
	- P-Mean bei HFO: ist Lungenrekruitment deaktiviert, wird P-Mean-rec nicht nachgezogen
	- Pmean-Rec wird bezüglich Pmean überprüpft (HFO)
 @endverbatim
 *
 * @subsection ver20010 Version 2.0.0.10
 * @date 2012-12-07
 * @verbatim
	 (Ctrl 3.2.7 / Mon 2.8.8 oder 3.8.8 / HFO 2.0.1 / Blender 2.6)
	- Servicemenü: Satz „connect test lung“ durch „close Y-Piece at ET tube connector“ ersetzt
	- Voulmengarantie: bei PSV-Mode wird Wert der rote Linie vom P-PSV-Para genommen, sonst vom P-Insp
 @endverbatim
 *
 * @subsection ver20009 Version 2.0.0.9
 * @date 2012-11-05
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.8 oder 3.8.8 / HFO 2.0.1 / Blender 2.6)
	- Interface Terminal nach AcuLink umbenannt
	- Bug beim Startup I:E-Verhältnis beseitigt
	- Projekt mit CppCheck Version 1.57 überprüft und bereinigt
	- Mischer alarm erweiter: SelfTest
	- Alarm AV-Abgleich implementiert
	- Language file 1.0.0.92 nötig
	- CO2-Lizenzdatei über Servicemenü komplett entfernt
	- CO2-Umschaltung geändert -> kein Absturz mehr
 @endverbatim
 *
 * @subsection ver20008 Version 2.0.0.8
 * @date 2012-11-02
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.8 oder 3.8.8 / HFO 2.0.1 / Blender 2.6)
	- Volumengarantie bis 1,2 ml Minimum
 @endverbatim
 *
 * @subsection ver20007 Version 2.0.0.7
 * @date 2012-10-30
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Startup-Delay von 2 Minuten auch in NMODE
 @endverbatim
 *
 * @subsection ver20006 Version 2.0.0.6
 * @date 2012-10-26
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- O2-Flush-Button bleibt aktiv auch bei mehrmaligem on/off
 @endverbatim
 *
 * @subsection ver20005 Version 2.0.0.5
 * @date 2012-10-24
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Ausatemventilkalibrierung: I:E wird richtig gesetzt
 @endverbatim
 *
 * @subsection ver20004 Version 2.0.0.4
 * @date 2012-10-23
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- "Apnoebeatmung aktiv" und "Backup aktiv" wird generell bei Moduswechsel gelöscht
	- Controller-PIC-Version wird im Log richtig eingetragen
 @endverbatim
 *
 * @subsection ver20003 Version 2.0.0.3
 * @date 2012-10-18
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Infoanzeige: bei ein-/ausschalten VG wird Infoanzeige neu gezeichnet -> "IPPV + VG"
	- Überprüfung gegenüber alter SW + Ppsv ü.PEEP -> Ppsv >= PEEP+2mbar
 @endverbatim
 *
 * @subsection ver20002 Version 2.0.0.2
 * @date 2012-10-17
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Kalibrierung Ausatemventil gefixt
 @endverbatim
 *
 * @subsection ver20001 Version 2.0.0.1
 * @date 2012-10-17
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Limits von Pinsp wird bei VG-Decrease jetzt richtig gesetzt
 @endverbatim
 *
 * @subsection ver20000 Version 2.0.0.0
 * @date 2012-10-15
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	 - Release-Version
 @endverbatim
 *
 * @subsection ver19999 Version 2.0.0.0beta83
 * @date 2012-10-11
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Alarmdelay bei NMODE (BREATH_ALARMDELAY) von 400 auf 3000 ms gesetzt
	- Alarmlimits: NCPAP, CPAP und DUOPAP -> Druck bis max 40mbar
	 - Nebulizer deaktiviert
 @endverbatim
 *
 * @subsection ver19998 Version 2.0.0.0beta82
 * @date 2012-10-08
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- bei VG: decrease/increase von Pinsp geändert
	- Werte werden richtig in die Config übernommen
	- PDMS-VueLink: Sprache wird gesetzt
	- Ppsv immer mindestens 2mbar über PEEP und immer kleiner oder gleich dem Pinsp sein
	- HF-Amplitude über Registry konfigurierbar
	- Lungenrecruitment disabled -> Flag: TODO_LUNREC
 @endverbatim
 *
 * @subsection ver19997 Version 2.0.0.0beta81
 * @date 2012-10-08
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Wird VG on/off nicht bestätigt, wird P-Insp wieder auf den alten Wert gesetzt
	- Netzwerk: Anzeige geändert, Port hinzugefügt
	- Port mit Installer konfigurierbar
 @endverbatim
 *
 * @subsection ver19996 Version 2.0.0.0beta80
 * @date 2012-10-02
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- P-PSV als Absolutwert (nicht mehr "über PEEP") -> BENÖTIGT NEUE PIC VERSIONEN!!!!!!!!!!!!!!
	- PSV-Modus: Anzeige "Apnoebeatmung aktiv" implementiert -> Languagefile 1.0.0.89 nötig!!!!
	- Port für PDMS->Ethernet wird eingelesen und gesetzt
	- Port und PDMS-Protokoll werden als Hospitalsettings geschrieben und eingelesen
 @endverbatim
 *
 * @subsection ver19995 Version 2.0.0.0beta79
 * @date 2012-10-01
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Apnoe-Bereich einstellbar von 2 bis 20 sec + off
	- MV-Bereich Neo einstellbar bis 7 lpm
 @endverbatim
 *
 * @subsection ver19994 Version 2.0.0.0beta78
 * @date 2012-10-01
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Logo logoFabianHFO.bmp jetzt auch in der Shutdown-View
 @endverbatim
 *
 * @subsection ver19993 Version 2.0.0.0beta77
 * @date 2012-09-27
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Range Abbruchkriterium in den Para-Settings geht jetzt auch von 5 bis 70
 @endverbatim
 *
 * @subsection ver19992 Version 2.0.0.0 beta76
 * @date 2012-09-27
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- IPPV + SIPPV: Alarmlimit State Apnoe wird auf OFF gesetzt (zur Sicherheit für die PICs)
	- V-Ti wird BTB angezeigt
	- Range Abbruchkriterium in den Para-Settings geht jetzt auch von 5 bis 70
	- Logo logoFabianHFO.bmp jetzt auch in der Shutdown-View
	- Apnoe-Bereich einstellbar von 2 bis 20 sec + off
	- MV-Bereich Neo einstellbar bis 7 lpm
	- P-PSV als Absolutwert (nicht mehr "über PEEP") -> BENÖTIGT NEUE PIC VERSIONEN!!!!!!!!!!!!!!
	- PSV-Modus: Anzeige "Apnoebeatmung aktiv" implementiert -> Languagefile 1.0.0.89 nötig!!!!
	- Wird VG on/off nicht bestätigt, wird P-Insp wieder auf den alten Wert gesetzt
	- bei eingeschaltener VG (noch nicht bestätigt) und Focusverlust wird Pinsp auf richtigen Wert zurückgesetzt
	- Änderungen aus FabianHFO 2.0.0.0 übernommen
	- Limits von Pinsp wird bei VG-Decrease jetzt richtig gesetzt
	- Kalibrierung Ausatemventil gefixt
	- Infoanzeige: bei ein-/ausschalten VG wird Infoanzeige neu gezeichnet -> "IPPV + VG"
	- Überprüfung gegenüber alter SW + Ppsv ü.PEEP -> Ppsv >= PEEP+2mbar
	- "Apnoebeatmung aktiv" und "Backup aktiv" wird generell bei Moduswechsel gelöscht
	- Controller-PIC-Version wird im Log richtig eingetragen
	- GetParent() wird überprüft
	- Ausatemventilkalibrierung: I:E wird richtig gesetzt
	- Servicemenü: Satz „connect test lung“ durch „close Y-Piece at ET tube connector“ ersetzt
	- O2-Flush-Button bleibt aktiv auch bei mehrmaligem on/off
	- View-Menü implementiert
	- Trend-Menü an View-Menü angepasst
	- Graph-Menü angepasst (CO2 etc)
	- CO2-View MicroPod angepasst
	- Startup-Delay von 2 Minuten auch in NMODE
	- Interface Terminal nach AcuLink umbenannt
	 - Volumengarantie bis 1,2 ml Minimum
	 - Bug beim Startup I:E-Verhältnis beseitigt
	- IPPV + SIPPV: Alarmlimit State Apnoe wird auf OFF gesetzt (zur Sicherheit für die PICs)
	- V-Ti wird BTB angezeigt
 @endverbatim
 *
 * @subsection ver19991 Version 2.0.0.0beta75
 * @date 2012-09-25
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Apnoe-Alarm aus IPPV und SIPPV entfernt
	- in der Wahl manuelle Skalierung der Diagramm: die Zeitskalierung lässt sich nicht mehr mit dem Encoder, sondern nur noch über die Cursor Tastatur skalieren
	- wenn im Schleifenbildschirm „Freeze“ betätigt wird, wird kein Numerikblock mit den gespeicherten Meßwerten mehr angezeigt
	- Languagefile 1.0.0.88 verwenden
 @endverbatim
 *
 * @subsection ver19989 Version 2.0.0.0beta74
 * @date 2012-09-21
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Reinitialisierung der seriellen Schnittstelle neu implementiert
	- Button-Klicks werden geloggt
 @endverbatim
 *
 * @subsection ver19988 Version 2.0.0.0beta73 
 * @date 2012-09-10
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Therapy: Flowmeter an Range (PEDIATRIC) angepasst
	- Datum der letzten Flow-Kalibrierung wird ins Log gechrieben
	- Sprachenänderung neu implementiert
	- Logo kann ersetzt werden: logoFabianHFO.bmp -> Verzeichnis FFSDISK\\logoFabianHFO.bmp, wenn SW dies Datei vorfindet, wird dieses Bitmap als Logo verwendet, ansonst defaultmäßig das Acutronic-Logo
 @endverbatim
 *
 * @subsection ver19987 Version 2.0.0.0beta72
 * @date 2012-09-10
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Alarmlimitprüfung PminHigh bei PRESET -> wird jetzt vom aktiven Modus genommen
	- beim STARTUP gilt ein Alarmdelay von 10 Sekunden, unabhängig von dem eingestellten Alarmdelay
	- Text "autoset alarm limits canceled" gelöscht
	- bei Aufhebung Schlauchalarm wird Alarmdelay aktiviert
	- Alarm "Volumengarantie hoch" entfernt
	- im Falle eines COM-Errors wird die serielle Schnittstelle erst noch neu initialisiert, wenn kein Erfolg -> Alarm
	- wenn O2-Flush aktiv ist und O2-Para geändert wird, wird der Wert nicht an PICs gesendet, O2-Para wird disabled
 @endverbatim
 *
 * @subsection ver19986 Version 2.0.0.0beta71
 * @date 2012-08-10
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Lizensierung CO2 entfernt, einstellbar im CO2-Menü (disabled,MicroPod,Capnostat)
 @endverbatim
 *
 * @subsection ver19985 Version 2.0.0.0beta70
 * @date 2012-08-08
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- CriticalSection der Trends geändert
 @endverbatim
 *
 * @subsection ver19984 Version 2.0.0.0beta69
 * @date 2012-08-08
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- PDMS mit Ethernet implementiert (noch nicht aktiv!)
 @endverbatim
 *
 * @subsection ver19983 Version 2.0.0.0beta68
 * @date 2012-08-01
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Datum Logfile wird zyklisch überprüft
	- Watchdog WD_PERIODE 20000
 @endverbatim
 *
 * @subsection ver19982 Version 2.0.0.0beta67
 * @date 2012-07-27
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Alarmdelay einstellbar im Menü->Ventilation
 @endverbatim
 *
 * @subsection ver19981 Version 2.0.0.0beta66
 * @date 2012-07-27
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Therapie -> FLow: unterster Wert = 0lpm, von 0 bis 1lpm in 0,1er Schritte einstellbar
	- Rücksetzen der Patientenalarme unterliegt nicht dem 20sec Alarmdelay
 @endverbatim
 *
 * @subsection ver19980 Version 2.0.0.0beta65
 * @date 2012-07-26
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- CalculateAutoAlarmlimitVgaranty + CalculateAutoAlarmlimitVlimit: bei VM_SIMV und VM_SIMVPSV wird Wert von getAVGMessureDataTVEresp genommen, sonst getAVGMessureDataTVE
	- Service-Menü->MonitorPIC: Reihenfolge der ADC,OFFS,SCL-Werte geändert
	- Trigger-Bereich auf 1-10 verringert
	- Autoset Alarmlimits: ist der entsprechende Wert ein Alarm, wird dieser gelöscht
	- Btn Autoset Alarmlimits: mit Graphik
	- Sprachen-Menü: beim Laden wird der Focus auf die eingestellte Sprache gesetzt und diese angezeigt
 @endverbatim
 *
 * @subsection ver19979 Version 2.0.0.0beta64
 * @date 2012-07-25
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- O2-Flush: keine Anzeige mehr "gestartet/gestoppt"
	- bei Autoset (Alarmlimits) kein Autosilent 15sec mehr
 @endverbatim
 *
 * @subsection ver19978 Version 2.0.0.0beta63
 * @date 2012-07-24
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- kein AUTOSET ALL mehr bei den Alarmlimits
	- Frequenz bei SIMV / SIMV+PSV automatisch bei 100 begrenzt (durch Begrenzung E-Time 0,5 sec)
 @endverbatim
 *
 * @subsection ver19977 Version 2.0.0.0beta62
 * @date 2012-07-24
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- NMODE auch im Pediatrie-Modus (sofern NMODE lizensiert)
	- Autosilent bei TubeOcclusion im HFO entfernt
	- Flowsensor lässt sich auch mit Alarm "Flowsensor reinigen" kalibrieren
	- bei Flowsensorkalibrierung werden alte Flowsennsoralrme gelöscht
 @endverbatim
 *
 * @subsection ver19976 Version 2.0.0.0beta61
 * @date 2012-07-23
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Apnoe-Alarm PSV frei geschalten
 @endverbatim
 *
 * @subsection ver19975 Version 2.0.0.0beta60
 * @date 2012-07-23
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- BPM und E-Time bei SIMV/SIMV+PSV begrenzt: 
		BPM bei 150bpm
		E-Time bei 0,5sec
	- Autosilent aus ff Alarme entfernt:
		AL_SysAl_P_IN_O2
		AL_SysAl_P_IN_AIR
		AL_SysAl_TUBE_OCCLUSION
	- werden Alarmlits ausgeschalten, werden entsprechende Alarme gelöscht (AL_PatAl_Apnoe, AL_PatAl_Leakmax, AL_PatAl_BPMmax, AL_PatAl_DCO2max, AL_PatAl_DCO2min, AL_PatAl_ETCO2max, AL_PatAl_ETCO2min)
	- ist VG oder VL aktiv, wird zusätzlich zum Modus "+VG" oder "+VL" in der Infozeile angezeigt
	- untere Grenze CPAP-Parameter im NMODE jetzt 2 mbar
 @endverbatim
 *
 * @subsection ver19974 Version 2.0.0.0beta59
 * @date 2012-07-19
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Liste des Besprechnungsprotokolls vom 16.07.2012 durchgearbeitet:
		Punkt 3/9: ist die VG aktiv und der Flowsensor wird ausgeschaltet, geht der Pinsp auf den letzten gültigen Wert, der während der VG verwendet wurde, ist kein gültiger Wert vorhanden auf den vom Anwender zuvor eingestellten Wert
		Punkt 4: kein Autosilent nach "Disconnection" und "Tubus blockiert"
		Punkt 5: bei Dekonnektion wird die Autoskalierung ausser Kraft gesetzt
		Punkt 10: beim Einschalten von VLimit dird die Kurve nicht mehr gewechselt
		Punkt 11: Pfeil bei Pinsp bei Alarm "Tidalvolumen zu hoch" weg, Text von "Volumen nicht erreicht" nach "Tidalvolumen nicht erreicht" geändert, 
		Punkt 13: Backup-Btn ohne 1/min
		Punkt 15: PSV Abbruchkriterium von 5-70 einstellbar
		Punkt 16: wird der Flowsensor ausgeschaltet, werden MV-Alarme gelöscht
	- Sprachenfiles 1.0.0.85 nötig
 @endverbatim
 *
 * @subsection ver19973 Version 2.0.0.0beta58
 * @date 2012-07-12
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- 2 graue Glocke + Alarmtext bei signalisiertem Alarme in Infozeile
	- kein Text "signalisierte Alarme gelöscht"
	- letzter signalisierter Alarm bleibt in Infozeile stehen bis RESET
	- wenn signalisierte Alarme mit der Tatse "Alarmsilent" gelöscht werden, wird kein "Send_ALARMKEY_PUSH" an SPI gesendet
	- wenn Alarmsilent aktiv geschalten wird, wird ein Delay von 20 Sekunden für alle Patientenalarme gesetzt
	- CPAP: wenn BackupParaBtn auf aus geschalten wird, wird aktiver Backup gelöscht
	- Alarmlimits: Reset-Btn gelöscht, Rest nur noch über Alarmsilent-Taste
	- Menüführung Setup/Service geändert
	- Backup-Akustik-Meldung wählbar in Ventilation Settings
	- erledigt: HFO + VG + Autoscale: rote Linie in Druckkurve rutscht mitunter aus dem Bild oben oder unten hinaus.
	- erledigt: HFO + VG:  bei "Volumen nicht erreicht" -> Pfeil aufwärts bei Amplitude setzen
	- erledigt: Autoscale: X-Achse manuell verstellt: -> nach Alarmbildschirm Rückkehr immer in "10 sek" Einstellung (gewählte Einstellung geht verloren)
	- erledigt: die Alarmgeschichte wieder so wie zu Beginn gelöst werden, d.h. wenn der Alarm stumm geschalten ist, dann ist er stumm, egal was kommt. 
		Der Alarm soll aber visuell dargestellt werden. So ist es bei der ganzen fabian-Familie und Mitbewerbern bisher umgesetzt worden. 
		Gemäss Norm entspricht das einem „Audio AUS“. Da die Stummschaltung bei uns zeitlich begrenzt ist muss die Glocke oben rechts der Richtigkeit halber 
		gestrichelte gekreuzte Linien und keine durchgezogenen haben. Im Service Modus muss die Glocke durchgezogene Linien haben, da nicht zeitlich begrenzt.
 @endverbatim
 *
 * @subsection ver19972 Version 2.0.0.0beta57
 * @date 2012-07-10
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Während autoset Alarmlimts läuft und Modus gewechselt wird, wird autoset abgebrochen
	- Languagefiles 1.0.0.84 nötig
	- graue Alarmglocke nicht mehr in Infozeile, nur noch in Numerikzeile
	- Alarm "High Tidal Volume" der Volumengarantie implementiert
	- Autosilent 30sec nur, wenn Wert geändert wurde
	- Alarme Inspirationsschlauch, Exspirationsschlauch, ProximalPressure sind durch Alarm Tobe Occlusion ersetzt
 @endverbatim
 *
 * @subsection ver19971 Version 2.0.0.0beta56
 * @date 2012-07-09
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- wenn ALarme signalisiert sind (graue Glocke) können diese mit der gelben Alarmstumm-Taste gelöscht (reset) werden
	- Languagefiles 1.0.0.82 nötig
 @endverbatim
 *
 * @subsection ver19970 Version 2.0.0.0beta55
 * @date 2012-07-07
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- VG geändert: Während der Beatmung wird ständig der aktuelle und gültige Pins mitgeloggt (mit jedem Atemzug). Der Wert wird als gültig angesehen, wenn der Wert folgende Kriterien erfüllt:
	- bei eingeschalteter VG: wenn das gemessene Vte +-15% im Bereich der eingestellten VG liegt
	- bei ausgeschalteter VG und bei Preset: wenn der gemessene Pmax +-15% im Bereich des eingestellten Pinsp liegt
		Wird nun die VG eingeschaltet, erhöht sich der erlaubte Pins (Parameter) wie bisher um 5mbar, wird VG ausgeschaltet, wird nachgesehen, ob ein letzter gültiger Pins vorliegt, wenn ja, wird dieser verwendet, wenn nein, wird der eingestellte/max. erlaubte Pins um 5 mbar verringert.
		Kriterien, wenn Pinsp verringert wird: 
	- Pinsp >= PEEP+Ppsv: der Pinsp darf, wenn er verringert wird, nicht kleiner PEEP+Ppsv sein (bei SIMV+PSV). In diesem Fall (habe ich bereits mit Franz W geklärt), wird der Ppsv ebenfalls reduziert
	- Pinsp muss immer 2mbar größer als PEEP sein
	- SIMV+PSV+VG: bei automatischer Deaktivierung und Reaktivierung wird der ursprünglich P-PSV wider hergestellt (sofern Bereich passt)
	- es wird nur noch bei ff Alarmen die aktuelle View in die entsprechende Alarmview gewechselt:
		AL_Accu_Defect
		AL_Accu_Empty
		AL_SysFail_MIXER:
		AL_SysFail_P_PROXIMAL:
		AL_SysAl_P_IN_O2:
		AL_SysAl_P_IN_AIR:
		AL_SysAl_P_EXSPIRATIONTUBE:
		AL_SysAl_P_INSPIRATIONTUBE:
		AL_SysFail_ChecksumConPIC:
		AL_SysFail_ChecksumMonPIC:
		AL_SysFail_IF_SPI:
		AL_SysFail_IF_DIO:
		AL_SysFail_IF_COM:
		AL_SysFail_IF_I2C:
		AL_SysFail_IF_PIF:
		AL_SysFail_IF_ACULINK:
		AL_SysFail_RELAIS_DEFECT:
		AL_SysFail_P_IN_MIXER:
		AL_SysFail_OVERPRESSURE:
		AL_SysFail_VOLTAGE:
		AL_SysFail_OUTOFMEMORY:
		AL_SysFail_Fan:
 @endverbatim
 *
 * @subsection ver19969 Version 2.0.0.0beta54
 * @date 2012-07-04
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- HFO + VG:  bei "Volumen nicht erreicht" -> Pfeil aufwärts bei Amplitude
	- Autoscale: X-Achse gewählte Einstellung wird bei HFO sepaart gespeichert, auch bei Autoscale wird die letzte Einstellung geladen
	- HFO + VG + Autoscale:Skalierung richtet sich auch nach den roten Linien der VG
	- PSV: "Backup aktiv" aus der Infozeile entfernt
	- Alarmstummschaltung "Manual Silent": Alle Alarme sind für 2 Minuten stumm
	- Alarmglocke für Alarmstumm rot gestrichelt
	- Backup-Anzeige nur wenn FLowsensor on
	- Signalton bei Backup läsdst sich im Menü ausschalten
 @endverbatim
 *
 * @subsection ver19968 Version 2.0.0.0beta53
 * @date 2012-07-04
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- DUOPAP -> PEEP: Einheit jetzt sichtbar
	- CO2 Factory ALarmlimits: High 60 mmHg, Low 8 mmHg
	- Diverse Änderungen bei CO2-Implementierung MicroPod (Zeit einstellbar bis Pumpe automatisch eingeschaltet wird etc)
	- Languagefile Version 1.0.0.81 nötig
 @endverbatim
 *
 * @subsection ver19967 Version 2.0.0.0beta52
 * @date 2012-06-29
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Numerik angepasst
 @endverbatim
 *
 * @subsection ver19966 Version 2.0.0.0beta51
 * @date 2012-06-29
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Filter Volumengarntie entfernt
	- Menü-Struktur geändert (Return-Button etc)
	- VTe-Werte können entweder BTB oder AVG verwendet werden, Service-Menü->Settings->Use BTB for VTe
	- Languagefiles 1.0.0.79 nötig
	- O2 aus Numerik ausgebaut
	- Anzeige Tubeset aus Numerik ausgebaut
	- Numerik: Mechanikblöcke nur noch bei Loops
	- CPAP: "Backup aktiv" in der Infozeile geändert
 @endverbatim
 *
 * @subsection ver19965 Version 2.0.0.0beta50
 * @date 2012-06-27
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- CO2: bei ALarm werden die Messwerte auf "invalid" gesetzt
 @endverbatim
 *
 * @subsection ver19964 Version 2.0.0.0beta49
 * @date 2012-06-27
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- ALARMLIMITS werden geloggt
 @endverbatim
 *
 * @subsection ver19963 Version 2.0.0.0beta48
 * @date 2012-06-26
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- CO2:	Anzeige der verbleibenden Stunden bis zur Kalibrierung im CO2-Menü(nur Oridion)
			Anzeige, wenn Kalibrierung oder Service erforderlich ist ->Status CO2Sensor, wenn erforderlich dann rot (nur Oridion, Anzeigen-Bsp: Sensor state: ok (calibration required) )
			Anzeige des Datum der letzten Kalibrierung, ANzeige, wenn Systemzeit ungültig
			Factory-Werte: etCO2 high: 50mmHg, etCO2 low: 20mmHg
			Alarmlimit range: etCO2 high: 5 - 150mmHg, etCO2 low: 0 - 145mmHg
	- Volumengarantie geändert: bei SIMVPSV wird beim EInschalten der VG der VG-Wert vom TVEresp ermittelt
 @endverbatim
 *
 * @subsection ver19962 Version 2.0.0.0beta47
 * @date 2012-06-25
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Volumengarantie geändert:
		PSV+VG:			VG aktiv für PSV Atemhub, Taste I-Time + Pinsp wird orange, Taste P_PSV dird nicht angezeigt 
		im Apnoe-Fall wird mit Pinsp beatmet und an die VG herangetastet, Pinsp erhöht sich um 5mbar bei 
		zuschalten der VG, maximale Grenze Pinsp
	- CPAP: bei Backup wird "Backup aktiv" in der Infozeile angezeigt
	- Verriegelung Pinsp bei PSV+VG entfernt
	- Verriegelung PEEP bei PSV+VG entfernt
 @endverbatim
 *
 * @subsection ver19961 Version 2.0.0.0beta46
 * @date 2012-06-22
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Volumengarantie neu implementiert:
		SIMV+PSV+VG:	VG aktiv für SIMV Atemhub, PSV Atemhub nicht VG kontolliert. 
			I-Time + Pinsp Button wird orange, Pinsp erhöht sich um 5mbar bei 
			zuschalten der VG, bei nichgt erreichen von VG -> Pfeil nach oben 
			in der Pinsp-Taste
		PSV+VG:			VG aktiv für PSV Atemhub, Taste I-Time + Pinsp wird orange, 
			im Apnoe-Fall wird mit Pinsp beatmet und an die VG herangetastet, 
			maximale Grenze Pinsp
	- Button I-Time bei PSV und SIMV+PSV auch ohne VG orange
 @endverbatim
 *
 * @subsection ver19960 Version 2.0.0.0beta45
 * @date 2012-06-21
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Autoset Alarmlimit lässt sich deaktivieren
	- Button Factory Settings lässt sich ausblenden
 @endverbatim
 *
 * @subsection ver19959 Version 2.0.0.0beta44
 * @date 2012-06-18
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Therapie-Modus auch im Pediatriebereich
 @endverbatim
 *
 * @subsection ver19958 Version 2.0.0.0beta43
 * @date 2012-06-14
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- Referenzschleife wird jetzt gleich gespeichert
	- Alarmlimit Leak: on/off-Regelung geändert -> off -> 10 -> 50
	- Verzögerung Alarm "Druck tief" (AL_PatAl_PminLow) von 500ms auf 1000ms erhöht
	- Im NMODE wird bei Aufhebung Disconnectionsalarm kein Autosilent(30sec) gesetzt
 @endverbatim
 *
 * @subsection ver19957 Version 2.0.0.0beta42
 * @date 2012-06-11
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- TVE-Werte werden nur noch BTB angezeigt
 @endverbatim
 *
 * @subsection ver19956 Version 2.0.0.0beta41
 * @date 2012-05-31
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- SIMULATION_VERSION implementiert
 @endverbatim
 *
 * @subsection ver19955 Version 2.0.0.0beta40
 * @date 2012-05-24
 * @verbatim
	(Ctrl 3.2.7 / Mon 2.8.7 oder 3.8.7 / HFO 2.0.1 / Blender 2.6)
	- PDMS-Testversion für Holland
 @endverbatim
 *
 * @subsection ver19954 Version 2.0.0.0beta
 * @date 2011-12-15
 * @verbatim
 (Ctrl 3.0.x / Mon 2.7.2 oder 3.0.1 / HFO 2.0.1 / Blender 2.5)
	- Video implementiert (es muss künftig die Video-DLL im Verzeichnis FFSDISK der NetDCU liegen, auch NetDCU11!)
	- PDMS implementiert (acuLink)
	- Trends mit Lizenzschlüssel
	- AutoIFlow + Lungenrekruitment implementiert
	- Lungenrekruitment mit Lizenzschlüssel implementiert
	- Mainboard-Version wird unterstützt, wird bei Startup an SPI & COM geschickt,
		wird bei System-Info im Menü und im Service-Mode angezeigt
	- Serielle Schnittstellen sind an Mainboard-Version angepasst
	- Autoscale Pressure-Graoh an Pmean rec (Lungenrekruitment) angepasst
	- FlowTrigger: Einheit jetzt als Liter/Minute
	- Pmanual wir in HFO mit Lungenrecruitment ausgeblendet
	- minimale Differenz Pmean und Pmean rec beträgt jetzt 2mbar
	- CO2-Modul: ALarmlimits an Einheit angepasst
	- Send_PARAVAL_EXH_TIME(iETIME/1000);
	- Signaltöne SetPIFSound(PIF_SIGNAL) werden mit minimaler Lautstärke gesendet
	- Flowkalibrierung: DAC-Werte aus Anzeige entfernt
	- MAC-Adresse wir überprüft
	- Trends: Kommastelle bei COMPLIANCE, Vte, MV und Pmean
	- Nebulizer implementiert, Alarme: 
		AL_Nebulizer_Disconnection,			// 37,
		AL_Nebulizer_SysError,				// 38,
	- Languagefiles 1.0.0.73 nötig
	- I2C: delay bei I2C-Alarm von 15 sec, Überprüfung der CalibratioBits vom AccuStatus
	- CModelObserver	* pObserver wird auf NULL geprüft
	- I2C über CriticalSection abgesichert
	- SERIAL über CriticalSection abgesichert
	- etCO2: Capnostat -> GasCompensation wird vom aktuellen O2 Wert automatisch geschickt, kann nicht mehr eingestellt werden
	- CMVModel::SaveLastGuiltyBreathValues() nur wenn VENT_RUNNING==GetVentRunState()
	- Volumengarantie/-limit: ResetLastGuiltyPINSP() und ResetLastGuiltyHFamp() wird erst nach Deaktivierung durchgeführt 
	- CO2 Implementierung: neue ALarme: 
		AL_Sens_CO2_MODULE_NOTCONNECTED,	// 32,
		AL_Sens_CO2_FILTERLINE_NOTCONNECTED,// 33,
		AL_Sens_CO2_CHECKSAMPLINGLINE,	// 34,
		AL_Sens_CO2_CHECKAIRWAYADAPTER,	// 35,
		AL_Sens_CO2_SENSORFAULTY,		// 36,
	- CO2 neu implementiert (MicroPod & Capnostat)
	- wenn CO2-Modul aktiv und FLowsensor ausgeschaltet -> RR wird von CO2-Modul angezeigt
	- P-PROXIMAL-Alarm über serielle Schnittstelle implementiert
	- Erkennung NetDCU9 + LED/LCD Display mittels RegKey "HKLM\\Drivers\\Display\\SMIVGX\\Mode20"
	- etCO2 development project merged
	- Nebulizer implementiert
	- Languagefiles 1.0.0.74 nötig
	- CMVModel::SaveLastGuiltyBreathValues() nur wenn VENT_RUNNING==GetVentRunState()
	- DEMOVERSION als #define implementiert
	- AlarmLogList: wenn voll wird ältester Eintrag gelöscht 
	- ViewDiagramm/CircularBuffer: for-Schleifen durch memcpy ersetzt
	- Timer zum Wechsel von den Menüs zum HomeBS auf 45 Sekunden erhöht
	- Timer der Parameterbuttons auf 45 Sekunden erhöht
	- HOMETIMER (Moduswechsel) auf 45 Sekunden erhöht
	- Trigger ParaBtn: Anzeige je nach Wert: sensitivity high/middle/low
	- PSV-Modus: Pinsp wir bei Aktivierung nicht erhöht, bei Alarm "Volumen nicht erreicht" wird Pfeil nach oben in Ppsv-Para gezeichnet (nicht mehr Pinsp)
	- PDMS und CO2-Project zusammen geführt
	- InfantFlowLP included, LanguageFile 1.0.0.76 nötig!
	- Sprachfiles bezüglich Tubeset angepasst
	- AfxGetMainWnd()->PostMessage() durch AfxGetApp()->GetMainWnd()->PostMessage() ersetzt
	- bei VueLink wird nur der aktive Alarm gesetzt, alle anderen sind '0', bei acuLink werden alle Alarmstati gesetzt
	- acuLink: ALINK_ALARM_ENABLED implementiert, Messwerte werden nicht mehr in AVG und BTB unterschieden
	- autoIFlow deaktiviert
	- View wird bei ASTATE_MANSILENT und Patientenalarm nicht mehr umgeschaltet
 @endverbatim
 **/