/*
 *
 * ��Ȩ(c) 2017 ��ʯ���⣨�������Ƽ����޹�˾
 * ��Ȩ(c) 2011-2017 ��ʯ���⣨�������Ƽ����޹�˾��Ȩ����
 * 
 * ���ĵ�������Ϣ��˽�ܵ���Ϊ��ʯ����˽�У������񷨺��̷��ж���Ϊ��ҵ������Ϣ��
 * �κ���ʿ�������Ը�ӡ��ɨ����������κη�ʽ���д����������ʯ������Ȩ׷���������Ρ�
 * �Ķ���ʹ�ñ����ϱ�������Ӧ��������Ȩ���е��������κͽ�����Ӧ�ķ���Լ����
 *
 */

#include "rs_ua_flash.h"
#include "rs_ua_porting.h"

#include "hal_flash.h"


/**
* ����˵����flash��ʼ��
* ����˵������
* ����ֵ���ɹ�����rs_true��ʧ��Ϊrs_false
*/
rs_bool rs_ua_flash_init(void)
{
		
	 hal_flash_status_t ret;
	 ret = hal_flash_init();
	if (ret < HAL_FLASH_STATUS_OK){
		 rs_trace("rs_ua_flash_init eror\n");
	  return rs_false;
	}
	return rs_true;
}

/**
* ����˵������ȡflash�е����ݵ��ڴ滺������
* ����˵����[IN OUT]destination ���ն�ȡflash�����ݵĻ�����
*           [IN]address flash�ĵ�ַ
*					  [IN]size ��Ҫ��ȡ�ĳ���
*
* ����ֵ���ɹ�����rs_true, ʧ�ܷ���rs_false
*/
rs_bool rs_ua_flash_read(rs_u8 *destination,
                       volatile rs_u8 *address,
                       rs_u32 size)
{
    hal_flash_status_t status = HAL_FLASH_STATUS_OK;

	//rs_trace("rs_ua_flash_read  address = 0x%x  size = %d \r\n", address , size);
	
	status = hal_flash_read((uint32_t )address, (uint8_t *)destination, size);
	if (status != HAL_FLASH_STATUS_OK) {
		rs_trace("hal_flash_read  address = 0x%x  size = %d status = %d \r\n", address , size , status);
		return rs_false;
	}
	return rs_true;
}

/**
* ����˵�������ڴ滺��������д�뵽flash��
* ����˵����[IN]address flash�ĵ�ַ
*           [IN]source ��Ҫд������ݻ�����
*					  [IN]size  ���ݳ���
*
* ����ֵ���ɹ�����rs_true, ʧ�ܷ���rs_false
*
* ��ע��address���������ж���block����ģ�size����block�ĳ��ȣ����ÿ��ǲ���������������Ӧ�ò����ع��������ֱ����
				��flash����д��ʱ�򣬶���page����ģ�size��һ��page�Ĵ�С��
*/
rs_bool rs_ua_flash_write(volatile rs_u8 *address,
                       rs_u8 *source,
                       rs_u32 size)
{
						   
   hal_flash_status_t status = HAL_FLASH_STATUS_OK;
   //rs_trace("rs_ua_flash_write address = 0x%x  size = %d \r\n", address , size);
   
   status = hal_flash_write((uint32_t)address, (uint8_t *)source, size);
   if (status != HAL_FLASH_STATUS_OK) {
		rs_trace("hal_flash_write  address = 0x%x  size = %d status = %d \r\n", address , size , status);
		return rs_false;
   }
   return rs_true;
}


/**
* ����˵������block����flash
* ����˵����[IN]address flash�ĵ�ַ
*					  [IN]size Ҫ���������ݵĳ���
*
* ����ֵ���ɹ�����rs_true, ʧ�ܷ���rs_false
*
* ��ע�����������������л���Ӧ�ò㣬address����block����ģ�size����block�ĳ��ȡ�
*/
rs_bool rs_ua_flash_erase(volatile rs_u8 *address,
                        rs_u32 size)
{
						   
   hal_flash_status_t status = HAL_FLASH_STATUS_OK;

   //rs_trace("rs_ua_flash_erase  address = 0x%x  size = %d \r\n", address , size);
  
   status = hal_flash_erase((uint32_t)address,	HAL_FLASH_BLOCK_4K);
   if (status != HAL_FLASH_STATUS_OK) {
		rs_trace("hal_flash_erase    address = 0x%x  size = %d status = %d \r\n", address , size , status);
		return rs_false;
   }
   
   return rs_true;
}


