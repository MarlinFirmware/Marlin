#include "mks_wifi.h"


#ifdef MKS_WIFI

#include "../../lcd/marlinui.h"
#include "mks_wifi_sd.h"

volatile uint8_t mks_in_buffer[MKS_IN_BUFF_SIZE];
uint8_t mks_out_buffer[MKS_OUT_BUFF_SIZE];

volatile uint8_t esp_packet[MKS_TOTAL_PACKET_SIZE];

MKS_WIFI_INFO mks_wifi_info;// __attribute__ ((section (".ccmram")));

void mks_wifi_init(void){

	SERIAL_ECHO_MSG("Init MKS WIFI");	
    DEBUG("Init MKS WIFI");

	memset(&mks_wifi_info, 0, sizeof(mks_wifi_info));
	
	SET_OUTPUT(MKS_WIFI_IO0);
	WRITE(MKS_WIFI_IO0, HIGH);

	SET_OUTPUT(MKS_WIFI_IO4);
	WRITE(MKS_WIFI_IO4, HIGH);

	SET_OUTPUT(MKS_WIFI_IO_RST);
	WRITE(MKS_WIFI_IO_RST, LOW);

	ui.set_status((const char *)"WIFI: waiting... ",false);

	safe_delay(200);	
	WRITE(MKS_WIFI_IO_RST, HIGH);
	
	safe_delay(200);	
	WRITE(MKS_WIFI_IO4, LOW);
}


void mks_wifi_set_param(void){
	uint32_t packet_size;
	ESP_PROTOC_FRAME esp_frame;


	uint32_t ap_len = strlen((const char *)MKS_WIFI_SSID);
	uint32_t key_len = strlen((const char *)MKS_WIFI_KEY);


	memset(mks_out_buffer, 0, MKS_OUT_BUFF_SIZE);

	mks_out_buffer[0] = WIFI_MODE_STA;

	mks_out_buffer[1] = ap_len;
	memcpy((char *)&mks_out_buffer[2],(const char *)MKS_WIFI_SSID,ap_len);

	mks_out_buffer[2+ap_len] = key_len;
	memcpy((char *)&mks_out_buffer[2 + ap_len + 1], (const char *)MKS_WIFI_KEY, key_len);

	esp_frame.type=ESP_TYPE_NET;
	esp_frame.dataLen= 2 + ap_len + key_len + 1;
	esp_frame.data=mks_out_buffer;
	packet_size=mks_wifi_build_packet((uint8_t *)esp_packet,&esp_frame);

	if(packet_size > 8){ //4 байта заголовка + 2 байта длины + хвост + название сети и пароль
		//выпихнуть в uart
		mks_wifi_send((uint8_t *)esp_packet, packet_size);
	};
}

/*
Получает данные из всех функций, как только
есть перевод строки 0x0A, формирует пакет для
ESP и отправляет
*/
void mks_wifi_out_add(uint8_t *data, uint32_t size){
	static uint32_t line_index=0;
	uint32_t packet_size;
	ESP_PROTOC_FRAME esp_frame;

	while (size--){
		if(*data == 0x0a){
			//Перевод строки => сформировать пакет, отправить, сбросить индекс
			esp_frame.type=ESP_TYPE_FILE_FIRST; //Название типа из прошивки MKS. Смысла не имееет.
			esp_frame.dataLen=strnlen((char *)mks_out_buffer,MKS_OUT_BUFF_SIZE);
			esp_frame.data=mks_out_buffer;
			packet_size=mks_wifi_build_packet((uint8_t *)esp_packet,&esp_frame);

			if(packet_size == 0){
				ERROR("Build packet failed");
				line_index=0;
				memset(mks_out_buffer,0,MKS_OUT_BUFF_SIZE);
				break;
			}
			//выпихнуть в uart
			mks_wifi_send((uint8_t *)esp_packet, packet_size);
			//очистить буфер
			memset(mks_out_buffer,0,MKS_OUT_BUFF_SIZE);
			//сбросить индекс
			line_index=0;
		}else{
			//писать в буфер			
			mks_out_buffer[line_index++]=*data++;
		}

		if(line_index >= MKS_OUT_BUFF_SIZE){
			ERROR("Max line size");
			line_index=0;
			memset(mks_out_buffer,0,MKS_OUT_BUFF_SIZE);
			break;
		}
	}
}

uint8_t mks_wifi_input(uint8_t data){
	ESP_PROTOC_FRAME esp_frame;
	#ifdef MKS_WIFI_ENABLED_WIFI_CONFIG 
	static uint8_t get_packet_from_esp=0;
	#endif
	static uint8_t packet_start_flag=0;
	static uint8_t packet_type=0;
	static uint16_t packet_index=0;
	static uint16_t payload_size=ESP_PACKET_DATA_MAX_SIZE;
	uint8_t ret_val=1;

	//Не отдавать данные в очередь команд, если идет печать
	// if (CardReader::isPrinting()){
	// 	DEBUG("No input while printing");
	// 	return 1;
	// }	
	
	if(data == ESP_PROTOC_HEAD){
		payload_size = ESP_PACKET_DATA_MAX_SIZE;
		packet_start_flag=1;
		packet_index=0;
		memset((uint8_t*)mks_in_buffer,0,MKS_IN_BUFF_SIZE);
	}else if(!packet_start_flag){
		DEBUG("Byte not in packet %0X",data);
		return 1;
	}

	if(packet_start_flag){
		mks_in_buffer[packet_index]=data;
	}

	if(packet_index == 1){
		packet_type = mks_in_buffer[1];
	}

	if(packet_index == 3){
		payload_size = uint16_t(mks_in_buffer[3] << 8) | mks_in_buffer[2];

		if(payload_size > ESP_PACKET_DATA_MAX_SIZE){
			ERROR("Payload size too big");
			packet_start_flag=0;
			packet_index=0;
			memset((uint8_t*)mks_in_buffer,0,MKS_IN_BUFF_SIZE);
			return 1;
		}

	}

	if( (packet_index >= (payload_size+4)) || (packet_index >= ESP_PACKET_DATA_MAX_SIZE) ){

		if(mks_wifi_check_packet((uint8_t *)mks_in_buffer)){
			ERROR("Packet check failed");
			packet_start_flag=0;
			packet_index=0;
			return 1;
		}


		esp_frame.type = packet_type;
		esp_frame.dataLen = payload_size;
		esp_frame.data = (uint8_t*)&mks_in_buffer[4];

		mks_wifi_parse_packet(&esp_frame);

		#ifdef MKS_WIFI_ENABLED_WIFI_CONFIG 
		if(!get_packet_from_esp){
			DEBUG("Fisrt packet from ESP, send config");
		
			mks_wifi_set_param();
			get_packet_from_esp=1;
		}
		#endif
		packet_start_flag=0;
		packet_index=0;
	}

	/* Если в пакете G-Сode, отдаем payload дальше в обработчик марлина */
	// if((packet_type == ESP_TYPE_GCODE) && 
	//    (packet_index >= 4) && 
	//    (packet_index < payload_size+5) 
	//   ){

	// 	if(!check_char_allowed(data)){
	// 		ret_val=0;
	// 	}else{
	// 		ERROR("Char not allowed: %0X %c",data,data);
	// 		packet_start_flag=0;
	// 		packet_index=0;
	// 		return 1;
	// 	}
		
	// }

	if(packet_start_flag){
		packet_index++;
	}

	return ret_val;
}

/*
Проверяет, что символы из текстового диапазона
для G-code команд
*/
uint8_t check_char_allowed(char data){

	if( data == 0x0a || data == 0x0d){
		return 0;
	}

	if( (data >= 0x20) && (data <= 0x7E) ){
		return 0;
	}

	return 1;

}

/*
Проверяет пакет на корректность:
наличие заголовка
наличие "хвоста"
длина пакета
*/
uint8_t mks_wifi_check_packet(uint8_t *in_data){
	uint16_t payload_size;
	uint16_t tail_index;

	if(in_data[0] != ESP_PROTOC_HEAD){
		ERROR("Packet head mismatch");
		return 1;
	}

	payload_size = uint16_t(in_data[3] << 8) | in_data[2];
	
	if(payload_size > ESP_PACKET_DATA_MAX_SIZE){
		ERROR("Payload size mismatch");
		return 1;
	}
	
	tail_index = payload_size + 4;

	if(in_data[tail_index] != ESP_PROTOC_TAIL ){
		ERROR("Packet tail mismatch");
		return 1;
	}

	return 0;
}


void mks_wifi_parse_packet(ESP_PROTOC_FRAME *packet){
	static uint8_t show_ip_once=0;
	char str[100];

	switch(packet->type){
		case ESP_TYPE_NET:
			memset(str,0,100);
			if(packet->data[6] == ESP_NET_WIFI_CONNECTED){
				if(show_ip_once==0){
					show_ip_once=1;
					sprintf(str,"IP %d.%d.%d.%d",packet->data[0],packet->data[1],packet->data[2],packet->data[3]);
					ui.set_status((const char *)str,true);
					SERIAL_ECHO_START();
					SERIAL_ECHOLN((char*)str);	

					//Вывод имени сети
					
					if(packet->data[8] < 100){
						memcpy(str,&packet->data[9],packet->data[8]); 
						str[packet->data[8]]=0;
						SERIAL_ECHO_START();
						SERIAL_ECHO("WIFI: ");
						SERIAL_ECHOLN((char*)str);
					}else{
						DEBUG("Network NAME too long");
					}
				}
//				DEBUG("[Net] connected, IP: %d.%d.%d.%d",packet->data[0],packet->data[1],packet->data[2],packet->data[3]);
			}else if(packet->data[6] == ESP_NET_WIFI_EXCEPTION){
				DEBUG("[Net] wifi exeption");
			}else{
				DEBUG("[Net] wifi not config");
			}
			mks_wifi_info.connected = (packet->data[6] == 0x0A);
			memcpy(&(mks_wifi_info.ip), &(packet->data[0]), 4);
			mks_wifi_info.mode = packet->data[7];
			if (packet->data[8] < 32)
				strncpy(mks_wifi_info.net_name, (char*)&(packet->data[9]), packet->data[8]);
			else
				strncpy(mks_wifi_info.net_name, (char*)&(packet->data[9]), 31);
			break;
		case ESP_TYPE_GCODE:
				char gcode_cmd[50];
				uint32_t cmd_index;
				// packet->data[packet->dataLen] = 0;
				
				cmd_index = 0;
				memset(gcode_cmd,0,50);
				for(uint32_t i=0; i<packet->dataLen; i++){
					
					if(packet->data[i] != 0x0A){
						gcode_cmd[cmd_index++] = packet->data[i];
					}else{
						GCodeQueue::ring_buffer.enqueue((const char *)gcode_cmd, false, MKS_WIFI_SERIAL_NUM);
						cmd_index = 0;
						memset(gcode_cmd,0,50);
					}

				}
				
				
			break;
		case ESP_TYPE_FILE_FIRST:
				DEBUG("[FILE_FIRST]");
				//Передача файла останавливает все процессы, 
				//поэтому печать в этот момент не возможна.
				if (!CardReader::isPrinting()){
					mks_wifi_start_file_upload(packet);
				}
			break;
		case ESP_TYPE_FILE_FRAGMENT:
				DEBUG("[FILE_FRAGMENT]");
			break;
		case ESP_TYPE_WIFI_LIST:
			DEBUG("[WIFI_LIST]");
			break;
		default:
			DEBUG("[Unkn]");
		 	break;

	}

}



uint16_t mks_wifi_build_packet(uint8_t *packet, ESP_PROTOC_FRAME *esp_frame){
	uint16_t packet_size=0;

	memset(packet,0,MKS_TOTAL_PACKET_SIZE);
	packet[0] = ESP_PROTOC_HEAD;
	packet[1] = esp_frame->type;

	for(uint32_t i=0; i < esp_frame->dataLen; i++){
		packet[i+4]=esp_frame->data[i]; //4 байта заголовка отступить
	}

	packet_size = esp_frame->dataLen + 4;

	if(packet_size > MKS_TOTAL_PACKET_SIZE){
		ERROR("ESP packet too big");
		return 0;
	}

	if(esp_frame->type != ESP_TYPE_NET){
		packet[packet_size++] = 0x0d;
		packet[packet_size++] = 0x0a; 
		esp_frame->dataLen = esp_frame->dataLen + 2; //Два байта на 0x0d 0x0a
	}
	
	*((uint16_t *)&packet[2]) = esp_frame->dataLen;

	packet[packet_size] = ESP_PROTOC_TAIL;
	return packet_size;
}


void mks_wifi_send(uint8_t *packet, uint16_t size){

	for( uint32_t i=0; i < (uint32_t)(size+1); i++){
		while(MYSERIAL2.availableForWrite()==0){
			safe_delay(10);				
		}
		MYSERIAL2.write(packet[i]);
	}
}
#else
void mks_wifi_out_add(uint8_t *data, uint32_t size){
	while(size--){
		MYSERIAL2.write(*data++);
	}
	return;
};

#endif