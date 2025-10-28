/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu0_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.10.2
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/
#include "motor.h"
#include "zf_common_headfile.h"
#include "switch.h"
#include "servo.h"
#include "rev_perspective.h"
#include "image_process.h"
#include "cv2.h"
#pragma section all "cpu0_dsram"
int offset = sizeof(uint8) * MT9V03X_W * ((MT9V03X_HS - MT9V03X_H) / 2);
uint8 bottom_threshold;
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU0��RAM��

// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������
// �������ǿ�Դ��չ��� ��������ֲ���߲��Ը���������

// **************************** �������� ****************************
int core0_main(void)
{
    clock_init();                   // ��ȡʱ��Ƶ��<��ر���>
    debug_init();                   // ��ʼ��Ĭ�ϵ��Դ���
    // �˴���д�û����� ���������ʼ����
    servo_init();
    motor_init();
    //switch_init();
    tft180_init();
    tft180_set_dir(TFT180_PORTAIT);
    mt9v03x_init();
//    InitLookupTable();
    // �˴���д�û����� ���������ʼ������
    cpu_wait_event_ready();
    // �ȴ����к��ĳ�ʼ�����
    while (TRUE)
    {

        ImagePerspective();
        compress_image((uint8 *) gray_image, (const uint8 *)mt9v03x_image + offset);  // ѹ��ͼ�񣬽�188*90ѹ����94*45������gray_image
        mt9v03x_finish_flag = 0;  // ԭʼͼ������ϣ������־λ
        cover_car_head();

        // �˴���д��Ҫѭ��ִ�еĴ���
        servo_set_angle(97);//85~110,MIN=97
        //ͼ���ʼ��
        calculate_contrast_x8((uint8 *)contrast_image, (const uint8 *)gray_image, 94, 45);  // ����Աȶ�
        memcpy((uint8 *) binary_image, (const uint8 *) contrast_image, 94 * 45);  // ���ƶԱȶ�ͼ�񵽴���ֵ��ͼ��
        my_cv2_doubleThreshold((uint8 *) binary_image, 94, 0, 0, 94, 45, 16, 30);
        my_cv2_checkConnectivity((uint8 *) binary_image, 94, 0, 0, 94, 45);  // �����ͨ��
        my_cv2_threshold((uint8 *) binary_image, 94, 0, 0, 94, 45, 127, 1);
        memcpy(gray_binary_image, (const uint8 *) gray_image, 94 * 45);
        bottom_threshold = get_otsu_threshold(0, 30, 94, 45, (const uint8 *) gray_image);
        my_cv2_threshold((uint8 *) gray_binary_image, 94, 0, 0, 94, 45, bottom_threshold, 1);
        //Ѳ��
        bottom_start_end_x_get();
        get_border_line(80);

        tft180_show_gray_image(15, 0, (const uint8 *) contrast_image, 94, 45, 94, 45,0);
        tft180_show_gray_image(15, 46, (const uint8 *) gray_image, 94, 45, 94, 45,0);
        tft180_show_binary_image(16, 91, (const uint8 *) binary_image, 94, 45);
        tft180_draw_border_line(16, 91, left_border, RGB565_RED);
        tft180_draw_border_line(16, 91, right_border, RGB565_RED);
        tft180_draw_border_line(16, 91, middle_line_single, RGB565_BLUE);
        system_delay_us(200);

        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}

#pragma section all restore
// **************************** �������� ****************************
