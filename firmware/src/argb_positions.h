/* ************************************************************************** */


#ifndef _ARGB_POS_H    /* Guard against multiple inclusion */
#define _ARGB_POS_H


#include "argb.h"


#define ARGB_POS_CPUBLOCK_X 173000
#define ARGB_POS_CPUBLOCK_Y 109500
#define ARGB_POS_CPUBLOCK_Z 101000

#define ARGB_POS_GPUBLOCK_X  43900
#define ARGB_POS_GPUBLOCK_Y  22300
#define ARGB_POS_GPUBLOCK_Z 273400

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    const uint32_t argb_positions[4][3 * ARGB_MAX_LENGTH] = {
        { //Channel A - PC Component LEDs
            //CPU Block
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  15000, ARGB_POS_CPUBLOCK_Z +   5000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +   5000, ARGB_POS_CPUBLOCK_Z +  12000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +   5000, ARGB_POS_CPUBLOCK_Z +  26000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +   5000, ARGB_POS_CPUBLOCK_Z +  43000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +   5000, ARGB_POS_CPUBLOCK_Z +  60000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  14000, ARGB_POS_CPUBLOCK_Z +  65000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  31000, ARGB_POS_CPUBLOCK_Z +  65000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  48000, ARGB_POS_CPUBLOCK_Z +  65000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  65000, ARGB_POS_CPUBLOCK_Z +  65000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  65000, ARGB_POS_CPUBLOCK_Z +  50000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  65000, ARGB_POS_CPUBLOCK_Z +  33000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  65000, ARGB_POS_CPUBLOCK_Z +  17000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  58000, ARGB_POS_CPUBLOCK_Z +   5000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  44000, ARGB_POS_CPUBLOCK_Z +   5000,
            ARGB_POS_CPUBLOCK_X +  10000, ARGB_POS_CPUBLOCK_Y +  27000, ARGB_POS_CPUBLOCK_Z +   5000,
                    
            //GPU Block
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 227000, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 210333, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 193667, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 177000, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 160333, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 143667, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 127000, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y + 110333, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y +  93667, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y +  77000, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y +  60333, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y +  43667, ARGB_POS_GPUBLOCK_Z + 110000,
            ARGB_POS_GPUBLOCK_X +  13000, ARGB_POS_GPUBLOCK_Y +  27000, ARGB_POS_GPUBLOCK_Z + 110000,
                    
            //Rest just zero
            0
        },
        { //Channel B - Case LEDs
            //Hexagon
            186231, 258169, 393000,
            184522, 264548, 393000,
            182813, 270927, 393000,
            181104, 277306, 393000,
            179394, 283685, 393000,
            177685, 290064, 393000,
            175976, 296443, 393000,
                    
            166656, 305736, 393000,
            160277, 307445, 393000,
            153898, 309154, 393000,
            147519, 310863, 393000,
            141140, 312573, 393000,
            134761, 314282, 393000,
            128382, 315991, 393000,
                    
            115675, 312567, 393000,
            111005, 307897, 393000,
            106336, 303227, 393000,
            101666, 298557, 393000,
             96996, 293888, 393000,
             92326, 289218, 393000,
             87657, 284548, 393000,
                    
             84269, 271831, 393000,
             85978, 265452, 393000,
             87687, 259073, 393000,
             89396, 252694, 393000,
             91106, 246315, 393000,
             92815, 239936, 393000,
             94524, 233557, 393000,
                    
            103844, 224264, 393000,
            110223, 222555, 393000,
            116602, 220846, 393000,
            112981, 219137, 393000,
            129360, 217427, 393000,
            135739, 215718, 393000,
            142118, 214009, 393000,
                    
            154825, 217433, 393000,
            159495, 222103, 393000,
            164164, 226773, 393000,
            168834, 231443, 393000,
            173504, 236112, 393000,
            178174, 240782, 393000,
            182843, 245452, 393000,
                    
            //Reservoir
            205000, 408875, 305793,
            205000, 408875, 299189,
            205000, 408875, 292585,
            205000, 408875, 285981,
            205000, 408875, 279377,
            205000, 408875, 272773,
            205000, 408875, 266169,
                    
            205000, 408875, 255793,
            205000, 408875, 249189,
            205000, 408875, 242585,
            205000, 408875, 235981,
            205000, 408875, 229377,
            205000, 408875, 222773,
            205000, 408875, 216169,
                    
            205000, 408875, 205793,
            205000, 408875, 199189,
            205000, 408875, 192585,
            205000, 408875, 185981,
            205000, 408875, 179377,
            205000, 408875, 172773,
            205000, 408875, 166169,
                    
            //Logo
            215000, 354000,  60000,
            215000, 354000,  50000,
            215000, 363000,  50000,
            215000, 372000,  50000,
            215000, 381000,  50000,
            215000, 382000,  58000,
            215000, 380000,  66000,
            215000, 372000,  70000,
            215000, 363000,  70000,
            215000, 354000,  70000,
            215000, 354000,  80000,
            215000, 354000,  90000,
            215000, 363000,  90000,
            215000, 372000,  90000,
            215000, 381000,  90000,
            215000, 382000,  82000,
            215000, 380000,  74000,
                    
            215000, 398000,  51000,
            215000, 398000,  61000,
            215000, 398000,  71000,
            215000, 398000,  81000,
            215000, 400000,  88000,
            215000, 409000,  88000,
            215000, 414000,  81000,
            215000, 414000,  71000,
            215000, 414000,  61000,
            215000, 414000,  51000,
            215000, 419000,  88000,
            215000, 428000,  88000,
            215000, 430000,  81000,
            215000, 430000,  71000,
            215000, 430000,  61000,
            215000, 430000,  51000,
                    
            //Front
            244793, 495000, 442231,
            238189, 495000, 442231,
            231585, 495000, 442231,
            224981, 495000, 442231,
            218377, 495000, 442231,
            211773, 495000, 442231,
            205169, 495000, 442231,
                    
            194793, 495000, 442231,
            188189, 495000, 442231,
            181585, 495000, 442231,
            174981, 495000, 442231,
            168377, 495000, 442231,
            161773, 495000, 442231,
            155169, 495000, 442231,
                    
            144793, 495000, 442231,
            138189, 495000, 442231,
            131585, 495000, 442231,
            124981, 495000, 442231,
            118377, 495000, 442231,
            111773, 495000, 442231,
            105169, 495000, 442231,
                    
             94793, 495000, 442231,
             88189, 495000, 442231,
             81585, 495000, 442231,
             74981, 495000, 442231,
             68377, 495000, 442231,
             61773, 495000, 442231,
             55169, 495000, 442231,
                    
            //Left
              5000, 455793, 402170,
              5000, 449189, 402170,
              5000, 442585, 402170,
              5000, 435981, 402170,
              5000, 429377, 402170,
              5000, 422773, 402170,
              5000, 416169, 402170,
                    
              5000, 180793, 376835,
              5000, 174189, 376835,
              5000, 167585, 376835,
              5000, 160981, 376835,
              5000, 154377, 376835,
              5000, 147773, 376835,
              5000, 141169, 376835,
            
              5000, 130793, 376835,
              5000, 124189, 376835,
              5000, 117585, 376835,
              5000, 110981, 376835,
              5000, 104377, 376835,
              5000,  97773, 376835,
              5000,  91169, 376835,
                    
              5000,  80793, 376835,
              5000,  74189, 376835,
              5000,  67585, 376835,
              5000,  60981, 376835,
              5000,  54377, 376835,
              5000,  47773, 376835,
              5000,  41169, 376835,
                    
            //Rest just zero
            0
        },
        { //Channel C - Extra LEDs
            0
        },
        { //Front panel (others zero)
            239500, 444800,      0,
            0
        }
    };
    
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* multiple inclusion */

/* *****************************************************************************
 End of File
 */
