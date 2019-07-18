#include "keep_temperature.h"


static int SET_VALUE=0;

static int current_value=0,last_value=0;//��ǰ�¶�ֵ���ϴε��¶�ֵ�������ж��趨�¶��Ƿ�ı�
static int last_duty_cycle=0, duty_cycle=0; //��ǰռ�ձȺ��ϴ�ռ�ձȣ������ж��Ƿ������趨pwm
static int last_value_diff=0, value_diff=0;//��ǰ�¶Ȳ�ֵ���ϴ��¶Ȳ�ֵ�������ж��¶ȱ仯���Ƶ�Ԥ��
	
static int negative_feedback=0,positive_feedback=0,max_positive_feedback=0;	//��������
static int negative_flag=0,positive_flag=0;//�����ļ�����־


Thermometer_t *ThermometerHandle = 0;


Thermometer_t ds18b20=
{
	DS18B20_Init,
	GetTemperatureDegree,
	SetTemperatureDegree,
	KeepTemperatureDegree2
};

Thermometer_t* Ds18b20_GetHandle(void)
{
  return (&ds18b20);
}
void ThermometerChooseHandle(THERMOMETER_type id)
{

  if (id == DS18B20)
  {
    ThermometerHandle = Ds18b20_GetHandle();
	}else if(id == PT1000)
	{
		ThermometerHandle = PT1000_GetHandle();
	}	
}


void SetTemperatureDegree(int degree, TMEPERATURE_type devices)
{
			SET_VALUE=degree;
}

int GetTemperatureDegree(TMEPERATURE_type devices)
{
			return current_value;
}


//��Ԥ��ֵ��1.5�����й��岹������---������ȹ���
//��Ԥ��ֵ��5V�ڽ��м��Ȳ�������---������Ȳ�������
int KeepTemperatureDegree(void)
{

			//����¶����裬���в�����Ҫ����У׼
			if(SET_VALUE!=last_value){
					last_value=SET_VALUE;
					{
							last_duty_cycle=0;
							duty_cycle=0;
						
							positive_feedback=0;
							max_positive_feedback=0;
						
							last_value_diff=0;
							value_diff=0;
						
							negative_feedback=0;
							negative_flag=0;
							positive_flag=0;

					}	
			}
	
	
			current_value =DS18B20_Get_Temp();
			
			if(current_value>=SET_VALUE){
					duty_cycle=500;
					//negative_feedback=0;
			}else{
					//ÿһ���ȼ���������Ҫ����У׼ֵ
					if(SET_VALUE-current_value<10){duty_cycle=465;}
					else if(SET_VALUE-current_value<15){duty_cycle=440;negative_feedback=0;}
					else if(SET_VALUE-current_value<20){duty_cycle=390;negative_feedback=0;}
					else if(SET_VALUE-current_value<35){duty_cycle=300;negative_feedback=0;}
					else if(SET_VALUE-current_value<40){duty_cycle=250;negative_feedback=0;}
					else if(SET_VALUE-current_value<50){duty_cycle=200;negative_feedback=0;}
					else {duty_cycle=0;negative_feedback=0;}
					
					//������λ��
					if(SET_VALUE-current_value<15)
							duty_cycle+=max_positive_feedback;
			}
			
			value_diff = SET_VALUE - current_value;
			//�¶ȼӲ���ȥ
			//����õ����¶�ֵƫ��Խ��Խ�󣬾�Ҫ��ռ�ձȽ��в���
				if((value_diff>0)&&(last_value_diff<=value_diff)){
					negative_flag++;
				//	printf("negative_flag:%d\r\n",negative_flag);
					//���2s���¶�û�б仯����û�дﵽԤ��ֵ��ҲҪ����
					if(negative_flag>=5){
							if(SET_VALUE-current_value<10)negative_feedback+=5;
							else if(SET_VALUE-current_value<15)negative_feedback+=10;
							else if(SET_VALUE-current_value<30)negative_feedback+=20;
							else negative_feedback+=30;
						
							negative_flag = 0;//����ֵ����
					}
					
			//����¶ȹ�����������Ҫ������---����Ԥ��ֵ		
			}else if(value_diff<0){
						positive_flag++;
						if(positive_flag>=5){
								//positive_feedback+=5;
								//ÿһ�����һ�Σ���¼���ֵ�����ݹ�������ֵ��������Ҫ��������
								//if(max_positive_feedback<positive_feedback)
										max_positive_feedback+=5;
								
								positive_flag=0;
						}		
						
			}
			
			last_value_diff=value_diff;
			
			//������λ��
			if(duty_cycle!=500)
					duty_cycle-=negative_feedback;
						
			printf("duty_cycle:%d negative:%d positive:%d---SET_VALUE:%d  current_value:%d\r\n",duty_cycle,negative_feedback,max_positive_feedback,SET_VALUE,current_value);
			
			//���������ۺϺ󣬽��н�������������ۼ����
			
			if(max_positive_feedback>negative_feedback){
					max_positive_feedback-=negative_feedback;
					negative_feedback=0;
			}else {
				  negative_feedback-=max_positive_feedback;
					max_positive_feedback=0;
			}
			
			
			
			
			//�����μ����ռ�ձȺ�֮ǰһ�����Ͳ�ȥ�����趨
			if(last_duty_cycle == duty_cycle)return current_value;
			
			last_duty_cycle = duty_cycle;
			TIM_SetTIM3Compare4(duty_cycle);	
			
			return current_value;
}





//��Ԥ��ֵ��1.5�����й��岹������---������ȹ���
//��Ԥ��ֵ��5V�ڽ��м��Ȳ�������---������Ȳ�������
void KeepTemperatureDegree2(void)
{

			//����¶����裬���в�����Ҫ����У׼
			if(SET_VALUE!=last_value){
					last_value=SET_VALUE;
					{
							last_duty_cycle=0;
							duty_cycle=0;
						
							positive_feedback=0;
							max_positive_feedback=0;
						
							last_value_diff=0;
							value_diff=0;
						
							negative_feedback=0;
							negative_flag=0;
							positive_flag=0;

					}	
			}
	
	
			current_value =DS18B20_Get_Temp();
			
			if(current_value>=SET_VALUE){
					duty_cycle=500;
			}else{
					//ÿһ���ȼ���������Ҫ����У׼ֵ
					if(SET_VALUE-current_value<10&&SET_VALUE<450){duty_cycle=465;}
					else if(SET_VALUE-current_value<15&&SET_VALUE<450){duty_cycle=420;negative_feedback=0;}//�趨�¶ȹ��ߣ���ռ�ձȲ���
					else if(SET_VALUE-current_value<20){duty_cycle=350;if(SET_VALUE<450){negative_feedback=0;}}
					else if(SET_VALUE-current_value<35){duty_cycle=300;negative_feedback=0;}
					else if(SET_VALUE-current_value<40){duty_cycle=250;negative_feedback=0;}
					else if(SET_VALUE-current_value<50){duty_cycle=200;negative_feedback=0;}
					else {duty_cycle=0;negative_feedback=0;}
			}
			
					negative_flag++;
					//���5s���¶�û�б仯����û�дﵽԤ��ֵ��ҲҪ���
				//����4s ��ȴ40s
					if(negative_flag>=5){
							
							positive_flag++;
							value_diff = SET_VALUE - current_value;
						
							if((value_diff>0)&&(last_value_diff<=value_diff))negative_feedback+=3;
							else if(value_diff<0){negative_feedback-=3;negative_flag=0;}
							else if(value_diff==0&&last_value_diff>0)negative_feedback+=3;//����ص�
							else if(value_diff==0&&last_value_diff<0&&positive_flag==10){negative_feedback-=3;negative_flag=0;}//��ȴ�ص�
							negative_flag = 0;//����ֵ����
						
							last_value_diff=value_diff;
					}
					
			
			
			//1.5V���ⲻ���㲹��ֵ
		  //�����40->25����˴����ļ������				
			if(current_value-SET_VALUE>15)negative_feedback=0;		
					
			//������λ��
			if(duty_cycle!=500)
					duty_cycle-=negative_feedback;
						
			printf("duty_cycle:%d negative:%d ---SET_VALUE:%d  current_value:%d\r\n",duty_cycle,negative_feedback,SET_VALUE,current_value);
			
			
			//�����μ����ռ�ձȺ�֮ǰһ�����Ͳ�ȥ�����趨
			if(last_duty_cycle == duty_cycle)return ;
			
			last_duty_cycle = duty_cycle;
			TIM_SetTIM3Compare4(duty_cycle);	
			
			return ;
}
