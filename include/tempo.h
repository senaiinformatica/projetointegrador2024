#pragma once

extern unsigned long timerDelay;
//Retorna a hora atual no formato hh:mm:ss
void hora_certa();
void salvarValor();
/*
@brief Retorna tempo no formato POSIX
@return tempo no formato POSIX
*/
unsigned long timeStamp();

//Configura o servidor NTP
void setup_time();

//Atualiza o servidor NTP
void atualiza_time();