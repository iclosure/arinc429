
#ifndef CRDTECH_ARAPI_H
#define CRDTECH_ARAPI_H

/* ��Ʒ�汾 */
#define ARAPI_MAJOR_VER  2
#define ARAPI_MINOR_VER  3
#define ARAPI_PATCH_VER  0
#define ARAPI_BUILD_VER  0

/*----------------------------------------------------------------------------*
 *  
 *---------------------------------------------------------------------------*/

#define ARINC_LABEL_NUM			256	  /* ����������                           */
#define ARINC_MAX_CH_NUM		16    /* ���ͨ��֧������                      */

#define ARINC_BOARD_TYPE_PCI	 0     /* PCI��429 */
#define ARINC_BOARD_TYPE_USB	 1     /* USB��429 */
#define ARINC_BOARD_TYPE_UNKNOWN 2     /* Unknown */

/*----------------------------------------------------------------------------*
 *  ȫͨ����������ѡ���
 *----------------------------------------------------------------------------*/
#define ARU_XMIT_RATE           1 /* �������ʣ�
                                     AR_100K  (100Kb) 
									 AR_50K   (50Kb) 
									 AR_12_5K (12.5Kb) 
                                     ���ö�����ͨ����Ч */

#define ARU_RECV_RATE           2 /* �������ʣ�
                                     AR_100K  (100Kb) 
									 AR_50K   (50Kb) 
									 AR_12_5K (12.5Kb) 
                                     ���ö�����ͨ����Ч */

#define ARU_PARITY              3 /* ��żЧ�飺
									 AR_NONE            ��Ч��
									 AR_ODD             żЧ��
									 AR_EVEN            ��Ч��
                                     ���ö�����ͨ����Ч */

#define ARU_INTERNAL_WRAP       4 /* �ڲ��ػ����ԣ�
                                     AR_WRAP_OFF        �ڲ��ػ�
                                     AR_WRAP_ON         �ڲ����ػ�
                                     ���ö�����ͨ����Ч */

/*----------------------------------------------------------------------------*
 *  �������ú���
 *----------------------------------------------------------------------------*/
#define AR_WRAP_OFF               0  /* �ڲ����ػ�                            */
#define AR_WRAP_ON                1  /* �ڲ��ػ�                              */
#define AR_OFF                    8  /* ͨ����Ч��־                          */
#define AR_ON                     7  /* ͨ����Ч��־                          */

/* У��λ����ѡ�� */
#define AR_NONE                   0  /* ��Ч��								  */
#define AR_ODD                    1  /* ����������Ч��                         */
#define AR_EVEN                   2  /* ��������żЧ��                         */

/* ����������ѡ�� */
#define AR_12_5K                  3  /* ʹ�õ��ٲ�����(12.5 kbps)              */
#define AR_50K                    1  /* ʹ������Ĳ�����(50 kbps)              */
#define AR_100K                   0  /* ʹ�ø��ٲ�����(100 kbps)               */

#define AR_HIGH                   0  /* ʹ�ø��ٲ�����(100 Kbaud)              */
#define AR_12K5					  3  /* ��AR_LOW��ͬ                          */
#define AR_LOW                    3  /* ʹ�õ��ٲ�����(12.5 Kbaud)             */

/* SDI�Ƚ�ֵ�������ѡ�� �� decoder -- BIT9 BIT8 -- */
#define ARS_SDI_00			      0
#define ARS_SDI_01                1
#define ARS_SDI_10                2
#define ARS_SDI_11                3

/*-----------------------------------------------------------------------------*
 *  ÿ������ͨ�������Ļػ����Զ���
 *  �����У�  
 *----------------------------------------------------------------------------*/
#define ARU_CH01_LOOPBACK      300
#define ARU_CH02_LOOPBACK      301
#define ARU_CH03_LOOPBACK      302  
#define ARU_CH04_LOOPBACK      303  
#define ARU_CH05_LOOPBACK      304
#define ARU_CH06_LOOPBACK      305
#define ARU_CH07_LOOPBACK      306
#define ARU_CH08_LOOPBACK      307
#define ARU_CH09_LOOPBACK      308  
#define ARU_CH10_LOOPBACK      309  
#define ARU_CH11_LOOPBACK      310  
#define ARU_CH12_LOOPBACK      311  
#define ARU_CH13_LOOPBACK      312
#define ARU_CH14_LOOPBACK      313
#define ARU_CH15_LOOPBACK      314
#define ARU_CH16_LOOPBACK      315
/*-----------------------------------------------------------------------------*
 *  ÿ������ͨ�������Ľ������ʲ�������
 *  �����У� AR_12_5K, AR_50K, AR_100K
 *----------------------------------------------------------------------------*/
#define ARU_RX_CH01_BIT_RATE   500  
#define ARU_RX_CH02_BIT_RATE   501  
#define ARU_RX_CH03_BIT_RATE   502  
#define ARU_RX_CH04_BIT_RATE   503  
#define ARU_RX_CH05_BIT_RATE   504
#define ARU_RX_CH06_BIT_RATE   505
#define ARU_RX_CH07_BIT_RATE   506
#define ARU_RX_CH08_BIT_RATE   507
#define ARU_RX_CH09_BIT_RATE   508  
#define ARU_RX_CH10_BIT_RATE   509  
#define ARU_RX_CH11_BIT_RATE   510  
#define ARU_RX_CH12_BIT_RATE   511  
#define ARU_RX_CH13_BIT_RATE   512
#define ARU_RX_CH14_BIT_RATE   513
#define ARU_RX_CH15_BIT_RATE   514
#define ARU_RX_CH16_BIT_RATE   515
/*----------------------------------------------------------------------------*
 *  ÿ������ͨ�������ķ������ʲ������塣
 *  �����У� AR_12_5K, AR_50K, AR_100K
 *----------------------------------------------------------------------------*/
#define ARU_TX_CH01_BIT_RATE   700  
#define ARU_TX_CH02_BIT_RATE   701  
#define ARU_TX_CH03_BIT_RATE   702  
#define ARU_TX_CH04_BIT_RATE   703
#define ARU_TX_CH05_BIT_RATE   704
#define ARU_TX_CH06_BIT_RATE   705
#define ARU_TX_CH07_BIT_RATE   706
#define ARU_TX_CH08_BIT_RATE   707
#define ARU_TX_CH09_BIT_RATE   708
#define ARU_TX_CH10_BIT_RATE   709  
#define ARU_TX_CH11_BIT_RATE   710  
#define ARU_TX_CH12_BIT_RATE   711
#define ARU_TX_CH13_BIT_RATE   712
#define ARU_TX_CH14_BIT_RATE   713
#define ARU_TX_CH15_BIT_RATE   714
#define ARU_TX_CH16_BIT_RATE   715

/*----------------------------------------------------------------------------*
 *  ÿ������ͨ����������żЧ���������
 *  �����У� AR_NONE��AR_ODD��AR_EVEN
 *---------------------------------------------------------------------------*/
#define ARU_RX_CH01_PARITY     900   /* ���ý���ͨ����żЧ�����               */
#define ARU_RX_CH02_PARITY     901   /* AR_NONE��AR_ODD��AR_EVEN             */
#define ARU_RX_CH03_PARITY     902   /* ȱʡΪAR_NONE ģʽ(��Ч��)             */
#define ARU_RX_CH04_PARITY     903   /* ��żЧ�鹦����Ч����Ч��ʱ��������Ϊ   */
#define ARU_RX_CH05_PARITY     904   /* żЧ��ʱ��������Ϊż�����������ݵ�     */
#define ARU_RX_CH06_PARITY     905   /* MSBΪ0��                             */
#define ARU_RX_CH07_PARITY     906
#define ARU_RX_CH08_PARITY     907
#define ARU_RX_CH09_PARITY     908 
#define ARU_RX_CH09_PARITY     908
#define ARU_RX_CH10_PARITY     909
#define ARU_RX_CH11_PARITY     910
#define ARU_RX_CH12_PARITY     911
#define ARU_RX_CH13_PARITY     912
#define ARU_RX_CH14_PARITY     913
#define ARU_RX_CH15_PARITY     914
#define ARU_RX_CH16_PARITY     915

/*----------------------------------------------------------------------------*
 *  ÿ������ͨ����������żЧ���������
 *  �����У� AR_NONE��AR_ODD��AR_EVEN
 *---------------------------------------------------------------------------*/
#define ARU_TX_CH01_PARITY     1100  /* ���÷���ͨ����żЧ�����               */
#define ARU_TX_CH02_PARITY     1101  /* AR_NONE��AR_ODD��AR_EVEN             */
#define ARU_TX_CH03_PARITY     1102  /* ȱʡΪAR_NONE ģʽ(��Ч��)             */
#define ARU_TX_CH04_PARITY     1103
#define ARU_TX_CH05_PARITY     1104
#define ARU_TX_CH06_PARITY     1105
#define ARU_TX_CH07_PARITY     1106
#define ARU_TX_CH08_PARITY     1107
#define ARU_TX_CH09_PARITY     1108
#define ARU_TX_CH10_PARITY     1109
#define ARU_TX_CH11_PARITY     1110
#define ARU_TX_CH12_PARITY     1111
#define ARU_TX_CH13_PARITY     1112
#define ARU_TX_CH14_PARITY     1113
#define ARU_TX_CH15_PARITY     1114
#define ARU_TX_CH16_PARITY     1115
/*----------------------------------------------------------------------------*
 *  ÿ������ͨ��������SDI�ȽϿ��ز�������
 *  �����У� ARU_ON��AR_OFF
 *---------------------------------------------------------------------------*/
#define ARU_CH01_DECODER       1300
#define ARU_CH02_DECODER       1301
#define ARU_CH03_DECODER       1302
#define ARU_CH04_DECODER       1303  
#define ARU_CH05_DECODER       1304
#define ARU_CH06_DECODER       1305
#define ARU_CH07_DECODER       1306
#define ARU_CH08_DECODER       1307
#define ARU_CH09_DECODER       1308
#define ARU_CH10_DECODER       1309
#define ARU_CH11_DECODER       1310
#define ARU_CH12_DECODER       1311
#define ARU_CH13_DECODER       1312
#define ARU_CH14_DECODER       1313
#define ARU_CH15_DECODER       1314
#define ARU_CH16_DECODER       1315
/*----------------------------------------------------------------------------*
 *  ÿ������ͨ��������SDI�Ƚ�ֵ��������
 *  �����У� ARS_SDI_00��ARS_SDI_01��ARS_SDI_10��ARS_SDI_11
 *---------------------------------------------------------------------------*/
#define ARU_CH01_SDI           1400
#define ARU_CH02_SDI           1401
#define ARU_CH03_SDI           1402
#define ARU_CH04_SDI           1403
#define ARU_CH05_SDI           1404
#define ARU_CH06_SDI           1405
#define ARU_CH07_SDI           1406
#define ARU_CH08_SDI           1407
#define ARU_CH09_SDI           1408
#define ARU_CH10_SDI           1409
#define ARU_CH11_SDI           1410
#define ARU_CH12_SDI           1411
#define ARU_CH13_SDI           1412
#define ARU_CH14_SDI           1413
#define ARU_CH15_SDI           1414
#define ARU_CH16_SDI           1415

/*----------------------------------------------------------------------------*
 *  ���庯������
 *----------------------------------------------------------------------------*/

#define ARU_ALL_LABELS          0x1FF /* ��/�ر����еı�Ź���              */
#define AR_LABEL_NUM			256   /* label number */

#define ARU_FILTER_OFF          0     /* ��Ź��˹ر�--ͨ��ָ���ı��         */
#define ARU_FILTER_ON           1     /* ��Ź��˴�--ɾ��ָ���ı��         */

/*---------------------------------------------------------------------------*
 *  ��������ֵ����
 *---------------------------------------------------------------------------*/
#define ARS_FAILURE             -1

#define ARS_NODATA              0       /* ���ջ�����������                    */
#define ARS_NORMAL              1       /* ���������ɹ�                        */
#define ARS_GOTDATA             4       /* ���ջ����������ݱ�־                 */

#define ARS_TIMEOUT				1003    /* ��ʱ                                */
#define ARS_INVHARCMD			1004	/* ��Ч����������                       */
#define ARS_INVHARVAL			1005	/* ��Ч������ֵ                         */
#define ARS_XMITOVRFLO			1006	/* ���ͻ�������                         */
#define ARS_INVBOARD			1007	/* ��Ч�İ忨��                         */
#define ARS_BRDNOTLOAD			1008	/* �忨û�г�ʼ��                       */
#define ARS_INVARG				1009	/* ��Ч��ͨ����־                       */
#define ARS_WINRTFAIL			1010	/* Ӳ���豸����                         */

/*----------------------------------------------------------------------------*
 *  Arinc429���ݴ����ʽ����
 *----------------------------------------------------------------------------*/
#define AR_WORD_WIDTH           32           /* ����λ�� */

#define AR_WORD_LABEL_OFFSET    0            /* ��Ϣ��ʶ��λƫ���� */
#define AR_WORD_LABEL_MASK      0x000000FF   /* ��Ϣ��ʶ������ */

#define AR_WORD_SDI_OFFSET      8            /* Դ��Ŀ���ʶ��λƫ���� */ 
#define AR_WORD_SDI_MASK        0x00000300   /* ��Ϣ��ʶ������ */
#define AR_WORD_SDI_0           0x01         /* SDI0 */ 
#define AR_WORD_SDI_1           0x02         /* SDI1 */  

#define AR_WORD_DATA_OFFSET     10           /* ��Ϣ��ʶ��λƫ���� */
#define AR_WORD_DATA_MASK       0x1FFFFC00   /* ���ݶ����� */

#define AR_WORD_SSM_OFFSET      29           /* ��Ϣ��ʶ��λƫ���� */
#define AR_WORD_SSM_MASK        0x60000000   /* SSM���� */

#define AR_WORD_PARITY_OFFSET   31
#define AR_WORD_PARITY_MASK     0x80000000   /* У��λ���� */

/*----------------------------------------------------------------------------*
*  api functions
*----------------------------------------------------------------------------*/

#define __crd_api	__stdcall

#ifdef __cplusplus
extern "C" {
#endif	// _cplusplus

typedef void (*UserHandle)();
int  __crd_api ar_set_userhandle(UserHandle pUser);
int  __crd_api ar_board_num();
int  __crd_api ar_board_type(int board);

/* ---------------------------------------------------------
	                       ͨ�ú���
   --------------------------------------------------------- */

int  __crd_api ar_loadslv(int board, int base_reg, int base_port, int ram_size);
int  __crd_api ar_go(int board);
int  __crd_api ar_close(int board);
int  __crd_api ar_reset(int board);

/* ---------------------------------------------------------
	                     ARINC429 ����
   --------------------------------------------------------- */

int  __crd_api ar_putword(int board, int channel, unsigned long data);
int  __crd_api ar_getnext(int board, int channel, unsigned long* data);
int  __crd_api ar_getnextt(int board, int channel, unsigned long* data, void* timestamp);
int  __crd_api ar_set_config(int board, int item, int value);
int  __crd_api ar_get_config(int board, int item, int* value);
int  __crd_api ar_label_filter(int board, int channel, int label, int action);
void __crd_api ar_get_latest(int board, int channel, int label, void* data, int* seq_num);

int  __crd_api ar_num_rchans(int board);
int  __crd_api ar_num_xchans(int board);

/* extend functions */

/*************************************************
 *
 * For USB-interface arinc429 [start]
 *
 */

#pragma pack(1)  /* pack to 1 byte alignment */
typedef struct _ARINC_WORD_MSG
{
	unsigned long data; /* 4 bytes */
	__int64 timestamp;  /* 8 bytes (warning: not supported in some system, and can be replaced with four-words type) */

} ARINC_WORD_MSG, *PARINC_WORD_MSG;
#pragma pack()

int __crd_api ar_putwords(int board, int channel, unsigned long* buffer, int size, int* length);
int __crd_api ar_getwords(int board, int channel, unsigned long* buffer, int size, int* length);
int __crd_api ar_getwordst(int board, int channel, ARINC_WORD_MSG* buffer, int size, int* length);

/*
 *
 * For USB-interface arinc429 [end]
 *
 *************************************************/

#ifdef __cplusplus
};
#endif	/* _cplusplus */

#endif	/* !CRDTECH_ARAPI_H */
