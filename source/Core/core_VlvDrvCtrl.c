/*======================================================================
��� �����:          core_VlvDrvCtrl.c
�����:				����� �.�.
������ �����:		1.00
���� ���������:		16.04.2014
��������:
������ ���������� ���������
======================================================================*/

#include "core.h"

__inline void GetActiveControls(TCoreVlvDrvCtrl *);	// �������� ������ �� ���� ������������ �������
__inline void MpuControl(TCoreVlvDrvCtrl *);		// ������������ ������ ������
__inline void TeleControl(TCoreVlvDrvCtrl *);		// ������������ ������� ��/������
__inline void UnitControl(TCoreVlvDrvCtrl *);		// �������� �������� � ����������� �� ���������� ���������
__inline void DriveStop(TCoreVlvDrvCtrl *);			// �������� ��� ������� ����
__inline void DriveClose(TCoreVlvDrvCtrl *);		// ... ��� �������
__inline void DriveOpen(TCoreVlvDrvCtrl *);			// ... ��� �������
__inline void UpdateComm(TCoreVlvDrvCtrl *);		// ��������� ���������� ������� ��� ��������� �������
static   void ReverseDrive(TCoreVlvDrvCtrl *);		// �������� ��� ��������

__inline void GetCurrentCommand(TCoreVlvDrvCtrl *);

void Core_ValveDriveInit(TCoreVlvDrvCtrl *p)
{
	p->Status 				= ToPtr(&g_Core.Status);
	p->ControlWord 			= &g_Ram.ramGroupD.ControlWord;
	p->MuDuSetup			= &g_Ram.ramGroupB.MuDuSetup;
	p->DuSource				= &g_Ram.ramGroupB.DuSource;
	p->ReverseType			= &g_Ram.ramGroupH.ReverseType;
	p->Mpu.Enable			= true;
	p->Mpu.BtnKey			= &g_Ram.ramGroupH.CmdButton;
	p->Mpu.PduKey			= &g_Ram.ramGroupH.CmdKey;
	p->Mpu.CancelFlag		= false;
	p->Tu.Enable			= true;
	p->Tu.LocalFlag			= false;
	p->Tu.State				= 0;
	p->Tu.Ready				= true;
	p->Tu.LockSeal			= &g_Ram.ramGroupB.TuLockSeal;
	p->Valve.PosRegEnable 	= true;
	p->Valve.BreakFlag		= false;
	p->Valve.Position		= POS_UNDEF;
	p->Valve.BreakDelta		= 0;
	p->Valve.BreakMode 		= &g_Ram.ramGroupB.BreakMode;
	p->Valve.CalibStates	= &g_Ram.ramGroupA.CalibState;
	p->Valve.CalibFullStep  = &g_Ram.ramGroupH.FullStep;
	p->EvLog.Source			= 0;
	p->EvLog.Value			= 0;
	p->Command				= vcwNone;
	p->MuDuInput			= 0;
	p->ActiveControls		= 0;
	p->StartDelay			= 0;
	p->IgnorComFlag			= 0;
	p->StartControl			= &StartPowerControl;
	p->StopControl			= &StopPowerControl;

}

void Core_ValveDriveStop(TCoreVlvDrvCtrl *p)
{
	p->Valve.BreakFlag = False;

	if (p->Status->bit.Fault || !(*p->Tu.LockSeal)) p->Tu.Ready = False;

	p->Valve.Position = POS_UNDEF;

	p->IgnorComFlag = 0;

	p->StopControl();
}

void Core_ValveDriveUpdate(TCoreVlvDrvCtrl *p)
{
	GetCurrentCommand(p);	// ��������� ������ �� ������� ��������
	GetActiveControls(p);	// ��������� ������ �� ��������������� ���� ���������� (�������/���������)
	TeleControl(p);			// ������ ������ �������������� ����������
	MpuControl(p);			// ������ ������ �������� ����������
	UnitControl(p);			// �������� � ����������� �� ������� (�������/�������/����)
}

__inline void GetCurrentCommand(TCoreVlvDrvCtrl *p)
{
	// �������


	// ���������
}

//
__inline void GetActiveControls(TCoreVlvDrvCtrl *p)
{
		Bool Flag = False;
		Uns  DigState = 0;

		p->ActiveControls = 0;

		switch(*p->MuDuSetup)
		{
			case mdOff:    p->Status->bit.MuDu = 0; Flag = True; break;
			case mdSelect: p->Status->bit.MuDu = p->MuDuInput; break;
			case mdMuOnly: p->Status->bit.MuDu = 1; break;
			case mdDuOnly: p->Status->bit.MuDu = 0; break;
			default:       return;
		}

		if (Flag || p->Status->bit.MuDu)
		{
			p->ActiveControls |= (CMD_SRC_PDU|CMD_SRC_MPU);
			if (p->Tu.LocalFlag) p->ActiveControls |= CMD_SRC_DIGITAL;
		}

		if (Flag || !p->Status->bit.MuDu)
		{
			DigState = p->Tu.LocalFlag ? 0 : CMD_SRC_DIGITAL;
			switch(*p->DuSource)
			{
				case mdsAll:     p->ActiveControls |= (DigState|CMD_SRC_SERIAL); break;
				case mdsDigital: p->ActiveControls |= DigState; break;
				case mdsSerial:  p->ActiveControls |= CMD_SRC_SERIAL; break;
			}
		}
}

// ��������� ������ � ���
__inline void MpuControl(TCoreVlvDrvCtrl *p)
{
	Uns Key = 0;
			Uns  Active = 0;

		if(!p->Mpu.Enable) return;				// ��������� �������

		if (p->Mpu.BtnKey)						// ������ ������� � �����
		{
			Key = *p->Mpu.BtnKey;				// ��������� ��������� �������
			Active = (p->ActiveControls & CMD_SRC_MPU); // ��������� ����������� �� ����� ���
			p->EvLog.Source = CMD_SRC_MPU;				// �������� ������� ��� �������
			p->Mpu.BtnKey = 0;					// �������� �������
		}

		if (p->Mpu.PduKey)						//����������
		{
			Key = *p->Mpu.PduKey;
			Active = (p->ActiveControls & CMD_SRC_PDU);
			p->EvLog.Source = CMD_SRC_PDU;
			p->Mpu.PduKey = 0;
		}

		switch (Key)							// ��������� ��������� �������
		{
			case KEY_STOP:						// ���� ����
				*p->ControlWord = vcwStop;		// ������ ������� �� ���������, ��� ����������� �� ��������� ������ ���
				break;
			case KEY_CLOSE:
				if (Active) *p->ControlWord = vcwClose;	// ���� ����� ��� ����������� �� �������� �������
				else p->Mpu.CancelFlag = True;			// ���� ����� ��� �� ������� �� ������ ���� ������ �������
				break;
			case KEY_OPEN:
				if (Active) *p->ControlWord = vcwOpen;
				else p->Mpu.CancelFlag = True;
				break;
		}
}
__inline void TeleControl(TCoreVlvDrvCtrl *p)
{
	TValveCmd TuControl = vcwNone;
	Bool Ready;

	if (!p->Tu.Enable) return;

	if (!(p->ActiveControls & CMD_SRC_DIGITAL))
	{
		p->Tu.State &= ~(TU_STOP|TU_CLOSE|TU_OPEN);
		p->Tu.Ready = True;
		return;
	}

	Ready = p->Status->bit.Stop && p->Tu.Ready;

	switch(p->Tu.State & (TU_CLOSE|TU_OPEN))
	{
		case TU_CLOSE: if (Ready) TuControl = vcwClose; break;
		case TU_OPEN:	if (Ready) TuControl = vcwOpen;  break;
		case (TU_CLOSE|TU_OPEN):  TuControl = vcwStop;  break;
	}

	if (p->Tu.State & TU_STOP) TuControl = vcwStop;

	if (TuControl != vcwNone)
	{
		*p->ControlWord = TuControl;
		p->EvLog.Source = CMD_SRC_DIGITAL;
	}
	p->Tu.State &= ~(TU_STOP|TU_CLOSE|TU_OPEN);
}

__inline void UnitControl(TCoreVlvDrvCtrl *p)
{
		UpdateComm(p);

		if (p->StartDelay > 0) p->StartDelay--;
		else if (*p->ControlWord)
		{
			switch(*p->ControlWord)
			{
				case vcwStop:      DriveStop(p);  	break;
				case vcwClose:
				case vcwTestClose: DriveClose(p); 	break;
				case vcwOpen:
				case vcwTestOpen:  DriveOpen(p);  	break;
				case vcwDemo:        				break;
				case vcwTestEng:
				default: p->StartControl(*p->ControlWord);
			}
			*p->ControlWord = vcwNone;
		}
}

__inline void DriveStop(TCoreVlvDrvCtrl *p)
{
	Core_ValveDriveStop(p);
	p->EvLog.Value = CMD_STOP;
}

__inline void DriveClose(TCoreVlvDrvCtrl *p)
{
	Bool FreeRun;

	if (p->Status->bit.Closed && (*p->ControlWord != vcwTestClose)) // SVS
	{
		p->IgnorComFlag = 1;										// ���� ��� ������� �� ���������� ���� ������������� �������
	 	return;
	}
	if(p->Status->bit.Closing) return;								// ���� ���� �������� �� ������
	if (p->Status->bit.Opening) {ReverseDrive(p); return;}

	p->Valve.BreakFlag = False;										//
	FreeRun = (*p->ControlWord == vcwTestClose) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);		// ����� ���������� �������� - ���� �������� �������� ��� �� ��������������
	if (FreeRun) p->Valve.Position = POS_UNDEF;						//
	else if (!((*p->Valve.BreakMode == vtClose) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = 0;				// ���� �� �������� � ������ ���������� - ������� ��������� 0
	}
	else															// ���� �������� � ������ ����������
	{
		p->Valve.BreakFlag = True;									// ��������� ���� ������ � ����������
		p->Valve.Position  = -(LgInt)p->Valve.BreakDelta;			// �������� ������� ��������� �� ���������� ��� ��������� ����� ��� ����������
	}
	p->EvLog.Value = CMD_CLOSE;	// �������� ������� ��� �������

	p->StartControl(*p->ControlWord); // ������ ������� ���������� ��� ������ ������
}

__inline void DriveOpen(TCoreVlvDrvCtrl *p)
{
	Bool FreeRun;

	if (p->Status->bit.Opened && (*p->ControlWord != vcwTestOpen)) // SVS
	{
		p->IgnorComFlag = 1;
	 	return;
	}
	if (p->Status->bit.Opening) return;
	if (p->Status->bit.Closing) {ReverseDrive(p); return;}

	p->Valve.BreakFlag = False;
	FreeRun = (*p->ControlWord == vcwTestOpen) || !(*p->Valve.CalibStates == VLV_CLB_FLAG);
	if (FreeRun) p->Valve.Position = POS_UNDEF;
	else if (!((*p->Valve.BreakMode == vtOpen) || (*p->Valve.BreakMode == vtBoth)))
	{
		p->Valve.Position = (LgInt)p->Valve.CalibFullStep;
	}
	else
	{
		p->Valve.BreakFlag = True;
		p->Valve.Position  = (LgInt)p->Valve.CalibFullStep + (LgInt)p->Valve.BreakDelta;
	}
	p->EvLog.Value = CMD_OPEN;

	p->StartControl(*p->ControlWord);
}

__inline void UpdateComm(TCoreVlvDrvCtrl *p)		// ������� �������������� ������ ������� ��� ������� ()
{
	if (p->Command != vcwNone)					// ���� ������ ����������
	{
		if (p->Command == vcwStop) *p->ControlWord = vcwStop;	// ���� ������� ��� ������� ����, ������ ���������� ������� ����
		else if (!p->Status->bit.Stop) return;					// ���� ���� �����������
		else *p->ControlWord = p->Command;						// ������ ������� � ��������������� ������������
		p->Command = vcwNone;									// ���������� ���������� �������
	}
}

void ReverseDrive(TCoreVlvDrvCtrl *p)				// ������� �������
{
	switch (*p->ReverseType)					// � ����������� �� ���� �������
	{
		case rvtStop:  p->Command = vcwStop; break;	// ���� ��� �������
		case rvtAuto: {
						p->Command = *p->ControlWord; Core_ValveDriveStop(p);	// ����� ���� ������� � ������ �������
						break;
					  }
		case rvtNone: *p->ControlWord = vcwNone; break; // ������������� ������� ����������
	}
}




