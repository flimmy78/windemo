/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* COPYRIGHT NOTICE. 
* Copyright (c), 2015 Lierda.
* All rights reserved.
* 
* @file KDLock.h.
* @brief 
* 
* @version 1.0
* @author au
* @date 2016/09/21-11:38:02
* @email dongliang@lierda.com
* 
* @revision:
*  - 1.0 2016/09/21by au.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef __KDLOCK_H_
#define __KDLOCK_H_
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push)
#pragma pack(1)

  class KDLock {
  public:
	KDLock();
	virtual ~KDLock();
	
	int Ack(uint8 *ack, uint16 &len);

	int Reset(uint8 *req, uint16 &len, uint8 in_out);
	int ReportStatus(uint8 *req, uint16 &len); 
	int AlertInfo(uint8 *req, uint16 &len);

	int ReadLockInfo(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int OpenLock(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int ReadRecord(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int SetMode(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int TimeCalibration(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int AddDelKey(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int ReadKeyInfo(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
	int ModifyKey(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len);
  private:
  };
  KDLock::KDLock() {
  }
  KDLock::~KDLock() {
  }
  int KDLock::Ack(uint8 *ack, uint16 &len) {
	//0x80����ȷ  0x81������   0x92��ID�Ŵ���  0x93��δ֪����       ����������
	*ack = 0x80;
	len = 1;
	return 0;
  }

  int KDLock::Reset(uint8 *req, uint16 &len, uint8 _in_out) {
	struct {
	  /* 0x00������  0x01������ */
	  uint8 in_out;
	} buf = {
	  _in_out,
	};
	len = sizeof(buf);
	memcpy(req, &buf, len);
	return 0;	
  }
  int KDLock::ReportStatus(uint8 *req, uint16 &len) {
	struct {
	  /* Bit0��1Ϊ������0Ϊ����
		 Bit1��1Ϊ�����Կ֪ͨ��0Ϊû��֪ͨ
		 Bit2��1Ϊɾ����Կ֪ͨ��0Ϊû��֪ͨ
		 Bit3����ʼ����ʾ��1Ϊ��ʼ����0Ϊû�г�ʼ��
		 Bit4��1Ϊ������0Ϊ�������
		 Bit5��1Ϊ������0Ϊȡ������
		 Bit6��1Ϊ�ŴŹأ�0Ϊ�Ŵſ�
		 Bit7������
	  */
	  uint8 status; 
	  /* 1������ 2��ָ�� 3����Ƭ 4����еԿ��5��ң�ؿ��� 6��һ��������12V�����źſ�����7��APP���� */
 	  uint8 key_1_type;
	  /* �����ŷ�Χ��0-9 ָ�Ʊ�ŷ�Χ��0-99 ��Ƭ��ŷ�Χ��0-99 ��еԿ�ױ�ţ�
		 100 ң�ؿ�����ţ�101 һ��������ţ�102 APP������ţ�103
	  */
	  uint8 key_1_num;
	  /* 0x01������ 
		 0x02��ָ�� 
		 0x03����Ƭ
	  */
	  uint8 key_2_type;
	  /* �����ŷ�Χ��0-9
		 ָ�Ʊ�ŷ�Χ��0-99
		 ��Ƭ��ŷ�Χ��0-99
	  */
	  uint8 key_2_num;
	  /* ����ʱ�䣨������ʱ���룩��ʱ�����BCD�롣
		 ���磺14 04 09 13 13 10��ʮ������������ʾ2014��4��9�� 13ʱ13��10��
	  */
	  uint8 openlock_time[6];
	  /* BIT0-BIT6Ϊ����1-100�����磺1 = 1%  33 = 33%    100 = 100% */
	  uint8 battery;
	} buf = {
	  (1 << 0) | ( 1 << 3),
	  7,
	  103,
	  0xff,
	  0xff,
	  {0x14, 0x04, 0x09, 0x13, 0x13, 0x10},
	  33,
	};
	len = sizeof(buf);
	memcpy(req, &buf, len);
	return 0;
  }
  int KDLock::AlertInfo(uint8 *req, uint16 &len) {
	struct {
	  /* Bit0��1Ϊ���󱨾���0Ϊ��������������������ָ�ƻ�Ƭ����10�ξͱ�����
		 Bit1��1Ϊ�ٳֱ��� ��0Ϊ��������������ٳ��������ٳ�ָ�ƿ����ͱ�����
		 Bit2��1Ϊ���˱���  ��0Ϊ�������������˿���
		 Bit3��1Ϊ��еԿ�ױ�����0Ϊ��������ʹ�û�еԿ�׿�����
		 Bit4��1Ϊ�͵�ѹ������0Ϊ��������ص������㣩
		 Bit5-7������
	  */
	  uint8 alertinfo;
	  /* BIT0-BIT6Ϊ����1-100�����磺1 = 1%  33 = 33%    100 = 100%
	   */
	  uint8 battery;
	} buf = {
	  0x00,
	  33,
	};
	len = sizeof(buf);
	memcpy(req, &buf, len);
	return 0;
  }

  int KDLock::ReadLockInfo(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct {
	  /* 0x80���ɹ�  0x81��ʧ��   ������ʧ��*/
	  uint8 ret;
	  /*
		BIT0��ȫģʽ��־��0������ģʽ��1����ȫģʽ��
		BIT1����ģʽ��־��0������ģʽ��1������ģʽ��
		BIT2-BIT3����ģʽ��־��0�����ģ�1��Ӣ�ģ� ������������
		BIT4-BIT7 ���������0��
	  */
	  uint8 workMode;
	  /* �͵�ѹ־+����ֵ��BIT7Ϊ�͵�ѹ��־��BIT0-BIT6Ϊ����1-100��
		 ���磺1 = 1%  33 = 33%    100 = 100%
	  */
	  uint8 lowpower;
	  /* BIT0:��ʾ����֧���������� ��1��֧�� 0����֧�֣�
		 BIT1:��ʾ����֧�ֿ�Ƭ     ��1��֧�� 0����֧�֣�
		 BIT2:��ʾ����֧��ָ������ ��1��֧�� 0����֧�֣�
		 BIT3-7 :���������0��
	  */
	  uint8 lockFunction;
	  /* ���磺�ַ�����F6113��(���7��ASCII�ַ�)������7���ַ������ַ�
	   */
	  uint8 lockType[7];
	  /* ���磺�ַ�����V1.0����5��ASCII�ַ���������5���ַ������ַ�
	   */
	  uint8 lockVersion[5];
	  /* ����ǰʱ�䣨������ʱ���룩��ʱ�����BCD�룬��30��Ž��и��¡�
		 ���磺14 04 09 13 13 10��ʮ����������
		 ��ʾ2014��4��9�� 13ʱ13��10��
	  */
	  uint8 currentTime[6];
	} buf = {
	  0x80,
	  (1 << 0) | (1 << 1) | (0 << 2) ,
	  33,
	  (1 << 0) | (1 << 1) | (1 << 2),
	  {'F', '6', '1', '1', '3'},
	  {'V', '1', '.', '0'},
	  {0x14, 0x04, 0x09, 0x13, 0x13, 0x10},
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;
  }
  int KDLock::OpenLock(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct oc_lock_t{
	  /* BIT0:���뿪����1��ʹ�ø���Կ����  0����ʹ�ã�
		 BIT1:ָ�ƿ���
		 BIT2:ˢ������
		 BIT3:APP����
		 BIT4-7 :���������0��
	  */
	  uint8 ocMode;
	  uint8 key[12];
	} *preq = (oc_lock_t*)req;
	struct {
	  uint8 ret;
	} buf = {
	  /* 0x80���ɹ�  0x81��ʧ��  0x8C������ʧ�ܣ�Ϊ��ȫģʽ   ������ʧ��
	   */
	  0x80,
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;
  }
  int KDLock::ReadRecord(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct read_record_t {
	  /* 0x00����ȡ���Ϊ00-19���ż�¼
		 0x01����ȡ���Ϊ20-39���ż�¼
		 ������ƣ�
		 0x09����ȡ���Ϊ180-199���ż�¼
		 0xFF����ȡ���ȫ�����ż�¼
	  */
	} *preq = (read_record_t*)req;
	struct {
	  /* 0x80���ɹ� 0x81��ʧ�� 0x82��û������ ������ʧ��*/
	  uint8 rsp;
	  /* MAX=200 */
	  uint8 current_num;
	  /* 1�ֽڣ���0��ʼ */
	  uint8 pktidx;
	  uint8 record[10];
	}buf = {
	  0x80,
	  0x01,
	  0x00,
	  {0x07, 0x00, 0x07, 0x00,
	   0x14, 0x04, 0x09, 0x13, 0x13, 10},
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;
  }
  int KDLock::SetMode(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct sm_t {
	  /* 0x00������ģʽ��0x01����ȫģʽ ��0xFF�����޸� */
	  uint8 securityMode;
	  /* 0x00������ģʽ��0x01������ģʽ��0xFF�����޸� */
	  uint8 voiceMode;
	  /* 0x00�����ģ�0x01��Ӣ�ģ�0xFF�����޸� */
	  uint8 languageMode;
	} *preq = (sm_t*)req;
	struct {
	  /* 0x80���ɹ�   0x81��ʧ��   
		 0x91��û���������ͬ������Կ         
		 0x94��û��Ȩ��    ������ʧ��
	   */
	  uint8 ret;
	} buf = {
	  0x80,
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;
  }
  int KDLock::TimeCalibration(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct tc_t {
	  uint8 time[6];
	} *preq = (tc_t*)req;
	struct {
	  /* 0x80���ɹ�  0x81��ʧ��  0x94��û��Ȩ��   ������ʧ�� */
	  uint8 ret;
	} buf = {
	  0x80
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;	
  }
  int KDLock::AddDelKey(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct adk_t {
	  /* 1.����     2.ɾ�� */
	  uint8 op;
	  /* 1.����     2. ָ��     3.��Ƭ */
	  uint8 key_type;
	  /* �����ŷ�Χ��00-09
		 ָ�Ʊ�ŷ�Χ��00-99
		 ��Ƭ��ŷ�Χ��00-99
	  */
	  uint8 key_num;
	  /* 1����Կ�ֽ�����12�ֽ�
		 2��������������0x00
		 3.  ����ɾ����0xFF
	  */
	  uint8 key[12];
	} *preq = (adk_t*)req;
	struct {
	  /* 0x80���ɹ�   0x81��ʧ��  0x83����Կ�Ѵ���  
		 0x84����Կ������   0x85������   0x86������Ѵ���
		 0x87����Ų����� 0x88���޴˹���  0x8D����ų�����Χ  
		 0x8E��������ڼ�  0x8F��������Ч  
		 0x90��ɾ��ʧ�ܣ�Ϊ��ȫģʽ   0x94��û��Ȩ��    ������ʧ�� 
	   */
	  uint8 ret;
	} buf = {
	  0x80,
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;	
  }
  int KDLock::ReadKeyInfo(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct rki_t {
	  /* 0x01������  0x02��ָ��  0x03����Ƭ  
		 0xFF����ȡ������Կ\
	  */
	  uint8 key_type;
	} *preq = (rki_t*)req;
	struct {
	  /* 0x80���ɹ� 0x81��ʧ�� 0x82��û������ ������ʧ��*/
	  uint8 rsp;
	  /* Max=10 */
	  uint8 key_num;
	  /* Max=100*/
	  uint8 finger_num;
	  /* Max = 100 */
	  uint8 card_num;
	  /* 1�ֽڣ���0��ʼ */
	  uint8 pktidx;
	  /* 1���� 2. ָ�� 3. ��Ƭ  */
	  uint8 key_type;
	  uint8 key_idx;
	} buf = {
	  0x80,
	  1, 0, 0,
	  0, 
	  1,
	  0,
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;	
  }

  int KDLock::ModifyKey(uint8 *req, uint16 qlen, uint8 *rsp, uint16 &len) {
	struct mk_t {
	  /* 1����������  2���û����� */
	  uint8 key_type;
	  /* ��ŷ�Χ��00-09 ���޸Ĺ�������ʱ�����ֽ���0XFF�� */
	  uint8 key_num;
	  /* ��Чλ��6-12λ������12λ���0xFF */
	  uint8 key[12];
	}*preq = (mk_t*)req;
	struct {
	  /* 0x80���ɹ�   0x81��ʧ��  0x83����Կ�Ѵ��� 
		 0x8E��������ڼ�(ʧ��)  0x8F��������Ч   
		 0x94��û��Ȩ��    ������ʧ�� 
	  */
	  uint8 ret;
	} buf = {
	  0x00,
	};
	len = sizeof(buf);
	memcpy(rsp, &buf, len);
	return 0;
  }

#pragma pack(pop)
#ifdef __cplusplus
}
#endif
#endif
