typedef unsigned int u32;
typedef volatile u32 vu32;

#define LCD_REGS_BASE           0x10202000

#define HID_PAD                (*(vu32 *)0x10146000 ^ 0xFFF)

#define LCD_TOP_FILL_REG        *(vu32 *)(LCD_REGS_BASE + 0x200 + 4)
#define LCD_BOTTOM_FILL_REG     *(vu32 *)(LCD_REGS_BASE + 0xA00 + 4)
#define LCD_FILL_ENABLE         (1u << 24)

#define BUTTON_A               (1 << 0)
#define BUTTON_B               (1 << 1)
#define BUTTON_X               (1 << 10)
#define BUTTON_Y               (1 << 11)
#define BUTTON_LEFT            (1 << 5)
#define BUTTON_RIGHT           (1 << 4)

#define COLOR_BLACK   0x000000
#define COLOR_RED     0x0000FF
#define COLOR_GREEN   0x00FF00
#define COLOR_YELLOW  0x00FFFF
#define COLOR_BLUE    0xFF0000
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_AQUA    0xFFFF00
#define COLOR_WHITE   0xFFFFFF



int main(void){

    u32 n = 0;

    LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_BLACK;
    LCD_BOTTOM_FILL_REG = LCD_FILL_ENABLE | COLOR_WHITE;


loop:

    switch(HID_PAD){
        case BUTTON_A:

            if(n != 1){

                LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_RED;
                n = 1;

            }
            
            goto loop;

        case BUTTON_B:
        
            if(n != 2){
                
                LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_GREEN;
                n = 2;

            }

            goto loop;

        case BUTTON_X:
            
            if(n != 3){
                    
                LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_YELLOW;
                n = 3;

            }

            goto loop;

        case BUTTON_Y:
                
            if(n != 4){

                LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_BLUE;
                n = 4;
                
            }

            goto loop;

        case BUTTON_LEFT:
            
            if(n != 5){

                LCD_TOP_FILL_REG = LCD_FILL_ENABLE |  COLOR_MAGENTA;
                n = 5;
                
            }

            goto loop;

        case BUTTON_RIGHT:
            
            if(n != 6){
                n = 6;
                
                LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_AQUA;
            }

            goto loop;
    
        case 0x000000:

            n++;

            goto loop;

        default:
        
            LCD_TOP_FILL_REG = LCD_FILL_ENABLE | COLOR_WHITE;
            
            return;
    }
}
