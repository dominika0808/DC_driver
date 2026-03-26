#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>
#include "common.h"

Status_t Usart3DMAInit(void);
Status_t Usart3TransmitDMA(uint32_t addr_tx);
Status_t Usart3ReceiverDMA(uint32_t addr_rx);

#endif
