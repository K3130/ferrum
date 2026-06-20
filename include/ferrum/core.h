#ifndef FERRUM_CORE_H
#define FERRUM_CORE_H

#define FERRUM_VERSION_MAJOR 0
#define FERRUM_VERSION_MINOR 1
#define FERRUM_VERSION_PATCH 0

typedef enum 
{
	FR_OK            =  0,
	FR_ERR           = -1,
	FR_ERR_NOMEM     = -2,
	FR_ERR_INVAL     = -3,
	FR_ERR_TIMEOUT   = -4,
	FR_ERR_CONN      = -5,
} fr_result_t;



#endif /* FERRUM_CORE_H */