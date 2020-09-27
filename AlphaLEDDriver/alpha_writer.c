#include "pca9685.h"

#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIN_BASE_AP 300    // Provides PIN reference on the first PCA9685 board
#define PIN_BASE_QZ 320    // Provides PIN reference on the second PCA9685 board
#define MAX_PWM 4096    // Provides maximum PWN value
#define FREQUENCY 1000  // Rate at which lights flash (ms)

/**
 * Sets up flash and delay duration and links each letter to corresponding
 * pin on the pca9685 boards
 *
 * letter:				letter to flash
 * flash_duration_ms:	time the led will be illumianted for (ms)
 * kerning_ms:			pause after each flash (ms)
 */
void flash_letter
(
    char letter,              ///<IN The letter to flash
    const int flash_duration_ms,    ///<IN The time the letter will be illuminated (in ms)
    const int kerning_ms            ///<IN The time pause after this letter
)
{
    letter = tolower(letter);
    const int led = (letter < (int)'q') ? (PIN_BASE_AP + (letter - (int)'a')) : (PIN_BASE_QZ + (letter - (int)'q'));
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

    // Tests for error in setup
    int fd_ap = pca9685Setup(PIN_BASE_AP, 0x40, FREQUENCY);
    int fd_qz = pca9685Setup(PIN_BASE_QZ, 0x41, FREQUENCY);
    if (fd_ap < 0)
    {
        printf("Error setting up first pca9685 at GPIO, check pin mappings\n");
        return fd_ap;
    }
    if (fd_qz < 0)
    {
        printf("Error setting up second pca9685 at GPIO, check pin mappings\n");
        return fd_qz;
    }

    // Sets all leds back to default values
    pca9685PWMReset(fd_ap);
    pca9685PWMReset(fd_qz);

    int active = 1;
    while (active)
    {
        for (int i = 1; i < argc; ++i)
        {
            printf("Message: %s\n", argv[i]);
            for (int j = 0; j < strlen(argv[i]); ++j)
            {
                printf("%c\n", *(argv[i] + j));
                flash_letter(argv[i][j], 1000, 500);
            }
        }

        // Was using this for testing, not needed anymore
        // char message[] = "abcdefghijklmnop";
        // for (int i = 0; i < strlen(message); ++i)
        // {
        // 	printf("test");
        //     flash_letter(message[i], 500, 500);
        // }

        pwmWrite(PIN_BASE_AP + 16, 0);  // Pins powered while 0 < i < 4096 for first pca9685
        pwmWrite(PIN_BASE_QZ + 16, 0);  // Pins powered while 0 < i < 4096 for second pca9685

    }

    return 0;
}
