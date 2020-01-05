/*
*   File Name:graphics_driver.h
*   Written By: Tanner Kern
*   Purpose: Simple Graphics driver for derpOS, supports full screen pixel mode and emulated text mode
*/

void graphics_add_screen(unsigned int* mem, int width, int height, int process_id);