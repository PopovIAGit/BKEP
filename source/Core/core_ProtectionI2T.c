/*======================================================================
��� �����:          core_ProtectionI2T.c
�����:				����� �.�.
������ �����:		1.00
���� ���������:		28.04.2014
��������:
������ �����-������� ������
======================================================================*/

#include "core.h"

void Core_ProtectionI2TInit(TAlarmI2T *p)
{
	//p->minHighCurrent = (Uns)_IQ15int( _IQmpy (_IQ15(*p->NomCurrent), _IQ(K_HIGH_I)) );
	//p->maxLowCurrent  = (Uns)_IQ15int( _IQmpy (_IQ15(*p->NomCurrent), _IQ(K_LOW_I)) );

	p->minHighCurrent = *p->NomCurrent * K_HIGH_I;
	p->maxLowCurrent  = *p->NomCurrent * K_LOW_I;
}

void Core_ProtectionI2TUpdate(TAlarmI2T *p)
{
	register unsigned BitMask;

		BitMask = (1U << p->Cfg.bit.Num);	// ������� ���� Unsigned �������� ����� �� ����� ���� � �������� ������. �������� ����� ������.
		if (!p->Cfg.bit.Enable)	// ���� "���������� ������"=0, �� ���� "����", ��
		{
			*p->Output &= ~BitMask;	// �������� "�����" �� ��������������� �����, �� ����, ������� ������
			p->Timer = 0;
			return;		// ������� �� �������
		}

		if (!(*p->Output & BitMask))	// ���� ������ ��� (*p->Output & BitMask ���� "0")
		{
			if (*p->InputCurrentMid > p->minHighCurrent )	// ���� ��� �������� ��������, ��� ������� �����-������� ������ ����������� �����������
			{	// ���������� ������ ����������
				*p->Output |= BitMask;
				p->Timer = 0;
			}
			else
			if (*p->InputCurrentMid < p->maxLowCurrent)	// ���� ��� ���� ��������, ��� ������� �����-������� ������ �� �������
			{	// ��������� ����������
				p->Timer = 0;
			}
			else
			{
				p->Timeout =  I2T_CONV( *p->InputCurrentMid, *p->NomCurrent, _IQ15(37.5), _IQ15(1.15), _IQ15(50));
				if (++p->Timer >= (p->Timeout * p->Scale)) //  ������  ������ ������� ������������ ������
				{
					*p->Output |= BitMask;	// ���������� ������
					p->Timer = 0;
				}
			}
		}
}
