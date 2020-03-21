#include "pca9685.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 1000

void flash_letter
(
    const char letter,              ///<IN The letter to flash
    const int flash_duration_ms,    ///<IN The time the letter will be illuminated (in ms)
    const int kerning_ms            ///<IN The time pause after this letter
)
{
    const int led = PIN_BASE + (tolower(letter) - (int)'a');
    const int pwm_step = 32;
    const int iterations =  (int)((float)MAX_PWM / pwm_step);
    const int d = flash_duration_ms / (iterations);
    for (int i = 0; i < MAX_PWM; i += pwm_step)
    {
        pwmWrite(led, i);
        delay(d);
    }
    pwmWrite(led, 0);
    delay(kerning_ms);
}

int main(int argc, char *argv[])
{
    printf("PCA9685 LED example\n");
    printf("There are %d args\n", argc);
    for (int i = 1; i < argc; ++i)
    {
        printf("%s\n", argv[i]);
    }

    // Calling wiringPi setup first.
    wiringPiSetup();

    int fd = pca9685Setup(PIN_BASE, 0x40, HERTZ);
    if (fd < 0)
    {
        printf("Error in setup\n");
        return fd;
    }

    pca9685PWMReset(fd);

    int i, j;
    int active = 1;

    while (active)
    {
        for (int i = 1; i < argc; ++i)
        {
            printf("Message: %s\n", argv[i]);
            for (int j = 0; j < strlen(argv[i]); ++j)
            {
                printf("%c", *(argv[i] + j));
                flash_letter(argv[i][j], 1000, 500);
            }
        }


        // char message[] = "dadda";
        // while (message)
        // {
        //     flash_letter(*message, 1000, 500);
        //     message++;
        // }
        // for (i = 0; i < strlen(message); ++i)
        // {
        //     flash_letter(message[i], 1000, 500);
        // }
        // // flash_letter('A', 2000, 500);
        // flash_letter('d', 3000, 200);
        // flash_letter('a', 500, 250);
        // flash_letter('A', 4000, 400);
        // flash_letter('D', 2000, 100);
        // flash_letter('B', 1000, 200);
        // for (j = 0; j < 5; j++)
        // {
        //     for (i = 0; i < MAX_PWM; i += 32)
        //     {
        //         pwmWrite(PIN_BASE + 0, i);
        //         delay(4);
        //     }

        //     for (i = 0; i < MAX_PWM; i += 32)
        //     {
        //         pwmWrite(PIN_BASE + 0, MAX_PWM - i);
        //         delay(2);
        //     }
        // }

        // pwmWrite(PIN_BASE + 16, 0);
        // delay(500);

        // for (j = 0; j < 5; j++)
        // {
        //     for (i = 0; i < 16; i++)
        //     {
        //         pwmWrite(PIN_BASE + i, MAX_PWM);
        //         delay(20);
        //     }

        //     for (i = 0; i < 16; i++)
        //     {
        //         pwmWrite(PIN_BASE + i, 0);
        //         delay(20);
        //     }
        // }

        pwmWrite(PIN_BASE + 16, 0);
        delay(500);
    }


    return 0;
}