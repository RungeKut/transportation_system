/*
 * Flash.h
 *
 *  Created on: 24 мар. 2021 г.
 *      Author: arkhipov.a
 */

#ifndef INC_FLASH_H_
#define INC_FLASH_H_

#include "main.h"
#include "constatnts.h"
using namespace constants;


class Flash {

			public:
						void flashWrite (uint32_t Address , uint32_t Data);
						void flashRead (unsigned int &DatatoRead , uint32_t Address);
						void flashRead (Turn_t &DatatoRead , uint32_t Address);
						void flashRead (bool &DatatoRead , uint32_t Address);
			private:
						void flashErase (uint32_t pageAddress , int npages);

};

#endif /* INC_FLASH_H_ */
