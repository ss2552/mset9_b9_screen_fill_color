typedef unsigned int u32;
typedef volatile u32 vu32;

#define AUTO_EXIT_COUNT 999999

#define LCD_REGS_BASE           0x10202000

#define HID_PAD                (*(vu32 *)0x10146000 ^ 0xFFF)

#define LCD_TOP_FILL_REG        *(vu32 *)(LCD_REGS_BASE + 0x200 + 4)
// #define LCD_BOTTOM_FILL_REG     *(vu32 *)(LCD_REGS_BASE + 0xA00 + 4)
#define LCD_FILL_ENABLE         (1u << 24)

#define BUTTON_R1              (1 << 8)
#define BUTTON_L1              (1 << 9)
#define BUTTON_A               (1 << 0)
#define BUTTON_B               (1 << 1)
#define BUTTON_X               (1 << 10)
#define BUTTON_Y               (1 << 11)
#define BUTTON_SELECT          (1 << 2)
#define BUTTON_START           (1 << 3)
#define BUTTON_RIGHT           (1 << 4)
#define BUTTON_LEFT            (1 << 5)
#define BUTTON_UP              (1 << 6)
#define BUTTON_DOWN            (1 << 7)

#define COLOR_BLACK   0x000000
#define COLOR_RED     0x0000FF
#define COLOR_GREEN   0x00FF00
#define COLOR_YELLOW  0x00FFFF
#define COLOR_BLUE    0xFF0000
#define COLOR_MAGENTA 0xFF00FF
#define COLOR_AQUA    0xFFFF00
#define COLOR_WHITE   0xFFFFFF


void fill_screen(u32 c){
    
    LCD_TOP_FILL_REG = LCD_FILL_ENABLE | c;
    
}

void main(){

    u32 n = 0;

    // fill black screen
    fill_screen(COLOR_BLACK);
    
loop:

    if(n > AUTO_EXIT_COUNT){
        fill_screen(COLOR_WHITE);
        while(n - 1000);
        
    }

    switch(HID_PAD){
        case BUTTON_A:

            if(n != 1){
                n = 1;
                
                fill_screen(COLOR_RED);
            }
            
            goto loop;

        case BUTTON_B:
        
            if(n != 2){
                n = 2;
                
                fill_screen(COLOR_GREEN);
            }

            goto loop;

        case BUTTON_X:
            
            if(n != 3){
                n = 3;
                    
                fill_screen(COLOR_YELLOW);
            }

            goto loop;

        case BUTTON_Y:
                
            if(n != 4){
                n = 4;
                
                fill_screen(COLOR_BLUE);
            }

            goto loop;

        case BUTTON_LEFT:
            
            if(n != 5){
                n = 5;
                
                fill_screen(COLOR_MAGENTA);
            }

            goto loop;

        case BUTTON_RIGHT:
            
            if(n != 6){
                n = 6;
                
                fill_screen(COLOR_AQUA);
            }

            goto loop;
    
        case 0x000000:

            n++;

            goto loop;

        default:
        
            fill_screen(COLOR_WHITE);
            
            while(n++ > AUTO_EXIT_COUNT);
            
    }
}
